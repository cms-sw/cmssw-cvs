#ifndef VIS_G4_CORE_VIS_G4_VIS_SCENE_HANDLER_H
# define VIS_G4_CORE_VIS_G4_VIS_SCENE_HANDLER_H

//<<<<<< INCLUDES							>>>>>>

# include "VisGeant4/VisG4Core/interface/config.h"
# include <G4VGraphicsScene.hh>
# include <G4ViewParameters.hh>
# include <G4VisExtent.hh>
# include <G4Polymarker.hh>
# include <G4VMarker.hh>
# include <G4Material.hh>
# include <G4THitsMap.hh>
# include <stack>
# include <map>

//<<<<<< PUBLIC DEFINES							>>>>>>
//<<<<<< PUBLIC CONSTANTS						>>>>>>
//<<<<<< PUBLIC TYPES							>>>>>>

class VisG4VisSystem;
class VisG4Path;
class G4Visible;
class G4VisAttributes;
class SoSeparator;
class SoNode;
class SoGroup;
class SoDrawStyle;
class SoResetTransform;
class SoMarkerSet;
class SoMaterial;
class SoShape;
class SbVec3f;

//<<<<<< PUBLIC VARIABLES						>>>>>>
//<<<<<< PUBLIC FUNCTIONS						>>>>>>
//<<<<<< CLASS DECLARATIONS						>>>>>>

/* A quick introduction to Geant 4 volumes
  
   A LOGICAL VOLUME has a SOLID (which could be a complex object, such
   as a boolean solid).	 The logical volume also has a number of
   PHYSICAL daughter VOLUMEs, each of which has positions relative to
   the mother a logical volume.	 A single logical volume can be placed
   several times via several physical volumes (see pictures below).
   Normally all daughters of a logical volume are replicated in all
   copies of that logical volume, but as an exception a physical
   daughter may also have a PHYSICAL MOTHER, meaning that the daughter
   is a daughter of its logical mother only when the logical mother is
   being positioned by the given physical mother.  (In other words, a
   daughter may appear in only one of the copies of its mother.)
   Finally, a physical daughter volume may be REPLICATED, resulting in
   multiple copies of its logical volume in several positions, or
   PARAMETRISED, resulting in a number of copies of the logical volume
   in arbitrary transforms and with arbitrary solids.  */

/* This is essentially a rewrite of the G4 visualisation framework.
   The principal reason for this is that as of G4 version 4.3.2, the
   G4 visualisation framework is unsuitable for interactive use, for
   two major reasons.

   Firstly, the framework does not provide enough information about
   the processing of the volumes to allow the scene handler to map the
   representations it is creating to the original volume tree (it does
   provide information about the volumes it is processing, but not in
   a manner that allows the scene graph tree to be constructed).  As a
   consequence, picking is not possible.  It also makes it painful to
   try to reuse the representations like G4 does with logical volumes
   (but see below why we don't want to do this anyway).	 This same
   issue applies also to the event content (hits and trajectories).

   Secondly, the G4 framework has not been designed for incremental
   rendering.  We need to allow users to choose which parts of the
   detector and event are to be visualised, and with what parameters;
   each change of settings ought to allow for immediate change of the
   representation.  With the existing support in G4 it is not easy to
   (re)process only a particular volume without disturbing the rest of
   the existing scene graph.

   Therefore we have rewritten parts of the G4 visualisation framework
   in a way that matches our needs.  */

// FIXME: can't use G4 visualisation because the stuff below is far
// too complicated to get to work reliably, and even then we can't do
// things incrementally/interactively...

/* Try to determine what is going on: reuse nodes for a logical volume
   if possible, and build a scene graph for the nodes.	For reuse,
   reposition the same solid representation several times (just as G4
   does with logical volumes).	It is safe to not get the replication
   completely right, we'll just end up doing more work.	 However, the
   scene graph we do want to get right -- it is our primary output!
  
   We need to distinguish between two situations: 1) we are still
   building the solid representation for the logical volume and
   `AddThis' should append to the current definition, and 2) the
   logical volume solid is complete and we should remember the new
   positioning for it (new path, replica, ...) but ignore calls to
   `AddThis'.  For the latter case we need to be careful with
   boundaries so that we know when we should come out of the sharing
   mode (FIXME: just set CurtailDescent() to avoid processing the
   children!).
  
   This is not simple to achieve -- G4 visualisation architecture only
   gives us limited information of what is going on.  What we have is
   this: fpCurrentPV points to the current physical volume,
   fpCurrentLV to its logical volume, and PreAddThis, AddThis and
   PostAddThis are called for each solid in the logical volume.
   [FIXME: This ignores primitives drawn without solids, as
   G4VisManager::Draw via Begin/Add/EndPrimitives to draw event
   content (tracks and hits, with fReadyForTransients == true?), and
   stuff such as axes and text annotations.  Do we prefer to do these
   independent of G4 -- do we want to support full visualisation or
   only ``useful'' parts of it?]
  
   This method may be called several times in order to build the solid
   representation (without wrapping the calls in any way),
   unfortunately we cannot rely on recognising the last call to
   PostAddThis; this can happen for example with boolean solids.
   Replication has a similar issue: we are not being told when the
   logical volume of one replica ends and the next one begins; all we
   know is that we are still within the same physical/logical volume
   pair.  On the other hand, a logical volume may be repeated several
   times (via several physical volume paths).
  
   To deal with the above, we keep track of changes to the current
   physical volume.  We know a physical volume's relationship to its
   mother is unique: it is a child of any logical mother only once (it
   may appear several times during the processing as the logical
   mother volume may be repeated several times).  We also know that
   the solid of the logical mother is described fully before any of
   its daughters are described.	 Therefore, we compare the previous
   fpCurrentPV with the current one.  If they are different, the
   previous solid has become complete and we've gone somewhere else in
   the tree; if we then re-encounter the logical volume, we should
   just reuse the old definition with a different transformation (and
   tell the processing to ignore the rest of the tree).	 If they are
   the same, we are encountering one of the following cases: 1) we are
   being fed the solids of a composite solid and get first the
   constituents and then whole complex solid, and `AddThis' should
   append to the current representation; 2) we are being fed a
   replicated physical volume whose logical volume has no daughters,
   and we should reuse the existing solid representation for the
   logical volume (the solid may follow the previous case!); 3) we are
   being fed a parametrised physical volume whose logical volume has
   no daughters, but the solid changes from one replica to another
   (and may follow the first case!).  (NB: Not all combinations of
   parametrised volumes, solids and daughters are currently allowed,
   however the last case above already presents us one of the more
   difficult situations.)
  
   Note that a replicated and/or parametrised physical volume can
   point to a logical volume with daughters.  In that case the logical
   volume will be described in full for each replica, so we will see
   the current physical volume change; that is ok, we will
   automatically know to reuse the representations with new
   transformations.  (FIXME: For a parametrised physical volume we
   assume that the solids are not changing if the logical volume has
   daughters.  G4 does not seem to support parametrisations with
   daughters and varying solids anyway.)
  
   The primary problem we tackle is the building of scene graph once
   we've detected which physical volume owns the nodes.	 Here we rely
   on the knowledge that the mother is always processed before its
   daughters.  There is however a twist: the mother may be invisible
   and the daughters (perhaps several levels down) visible -- and we
   will not get invoked for the intermediate visible mothers.
  
   If `fpCurrentPV' has changed, we discover exactly where it should
   be in the scene graph; we assume the client wants results organised
   by the volume tree.	We also know that {FIXME].
  
   For now, we do nothing about replicated/parametrised physical
   volumes -- we do not have enough information to reuse the solid
   representations.  We simply lump all the replicas together as the
   representation of the physical volume.
  
  
   [FIXME: this breaks with voxelisation?] [FIXME: what about
   end-of-event models (hits, trajectories) and axes?
  
   [FIXME: currently (G4.3.2) visualises physical daughters that have
   physical mothers wrong: it always renders them.  If corrected,
   would it change the logic here?]
  
   (FIXME: do we detect the stacking order correctly?  I.e. when to
   start and *stop* ignoring AddThis, once we've decided that we are
   ignoring a replica/already represented volume?)


   ------------------------------------------------------------
   ASCII art diagram for a scenario that matters (sharing):
  
   | LV 1 | -> / S 1 /
      |
      +- ( PV 1.1 ) --+> | LV 2 | -> / S 2 /
      +- ( PV 1.2 ) -'	     |			
			     +- ( PV 2.1 ) -> | LV 3 | -> / S 3 /
  
   Processing order (note the indentantation is not explicit in
   the calls we get in any way!):
      < PV 0.1 / LV 1 / S1 >
	< PV 1.1 / LV 2 / S2 >
	  < PV 2.1 / LV 3 / S3 >
	< PV 1.2 / LV 2 / S2 >
	  < PV 2.1 / LV 3 / S3 >
  
   DIAGNOSIS: *detectable*; note the change of PV and close the
   current representation when that happens.
  

   ------------------------------------------------------------
   ASCII art diagram for a scenario that matters (csg replication):
  
   | LV 1 | -> / S 1 /
      |
      +- ( PV 1.1 ) -(2x)-> | LV 2 | -> / S 2 / +-> / S 2.1 /
						+-> / S 2.2 /
   Processing order:
      < PV 0.1 / LV 1 / S1 >
	< PV 1.1 / LV 2 / S2.1 >
	< PV 1.1 / LV 2 / S2.2 >
	< PV 1.1 / LV 2 / S2 >
	< PV 1.1 / LV 2 / S2.1 >
	< PV 1.1 / LV 2 / S2.2 >
	< PV 1.1 / LV 2 / S2 >
  
   DIAGNOSIS: *detectable*; note the call with the same solid as
   pointed to by LV (S2) to close the current replica rep.
   (FIXME: fragile -- what if the code changes!?)
  

   ------------------------------------------------------------
   ASCII art diagram for a scenario that matters (parametrisation):
  
   | LV 1 | -> / S 1 /
      |
      +- ( PV 1.1 ) -(2x)-> | LV 2 | -> / S 2 /
  
   Processing order (Sn, Sm CSG solids generated by parametrisation):
      < PV 0.1 / LV 1 / S1 >
	< PV 1.1 / LV 2 / Sn.1 >
	< PV 1.1 / LV 2 / Sn.2 >
	< PV 1.1 / LV 2 / Sn >
	< PV 1.1 / LV 2 / Sm.1 >
	< PV 1.1 / LV 2 / Sm.2 >
	< PV 1.1 / LV 2 / Sm >
  
   DIAGNOSIS: *undetectable*; merge all replicas together as the
   representation of the PV (if Sn == Sm == S2, this reduces to
   the previous case and is detectable).
  
   ------------------------------------------------------------
   FIXME: Can we prevent (or not support) the use of
   G4LogicalVolumeModel that causes the solid split?  */

// G4VPhysicalVolumeModel and G4VSceneHandler maintain two useful pointers:
// `fpCurrentPV' and `fpCurrentLV' (and `fpCurrentDepth'): the current
// physical and logical volume, respectively.  We use these to map the
// drawn primitives to the original volumes, and from there, the twig
// representables.  Once the view has been drawn, clients can query
// the map we've built.	 (FIXME: Also use reuse replicated solids.)

// FIXME: Abuse G4VSceneHandler to grab solid representations.

// Note on the appearance and placement handling of primitives:
//
// Primitives may both be created from shape simplification and added
// on their own right.  We ignore primitive appearance and assume the
// caller will invoke AddMaterial() and AddStyle() as appropriate --
// if the primitive is a result of shape simplification, this will
// have happened; if the primitive is an object of its own, then the
// caller must arrange for the suitable calls.  On the contrary, we do
// translate to the primitive position: we assume the caller has made
// a compatible invocation to AddTransform() (= no call for globally
// positioned primitives, and when one has already been added for a
// shape, then its primitives don't get an extra one).

class VIS_G4_CORE_API VisG4VisSceneHandler : public G4VGraphicsScene
{
public:
    typedef G4ViewParameters::RepStyle		RepStyle;
    typedef G4ViewParameters::DrawingStyle	DrawStyle;

    // implicit copy constructor
    // implicit destructor
    // implicit assignment operator

    static VisG4VisSceneHandler *instance (void);

    // Various settings
    virtual void	defineFallback	(RepStyle style);
    virtual void	defineBounds	(G4VisExtent bbox);
    virtual void	defineNoOfSides	(int sides);

    // Cache management
    virtual void	enableCache	(bool enabled);
    virtual void	clearCache	(void);

    // Delimiting objects.  Wrap various add*() and Add*() calls inside these
    virtual void	beginObject	(SoSeparator *node,const void *object);
    virtual void	endObject	(void);

    // Drawing attributes handling
    virtual void	addStyle	(const G4VisAttributes *from);
    virtual void	addStyle	(DrawStyle style);
    virtual void	addMaterial	(SoMaterial *mat);
    virtual void	addTransform	(const G4Transform3D &by);

    // Solid handling
    virtual void	AddSolid	(const G4Box	   &solid);
    virtual void	AddSolid	(const G4Cons	   &solid);
    virtual void	AddSolid	(const G4Tubs	   &solid);
    virtual void	AddSolid       	(const G4Trd	   &solid);
    virtual void	AddSolid       	(const G4Trap	   &solid);
    virtual void	AddSolid       	(const G4Sphere	   &solid);
    virtual void	AddSolid       	(const G4Para	   &solid);
    virtual void	AddSolid       	(const G4Torus	   &solid);
    virtual void	AddSolid       	(const G4Polycone  &solid);
    virtual void	AddSolid       	(const G4Polyhedra &solid);
    virtual void	AddSolid       	(const G4VSolid	   &solid);
    virtual void        AddCompound     (const G4VTrajectory &trajectory);
    virtual void        AddCompound     (const G4VHit      &hit);
    virtual void	AddCompound	(const G4THitsMap<double> &hitsMap);

    virtual void 	BeginPrimitives2D (const G4Transform3D& objectTransformation = G4Transform3D());
    
    // Solid expansion and primitives
    virtual void	AddPrimitive	(const G4Polyline   &object);
    virtual void        AddPrimitive    (const G4Scale      &object);
    virtual void	AddPrimitive	(const G4Text	    &object);
    virtual void	AddPrimitive	(const G4Circle	    &object);
    virtual void	AddPrimitive	(const G4Square	    &object);
    virtual void	AddPrimitive	(const G4Polymarker &object);
    virtual void	AddPrimitive	(const G4Polyhedron &object);
    virtual void	AddPrimitive	(const G4NURBS	    &object);

private:
    typedef std::map<const void *, SoGroup *>	NodeCache;
    typedef std::map<const G4Material *, SoMaterial *>	MaterialCache;
    enum MarkerSizeType { world, screen };

    VisG4VisSceneHandler (void);
    virtual ~VisG4VisSceneHandler (void) {};
    
    void		addTranslation	(SoSeparator *to, const G4Point3D &center);
    void		addTranslation	(SoSeparator *to, SbVec3f center);

    void		RequestPrimitives (const G4VSolid &solid);
    double		GetMarkerSize	(const G4VMarker &mark);
    double		GetMarkerSize   (const G4VMarker &mark,
					 MarkerSizeType &type);
    SoMarkerSet *	makeScreenMarker (G4Polymarker::MarkerType type,
					  G4VMarker::FillStyle fill,
					  double size);
    SoShape *		makeWorldMarker  (G4Polymarker::MarkerType type,
					  double size);
    
    DrawStyle		GetDrawingStyle (const G4Visible &);
    DrawStyle		GetDrawingStyle (const G4VisAttributes *);
    // G4double GetMarkerDiameter (== Size), ...Radius (== Size/2)

    // G4Colour GetColour (const G4Visible &);
    // G4Colour GetTextColour (const G4Text &);

    void		cacheBegin (const void *object);
    bool		cached (void);
    void		cache (SoNode *node);
    void		cacheEnd (void);

    SoSeparator		*m_node;

    NodeCache		m_shapeCache;
    bool		m_caching;
    bool		m_cached;
    SoGroup		*m_cacheNodes;
    int			m_cacheIndex;

    SoResetTransform	*m_reset;
    SoDrawStyle		*m_lines;
    SoDrawStyle		*m_solid;
    MaterialCache	m_materialCache;

    RepStyle		m_fallback;
    G4VisExtent		m_bbox;
    int			m_sides;
    double		m_fontSize;

    // Unsupported but required to be implemented to keep G4 happy.
    // Never use these, use BeginObject() end EndObject() instead.
    // Calling these will cause immediate termination.
    virtual void	BeginPrimitives (const G4Transform3D &transform);
    virtual void	PreAddSolid	(const G4Transform3D &transform,
					 const G4VisAttributes &attributes);
    virtual void	PostAddSolid	(void);
    virtual void	EndPrimitives   (void);
    virtual void BeginPrimitives2D () {}
    virtual void EndPrimitives2D (){}
};

//<<<<<< INLINE PUBLIC FUNCTIONS					>>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS					>>>>>>

#endif // VIS_G4_CORE_VIS_G4_VIS_SCENE_HANDLER_H
