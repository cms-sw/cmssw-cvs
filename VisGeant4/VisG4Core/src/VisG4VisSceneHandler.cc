//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Core/interface/VisG4VisSceneHandler.h"
#include "VisGeant4/VisG4Core/interface/VisG4VisTransform3D.h"

#include "Iguana/Inventor/interface/IgSoCube.h"
#include "Iguana/Inventor/interface/IgSoG4Box.h"
#include "Iguana/Inventor/interface/IgSoRotSolid.h"
#include "Iguana/Inventor/interface/IgSoG4Trd.h"
#include "Iguana/Inventor/interface/IgSoG4Trap.h"

#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoNurbsSurface.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoTransform.h>

#include <G4ThreeVector.hh>
#include <G4Point3D.hh>
#include <G4Normal3D.hh>
#include <G4Transform3D.hh>
#include <G4Polyline.hh>
#include <G4Scale.hh>
#include <G4Text.hh>
#include <G4Circle.hh>
#include <G4Square.hh>
#include <G4Polyhedron.hh>
#include <G4NURBS.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Cons.hh>
#include <G4Trap.hh>
#include <G4Trd.hh>
#include <G4Sphere.hh>
#include <G4Para.hh>
#include <G4Torus.hh>
#include <G4Polycone.hh>
#include <G4Polyhedra.hh>
#include <G4VisAttributes.hh>

#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

static inline bool operator== (const G4Colour &x, const G4Colour &y)
{
    float xrgba [4] = { x.GetRed (), x.GetGreen (), x.GetBlue (), x.GetAlpha () };
    float yrgba [4] = { y.GetRed (), y.GetGreen (), y.GetBlue (), y.GetAlpha () };
    for (unsigned i = 0; i < 4; ++i)
	if (xrgba [i] != yrgba [i])
	    return false;

    return true;
}

static inline bool operator< (const G4Colour &x, const G4Colour &y)
{
    float xrgba [4] = { x.GetRed (), x.GetGreen (), x.GetBlue (), x.GetAlpha () };
    float yrgba [4] = { y.GetRed (), y.GetGreen (), y.GetBlue (), y.GetAlpha () };
    for (unsigned i = 0; i < 4; ++i)
	if (xrgba [i] < yrgba [i])
	    return true;
	else if (xrgba [i] > yrgba [i])
	    return false;

    return false;
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4VisSceneHandler *
VisG4VisSceneHandler::instance (void)
{
    static VisG4VisSceneHandler *instance = new VisG4VisSceneHandler;
    return instance;
}


VisG4VisSceneHandler::VisG4VisSceneHandler (void)
    : m_node (0),
      m_caching (false),
      m_cached (false),
      m_cacheNodes (0),
      m_cacheIndex (0),
      m_lines (new SoDrawStyle),
      m_solid (new SoDrawStyle),
      m_fallback (G4ViewParameters::polyhedron),
      m_bbox (0, .5),
      m_sides (24),
      m_fontSize (12)
{
    // Initialise a shared scenegraph scaler node
    m_lines->ref ();
    m_solid->ref ();
    m_lines->style = SoDrawStyle::LINES;
    m_solid->style = SoDrawStyle::FILLED;

    // Determine if we can reuse OIV nodes for the volume.  We do not
    // reuse entire rep hierarchies, only solid shape lists, mainly
    // for the following reasons: 1) we do not yet have support for
    // shared reps, 2) each different volume path can have different
    // visualisation preferences, and 4) parametrised replicas might
    // vary solids (?).
    //
    // Note that we never clear our cache.  We assume the nodes
    // created for a particular solid are valid for forever (FIXME?).
    // Note that VisG4VolumeModel invokes this method exactly once.
    // Thus we can get away with single non-stacked caching state.
    //
    // We first check if we can share anything at all: in a replica we
    // can't.  Secondly, we only cache the solid shape nodes, the rest
    // is not cached (transformation matrices, material settings,
    // multiple broken down solid structures) (FIXME).  m_cacheIndex
    // tracks our progress in a complex solid structure; we assume
    // (and ASSERT) that the calls to solid description methods below
    // always appear in the same order for the same cached solid.
    //
    // Caching is on by default.  If $IGUANA_CACHING is defined, it
    // must be `1', `on' or `yes' for caching to be enabled.
    const char *cachingStr = getenv ("IGUANA_CACHING");
    m_caching = (! cachingStr
		 || !strcmp (cachingStr, "1")
		 || !strcmp (cachingStr, "on")
		 || !strcmp (cachingStr, "yes"));
}

void
VisG4VisSceneHandler::defineFallback (RepStyle style)
{ m_fallback = style; }

void
VisG4VisSceneHandler::defineBounds (G4VisExtent bbox)
{ m_bbox = bbox; }

void
VisG4VisSceneHandler::defineNoOfSides (int sides)
{ m_sides = sides; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
VisG4VisSceneHandler::enableCache (bool enabled)
{
    cacheEnd ();
    if (! (m_caching = enabled))
	clearCache ();
}

void
VisG4VisSceneHandler::clearCache (void)
{
    cacheEnd ();
    while (! m_shapeCache.empty ())
    {
	m_shapeCache.begin ()->second->unref ();
	m_shapeCache.erase (m_shapeCache.begin ());
    }
    while (! m_materialCache.empty ())
    {
	m_materialCache.begin ()->second->unref ();
	m_materialCache.erase (m_materialCache.begin ());
    }
}

void
VisG4VisSceneHandler::cacheBegin (const void *object)
{
    cacheEnd ();
    if (m_caching && object)
    {
	ASSERT (object);
	if (! m_shapeCache.count (object))
	{
	    m_shapeCache [object] = m_cacheNodes = new SoGroup;
	    m_cacheNodes->ref ();
	}
	else
	{
	    m_cacheNodes = m_shapeCache [object];
	    m_cached = true;
	}
    }
}

bool
VisG4VisSceneHandler::cached (void)
{
    if (m_cached)
    {
	ASSERT (m_node);
	ASSERT (m_cacheNodes);
	ASSERT (m_cacheIndex < m_cacheNodes->getNumChildren ());
	m_node->addChild (m_cacheNodes->getChild (m_cacheIndex));
	m_cacheIndex++;
	return true;
    }
    else
	return false;
}

void
VisG4VisSceneHandler::cache (SoNode *node)
{
    if (m_cacheNodes)
    {
	m_cacheNodes->addChild (node);
	m_cacheIndex++;
    }
}

void
VisG4VisSceneHandler::cacheEnd (void)
{
    m_cached = false;
    m_cacheNodes = 0;
    m_cacheIndex = 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
VisG4VisSceneHandler::beginObject (SoSeparator *node, const void *object)
{
    // Set up output target.
    ASSERT (! m_node);
    ASSERT (node);
    m_node = node;

    // Setup caching.
    cacheBegin (object);
}

void
VisG4VisSceneHandler::endObject (void)
{
    ASSERT (m_node);
    cacheEnd ();
    m_node = 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
VisG4VisSceneHandler::addStyle (const G4VisAttributes *from)
{
    // FIXME: To be removed when we get real vis prefs service
    ASSERT (from);
    addStyle (GetDrawingStyle (from));
    // Now using addMaterial(G4Material to set the material.
}

void
VisG4VisSceneHandler::addStyle (DrawStyle style)
{
    ASSERT (m_node);

    switch (style)
    {
    case G4ViewParameters::hlr:		// hidden lines removed
    case G4ViewParameters::wireframe:	// wireframe (no lines removed)
	m_node->addChild (m_lines);
	break;

    case G4ViewParameters::hlhsr:	// hidden lines and surfaces removed
    case G4ViewParameters::hsr:		// hidden surfaces removed
    default:
	m_node->addChild (m_solid);
	break;
    }
}

void
VisG4VisSceneHandler::addMaterial (SoMaterial *mat)
{
    ASSERT (mat);    
    ASSERT (m_node);
    
    m_node->addChild (mat);
}

void
VisG4VisSceneHandler::addTransform (const G4Transform3D &by)
{
    ASSERT (m_node);

    // Force uniform downscale to meters.
    SbMatrix scale;
    scale.setScale (1./m);

    SoMatrixTransform *xform = new SoMatrixTransform;
    xform->matrix.setValue (scale * VisG4VisTransform3D (by));
    m_node->addChild (xform);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
VisG4VisSceneHandler::addTranslation (SoSeparator *to, const G4Point3D &center)
{
    ASSERT (to);

    SoTranslation *t = new SoTranslation;
    t->translation.setValue (center.x (), center.y (), center.z ());
    to->addChild (t);
}

void
VisG4VisSceneHandler::addTranslation (SoSeparator *to, SbVec3f center)
{
    ASSERT (to);

    SoTranslation *t = new SoTranslation;
    t->translation = center;
    to->addChild (t);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
VisG4VisSceneHandler::AddSolid (const G4Box &solid)
{
    ASSERT (m_node);
    if (cached ()) return;

    // FIXME: Notice if the cube is "thin," in which case just use two
    // surfaces instead of the six, or use LOD.  This is to optimise
    // for large numbers of very thin boxes frequent in silicon wafers
    // etc.  It is useless to render all the sides always, you have to
    // be very close to see all the sides other than lines.
    IgSoCube *rep = new IgSoCube;
    rep->width     = 2 * solid.GetXHalfLength ();
    rep->length    = 2 * solid.GetYHalfLength ();
    rep->thickness = 2 * solid.GetZHalfLength (); 

    m_node->addChild (rep);
    cache (rep);
}

void
VisG4VisSceneHandler::AddSolid (const G4Cons &solid)
{
    ASSERT (m_node);
    if (cached ()) return;

    IgSoRotSolid *rep = new IgSoRotSolid;
    
    rep->makeCons (solid.GetRmin1 (),
		   solid.GetRmax1 (),
		   solid.GetRmin2 (),
		   solid.GetRmax2 (),
		   solid.GetDz (),
		   solid.GetSPhi (),
		   solid.GetDPhi ());
    
    m_node->addChild (rep);
    cache (rep);
}

void
VisG4VisSceneHandler::AddSolid (const G4Tubs &solid)
{
    ASSERT (m_node);
    if (cached ()) return;

    IgSoRotSolid *rep = new IgSoRotSolid;
    
    rep->makeTubs (solid.GetRMin (), 
		   solid.GetRMax (), 
		   solid.GetDz (),
		   solid.GetSPhi (), 
		   solid.GetDPhi ());
    m_node->addChild (rep);
    cache (rep);
}

void
VisG4VisSceneHandler::AddSolid (const G4Trd &solid)
{
    ASSERT (m_node);
    if (cached ()) return;

    IgSoG4Trd *rep = new IgSoG4Trd;
    rep->dx1 = solid.GetXHalfLength1 ();
    rep->dx2 = solid.GetXHalfLength2 ();
    rep->dy1 = solid.GetYHalfLength1 ();
    rep->dy2 = solid.GetYHalfLength2 ();
    rep->dz  = solid.GetZHalfLength ();
    m_node->addChild (rep);
    cache (rep);
}

void
VisG4VisSceneHandler::AddSolid (const G4Trap &solid)
{
    ASSERT (m_node);
    if (cached ()) return;

    G4ThreeVector	axis = solid.GetSymAxis ();
    IgSoG4Trap		*rep = new IgSoG4Trap;
    rep->phi   = atan2 (axis (kYAxis), axis (kXAxis));
    rep->theta = acos (axis (kZAxis));
    rep->dz    = solid.GetZHalfLength ();
    rep->dy1 	= solid.GetYHalfLength1 ();
    rep->dx1 	= solid.GetXHalfLength1 ();
    rep->dx2 	= solid.GetXHalfLength2 ();
    rep->alp1	= solid.GetTanAlpha1 ();
    rep->dy2 	= solid.GetYHalfLength2 ();
    rep->dx3 	= solid.GetXHalfLength3 ();
    rep->dx4 	= solid.GetXHalfLength4 ();
    rep->alp2	= solid.GetTanAlpha2 ();
    m_node->addChild (rep);
    cache (rep);
}

void
VisG4VisSceneHandler::AddSolid (const G4Sphere &solid)
{ RequestPrimitives (solid); }

void
VisG4VisSceneHandler::AddSolid (const G4Para &solid)
{ RequestPrimitives (solid); }

void
VisG4VisSceneHandler::AddSolid (const G4Torus &solid)
{ RequestPrimitives (solid); }

void
VisG4VisSceneHandler::AddSolid (const G4Polycone &solid)
{ 
    ASSERT (m_node);
    if (cached ()) return;

    int 			nr = solid.GetNumRZCorner();   
    std::vector<SbVec2f> 	polygon (nr);
    
    for (int i = 0; i < nr; ++i)
	polygon[i] = SbVec2f (solid.GetCorner (i).r, solid.GetCorner (i).z);	

    IgSoRotSolid *rep = new IgSoRotSolid;
    rep->phiStart 	= solid.GetStartPhi ();
    rep->phiDelta 	= solid.GetEndPhi () - solid.GetStartPhi ();
    rep->smooth  	= true;
    rep->polygon.setValues (0, nr, &polygon[0]);
    
    m_node->addChild (rep);
    cache (rep);
} 

void
VisG4VisSceneHandler::AddSolid (const G4Polyhedra &solid)
{
    ASSERT (m_node);
    if (cached ()) return;

    int 			nr = solid.GetNumRZCorner();   
    std::vector<SbVec2f> 	polygon (nr);
    
    for (int i = 0; i < nr; ++i)
	polygon[i] = SbVec2f (solid.GetCorner (i).r, solid.GetCorner (i).z); 

    IgSoRotSolid *rep = new IgSoRotSolid;
    rep->phiStart 	= solid.GetStartPhi ();
    (solid.GetEndPhi () == solid.GetStartPhi ()) ? rep->phiDelta = 2 * M_PI :
    rep->phiDelta 	= solid.GetEndPhi () - solid.GetStartPhi ();
    rep->divisions	= solid.GetNumSide ();    
    rep->smooth  	= false;
    rep->polygon.setValues (0, nr, &polygon[0]);
    
    m_node->addChild (rep);
    cache (rep);
}

void
VisG4VisSceneHandler::AddSolid (const G4VSolid &solid)
{
    // Duh, some forms are declared as methods in base class, but
    // never called from the solids...  (FIXME: which else can we
    // handle -- G4VCSGfaceted?)
    if (const G4Polycone *pcon = dynamic_cast<const G4Polycone *> (&solid))
	AddSolid (*pcon);
    else if (const G4Polyhedra *pgon = dynamic_cast<const G4Polyhedra *> (&solid))
	AddSolid (*pgon);
    else
	RequestPrimitives (solid);
}

void
VisG4VisSceneHandler::AddCompound (const G4VTrajectory & /*trajectory*/)
{ // FIXME 
}

void
VisG4VisSceneHandler::AddCompound (const G4VHit & /*hit*/)
{ // FIXME
}

void
VisG4VisSceneHandler::AddCompound (const G4THitsMap<double> & /*hitsMap*/)
{ // FIXME
}

void
VisG4VisSceneHandler::BeginPrimitives2D (const G4Transform3D& /* objectTransformation = G4Transform3D() */)
{ // FIXME
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
VisG4VisSceneHandler::AddPrimitive (const G4Polyline &object)
{
    ASSERT (m_node);
    if (cached ()) return;

    SoSeparator *me = new SoSeparator;
    m_node->addChild (me);

    // Force wireframe (FIXME: useless -- it's a line?).
    me->addChild (m_lines);

    // Add line with the points
    std::vector<SbVec3f> points (object.size (), SbVec3f ());
    for (unsigned i = 0; i < object.size (); ++i)
	points [i].setValue (object [i].x(), object [i].y(), object [i].z());

    SoLineSet		*lines = new SoLineSet;
    SoVertexProperty	*vtx = new SoVertexProperty;
    vtx->vertex.setValues (0, object.size (), &points[0]);
    lines->numVertices = object.size ();
    lines->vertexProperty = vtx;
    me->addChild (lines);
    cache (me);
}

void
VisG4VisSceneHandler::AddPrimitive (const G4Scale &object)
{
    // FIXME: This *really* should not be here.  Axes and scales are a
    // feature that should be done properly and only once in IGUANA,
    // not some crappy almost-good-for-something stuff like here!

    // Useful constants.
    double	length		= object.GetLength ();
    double	halfLength	= length / 2;
    double	tickLength	= length / 20;
    double	piBy2		= M_PI / 2;

    // Get size of the world.
    double	 xmin		= m_bbox.GetXmin ();
    double	 xmax		= m_bbox.GetXmax ();
    double	 ymin		= m_bbox.GetYmin ();
    double	 ymax		= m_bbox.GetYmax ();
    double	 zmin		= m_bbox.GetZmin ();
    double	 zmax		= m_bbox.GetZmax ();

    // Create (empty) polylines with present vis preferences.  Add
    // points to them to represent an scale parallel to the x-axis
    // centred on the origin.
    G4Polyline	scaleLine, tick11, tick12, tick21, tick22;
    G4Point3D	r1    (-halfLength, 0, 0);
    G4Point3D	r2    ( halfLength, 0, 0);
    G4Point3D	ticky (0, tickLength, 0);
    G4Point3D	tickz (0, 0, tickLength);

    scaleLine.push_back (r1);
    scaleLine.push_back (r2);
    tick11.push_back (r1 + ticky);
    tick11.push_back (r1 - ticky);
    tick12.push_back (r1 + tickz);
    tick12.push_back (r1 - tickz);
    tick21.push_back (r2 + ticky);
    tick21.push_back (r2 - ticky);
    tick22.push_back (r2 + tickz);
    tick22.push_back (r2 - tickz);

    // Annotation text.
    G4Text text (object.GetAnnotation(), G4Point3D (0., tickLength, 0.));
    text.SetScreenSize (24.);

    // Transformation.
    G4Transform3D rotation;
    switch (object.GetDirection ()) {
    case G4Scale::x: break;
    case G4Scale::y: rotation = G4RotateZ3D (piBy2); break;
    case G4Scale::z: rotation = G4RotateY3D (piBy2); break;
    }

    G4double	sxmid (object.GetXmid ());
    G4double	symid (object.GetYmid ());
    G4double	szmid (object.GetZmid ());

    if (object.GetAutoPlacing ()) {
	// Margins for comfortably scaled to be inside view volume.
	double	lomargin = 0.01;
	double	himargin = 1 - lomargin;

	sxmid = xmin + himargin * (xmax - xmin);
	symid = ymin + lomargin * (ymax - ymin);
	szmid = zmin + himargin * (zmax - zmin);
	switch (object.GetDirection ()) {
	case G4Scale::x: sxmid -= halfLength; break;
	case G4Scale::y: symid += halfLength; break;
	case G4Scale::z: szmid -= halfLength; break;
	}
    }

    // Add the transform and the primitives we've created
    SoSeparator *node = m_node;
    m_node = new SoSeparator;
    addTransform (G4Transform3D (G4Translate3D(sxmid,symid,szmid) * rotation));
    AddPrimitive (scaleLine);
    AddPrimitive (tick11);
    AddPrimitive (tick12);
    AddPrimitive (tick21);
    AddPrimitive (tick22);
    AddPrimitive (text);
    m_node = node;
}

void
VisG4VisSceneHandler::AddPrimitive (const G4Text &object)
{
    // FIXME: SoText2 supports only screen size.  It is not obvious
    // how useful the heroic attempts of G4 to compute screen size
    // from world size and current zoom settings are given that it all
    // varies very dynamically according to the current camera.  We
    // sure don't want to link the two by recomputing the text size on
    // the fly if we can avoid it.  OTOH, see the comment on the other
    // marker objects as well -- we do manage to do something sensible
    // there.  So perhaps we need to go into the trouble of getting it
    // right here with massive amount of GL magic?  If so, make it a
    // generic Inventor service and just use it here.
    ASSERT (m_node);
    if (cached ()) return;

    SoSeparator *me = new SoSeparator;
    m_node->addChild (me);

    // Translate to object position
    addTranslation (me, object.GetPosition ());
    // addMaterial (me, FIXME: is text colour special?)

    // FIXME: X/Y offset is in screen coordinates -- not supported;
    // needs raw GL support to be able to do that.

    // Font.  FIXME: fallback on global marker text size settings
    double size = object.GetScreenSize ();
    SoFont *font = new SoFont;
    font->size = size ? size : m_fontSize;
    me->addChild (font);

    // Text (FIXME: break string into separate values at newlines)
    SoText2 *text = new SoText2;
    text->string.setValue (object.GetText ());
    switch (object.GetLayout ()) {
    case G4Text::left:   text->justification = SoText2::LEFT;   break;
    case G4Text::centre: text->justification = SoText2::CENTER; break;
    case G4Text::right:  text->justification = SoText2::RIGHT;  break;
    }
    me->addChild (text);
    cache (me);
}

void
VisG4VisSceneHandler::AddPrimitive (const G4Circle &object)
{
    // FIXME: Steal more marker code into special GL shape from G4
    // OpenGL viewer and SoMarkerSet implementation.
    //
    // FIXME: Support an option to force markers on the top layer,
    // i.e. glDisable(GL_DEPTH_TEST). (IsMarkerNotHidden ()).
    ASSERT (m_node);
    if (cached ()) return;

    SoSeparator *me = new SoSeparator;
    m_node->addChild (me);

    MarkerSizeType	 type;
    G4VMarker::FillStyle fill = object.GetFillStyle ();
    G4ThreeVector	 pos (object.GetPosition ());
    double		 size = GetMarkerSize (object, type);
    SoMarkerSet		 *mset;
    SoVertexProperty	 *vtx;

    switch (type)
    {
    case screen:
	// The marker has screen size.  Use SoMarkerSet with just one
	// point -- if you have many, use G4Polymarker or IgSoHitsKit,
	// this method to draw loads of circles will kill performance.
	mset = makeScreenMarker (G4Polymarker::circles, fill, size);
	vtx = new SoVertexProperty;
	vtx->vertex.setValue (SbVec3f (pos.x (), pos.y (), pos.z ()));
	mset->vertexProperty = vtx;
	mset->numPoints = 1;
	me->addChild (mset);
	break;

    case world:
	// The marker has world size: draw a sphere.  This is wrong,
	// the marker should be a circle always facing the viewer.
	// Filled ones the spheres will do, though inefficiently.
	// Anything else is a failure (actually "hashed" probably
	// looks about right due to the triangulation :-).  We should
	// use more powerful marker nodes that work at raw GL level.
	// Perhaps we can reuse code from the G4 OpenGL viewer.
	// (FIXME: Disable lighting?)
	if (fill != G4VMarker::filled)
	    me->addChild (m_lines);
	addTranslation (me, pos);
	me->addChild (makeWorldMarker (G4Polymarker::circles, size));
	break;
    }
    cache (me);
}

void
VisG4VisSceneHandler::AddPrimitive (const G4Square &object)
{
    // FIXME: All this marker code should be replaced by a custom
    // marker node a la SoMarkerSet but with our own logic that we
    // could more or less steal from G4 OpenGL viewer.
    //
    // FIXME: Support an option to force markers on the top layer,
    // i.e. glDisable(GL_DEPTH_TEST). (IsMarkerNotHidden ()).
    ASSERT (m_node);
    if (cached ()) return;

    SoSeparator *me = new SoSeparator;
    m_node->addChild (me);

    MarkerSizeType	 type;
    G4VMarker::FillStyle fill = object.GetFillStyle ();
    G4ThreeVector	 pos (object.GetPosition ());
    double		 size = GetMarkerSize (object, type);
    SoMarkerSet		 *mset;
    SoVertexProperty	 *vtx;

    switch (type)
    {
    case screen:
	// The marker has screen size.  Use SoMarkerSet with just one
	// point -- if you have many, use G4Polymarker or IgSoHitsKit,
	// this method to draw loads of circles will kill performance.
	mset = makeScreenMarker (G4Polymarker::squares, fill, size);
	vtx = new SoVertexProperty;
	vtx->vertex.setValue (SbVec3f (pos.x (), pos.y (), pos.z ()));
	mset->vertexProperty = vtx;
	mset->numPoints = 1;
	me->addChild (mset);
	break;

    case world:
	// The marker has world size: draw a cube.  This is wrong, the
	// marker should be a circle always facing the viewer.  Filled
	// ones the cubes will do, though inefficiently.  Anything
	// else is a failure (actually "hashed" probably looks about
	// right due to the triangulation :-).  We should use more
	// powerful marker nodes that work at raw GL level.  Perhaps
	// we can reuse code from the G4 OpenGL viewer.  (FIXME:
	// Disable lighting?)
	if (fill != G4VMarker::filled)
	    me->addChild (m_lines);
	addTranslation (me, pos);
	me->addChild (makeWorldMarker (G4Polymarker::squares, size));
	break;
    }
    cache (me);
}

void
VisG4VisSceneHandler::AddPrimitive (const G4Polyhedron &object)
{
    ASSERT (m_node);
    if (! object.GetNoFacets () || cached ())
	return;

    // Collect the faces
    G4bool			notLastFace;
    std::vector<SbVec3f>	vertices;
    std::vector<SbVec3f>	normals;
    std::vector<G4int>		nfaces;
    G4Normal3D			normal;
    do {
	notLastFace = object.GetNextUnitNormal (normal);
	normals.push_back (SbVec3f (normal.x (), normal.y (), normal.z ()));

	G4int		faces = 0;
	G4bool		notLastEdge;
	G4Point3D	vertex;
	G4int		edgeFlag = 1;

	do {
	    notLastEdge = object.GetNextVertex (vertex, edgeFlag);
	    vertices.push_back (SbVec3f (vertex.x(), vertex.y(), vertex.z()));
	    faces++;
	} while (notLastEdge);

	nfaces.push_back (faces);
    } while (notLastFace);

    // Add owner
    SoSeparator *me = new SoSeparator;
    m_node->addChild (me);

    // Store faces with normals and coordinates
    SoFaceSet        *faces = new SoFaceSet;
    SoVertexProperty *vtx = new SoVertexProperty;

    vtx->normalBinding = SoNormalBinding::PER_FACE;
    vtx->normal.setValues (0, normals.size (), &normals[0]);
    vtx->vertex.setValues (0, vertices.size (), &vertices[0]);
    faces->numVertices.setValues (0, nfaces.size (), &nfaces[0]);
    faces->vertexProperty = vtx;
    me->addChild (faces);
    cache (me);
}

void
VisG4VisSceneHandler::AddPrimitive (const G4NURBS &nurb)
{
    // FIXME: Does G4 support NURB trimming?
    ASSERT (m_node);
    ASSERT (G4NURBS::NofC == 4);
    if (cached ()) return;

    // Knots and control points must be G4floats.
    std::vector<G4float> uknots (nurb.GetnbrKnots(G4NURBS::U), 0.);
    std::vector<G4float> vknots (nurb.GetnbrKnots(G4NURBS::V), 0.);
    std::vector<G4float> ctrls (nurb.GettotalnbrCtrlPts () * G4NURBS::NofC,0.);

    G4NURBS::KnotsIterator u (nurb, G4NURBS::U);
    for (int i = 0; u.pick (&uknots [i]); i++)
	;

    G4NURBS::KnotsIterator v (nurb, G4NURBS::V);
    for (int i = 0; v.pick (&vknots [i]); i++)
	;
    
    G4NURBS::CtrlPtsCoordsIterator c (nurb);
    for (int i = 0; c.pick (&ctrls [i]); i++)
	;
  
    // Add owner
    SoSeparator *me = new SoSeparator;
    m_node->addChild (me);

    // Set up NURBS
    G4int n = nurb.GettotalnbrCtrlPts ();
    std::vector<SbVec4f> points (n, SbVec4f ());
    for (G4int i = 0, j = 0; i < n ; ++i, j += 4)
	points [i].setValue (ctrls[j+0], ctrls[j+1], ctrls[j+2], ctrls[j+3]);

    SoComplexity *complexity = new SoComplexity;
    complexity->value = 0.6;
    me->addChild (complexity);

    SoCoordinate4 *controls = new SoCoordinate4;
    controls->point.setValues (0, n, &points[0]);
    me->addChild (controls);

    SoNurbsSurface *surface = new SoNurbsSurface;
    surface->numUControlPoints = uknots.size ();
    surface->numVControlPoints = vknots.size ();
    surface->uKnotVector.setValues (0,uknots.size (), &uknots [0]);
    surface->vKnotVector.setValues (0,vknots.size (), &vknots [0]);
    me->addChild (surface);
    cache (me);
}

void
VisG4VisSceneHandler::AddPrimitive (const G4Polymarker &object)
{
    ASSERT (m_node);
    if (cached ()) return;

    SoSeparator *me = new SoSeparator;
    m_node->addChild (me);

    // Translate to object position
    addTranslation (me, object.GetPosition ());

    // Compute points
    std::vector<SbVec3f> points (object.size (), SbVec3f ());
    for (unsigned i = 0; i < object.size (); ++i)
	points [i].setValue (object [i].x(),
			     object [i].y(),
			     object [i].z());

    // Determine marker type and for circles and squares, subtype too.
    MarkerSizeType	 type;
    G4VMarker::FillStyle fill = object.GetFillStyle ();
    double		 size = GetMarkerSize (object, type);
    SoPointSet		 *pset;
    SoMarkerSet		 *mset;
    SoVertexProperty	 *vtx;
    SoShape		 *shape;
    SbVec3f		 last (0, 0, 0);

    // Render
    switch (object.GetMarkerType ())
    {
    default:
    case G4Polymarker::dots:
	// Point set
	vtx = new SoVertexProperty;
	vtx->vertex.setValues (0, object.size (), &points[0]);
	pset = new SoPointSet;
	pset->numPoints = object.size ();
	pset->vertexProperty = vtx;
	me->addChild (pset);
	break;

    case G4Polymarker::circles:
    case G4Polymarker::squares:
	// A circle or square at each marker position.
	switch (type)
	{
	case screen:
	    vtx = new SoVertexProperty;
	    vtx->vertex.setValues (0, object.size (), &points[0]);
	    mset = makeScreenMarker (object.GetMarkerType (), fill, size);
	    mset->numPoints = object.size ();
	    mset->vertexProperty = vtx;
	    me->addChild (mset);
	    break;

	case world:
	    if (fill != G4VMarker::filled)
		me->addChild (m_lines);
	    shape = makeWorldMarker (object.GetMarkerType (), size);
	    for (unsigned i = 0; i < object.size (); last = points [i++])
	    {
		addTranslation (me, points [i] - last);
		me->addChild (shape);
	    }
	    break;
	}
	break;
    }
    cache (me);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
SoMarkerSet *
VisG4VisSceneHandler::makeScreenMarker (G4Polymarker::MarkerType type,
				       G4VMarker::FillStyle fill,
				       double size)
{
    // FIXME: If the marker size is not in range, create our own
    // custom bitmap of the required type, and then use that for the
    // markers.  (Cf. G4 OpenGL viewer for related code!)
    //
    // FIXME: If drawing screen-size filled markers and point size not
    // in range for SoMarkerSet, fall back on bare SoPointSet with
    // appropriate point size + glDisable(GL_POINT_SMOOTH)?
    SoMarkerSet *mset = new SoMarkerSet;
    switch (type)
    {
    case G4Polymarker::circles:
	switch (fill)
	{
	case G4VMarker::noFill:
	    mset->markerIndex
		= size <= 2.5 ? SoMarkerSet::CIRCLE_LINE_5_5
		: size <= 3.5 ? SoMarkerSet::CIRCLE_LINE_7_7
		: SoMarkerSet::CIRCLE_LINE_9_9;
	    break;

	case G4VMarker::hashed:
	    mset->markerIndex
		= size <= 2.5 ? SoMarkerSet::STAR_5_5
		: size <= 3.5 ? SoMarkerSet::STAR_7_7
		: SoMarkerSet::STAR_9_9;
	    break;

	case G4VMarker::filled:
	    mset->markerIndex
		= size <= 2.5 ? SoMarkerSet::CIRCLE_FILLED_5_5
		: size <= 3.5 ? SoMarkerSet::CIRCLE_FILLED_7_7
		: SoMarkerSet::CIRCLE_FILLED_9_9;
	    break;
	}
	return mset;

    case G4Polymarker::squares:
	switch (fill)
	{
	case G4VMarker::noFill:
	    mset->markerIndex
		= size <= 2.5 ? SoMarkerSet::SQUARE_LINE_5_5
		: size <= 3.5 ? SoMarkerSet::SQUARE_LINE_7_7
		: SoMarkerSet::SQUARE_LINE_9_9;
	    break;

	case G4VMarker::hashed:
	    mset->markerIndex
		= size <= 2.5 ? SoMarkerSet::CROSS_5_5
		: size <= 3.5 ? SoMarkerSet::CROSS_7_7
		: SoMarkerSet::CROSS_9_9;
	    break;

	case G4VMarker::filled:
	    mset->markerIndex
		= size <= 2.5 ? SoMarkerSet::SQUARE_FILLED_5_5
		: size <= 3.5 ? SoMarkerSet::SQUARE_FILLED_7_7
		: SoMarkerSet::SQUARE_FILLED_9_9;
	    break;
	}
	return mset;

    default:
	ASSERT (false);
	return 0;
    }
}

SoShape *
VisG4VisSceneHandler::makeWorldMarker (G4Polymarker::MarkerType type, double size)
{
    SoSphere *sphere;
    SoCube   *cube;

    switch (type)
    {
    case G4Polymarker::circles:
	sphere = new SoSphere;
	sphere->radius = size;
	return sphere;

    case G4Polymarker::squares:
	cube = new SoCube;
	cube->width = size * 2;
	cube->height = size * 2;
	cube->depth = size * 2;
	return cube;

    default:
	ASSERT (false);
	return 0;
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
double
VisG4VisSceneHandler::GetMarkerSize (const G4VMarker &marker,
				    MarkerSizeType &type)
{
    // FIXME: fall back on default marker
    double size = marker.GetWorldSize ();

    if (size)
	// Draw in world coordinates.
	type = world;
    else
    {
	// Draw in screen coordinates.
	size = marker.GetScreenSize ();
	type = screen;
    }

    return (size <= 0. ? 1. : size);
    // FIXME: * fpViewer -> GetViewParameters().GetGlobalMarkerScale();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
VisG4VisSceneHandler::RequestPrimitives (const G4VSolid &solid)
{
    if (m_fallback == G4ViewParameters::nurbs)
	if (G4NURBS *nurb = solid.CreateNURBS ())
	{
	    AddPrimitive (*nurb);
	    delete nurb;
	    return;
	}

    // Failed or want polyhedron, keep going (FIXME: barf if fails).
    G4Polyhedron::SetNumberOfRotationSteps (m_sides);
    if (G4Polyhedron *polyh = solid.CreatePolyhedron ())
    {
	AddPrimitive (*polyh);
	delete polyh;
    }
    G4Polyhedron::ResetNumberOfRotationSteps ();
}

G4ViewParameters::DrawingStyle
VisG4VisSceneHandler::GetDrawingStyle (const G4VisAttributes *attrs)
{
    G4ViewParameters::DrawingStyle style = G4ViewParameters::hlhsr;
    if (attrs->IsForceDrawingStyle ())
	// This is complicated because if hidden line removal has been
	// requested we wish to preserve this.
	switch (attrs->GetForcedDrawingStyle ())
	{
	case G4VisAttributes::solid:
	    switch (style)
	    {
	    case G4ViewParameters::hlr:
		style = G4ViewParameters::hlhsr;
		break;
	    case G4ViewParameters::wireframe:
		style = G4ViewParameters::hsr;
		break;
	    default:
		break;
	    } 
	    break;

	default:
	case G4VisAttributes::wireframe:
	    switch (style)
	    {
	    case G4ViewParameters::hlhsr:
		style = G4ViewParameters::hlr;
		break;
	    case G4ViewParameters::hsr:
		style = G4ViewParameters::wireframe;
		break;
	    default:
		break;
	    } 
	    break;
	}

    return style;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
VisG4VisSceneHandler::PreAddSolid (const G4Transform3D &,const G4VisAttributes &)
{ ASSERT (false); }

void
VisG4VisSceneHandler::PostAddSolid (void)
{ ASSERT (false); }

void
VisG4VisSceneHandler::BeginPrimitives (const G4Transform3D &)
{ ASSERT (false); }

void
VisG4VisSceneHandler::EndPrimitives (void)
{ ASSERT (false); }
