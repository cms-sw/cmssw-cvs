#ifndef VIS_G4_CORE_VIS_G4_VIS_MANAGER_H
# define VIS_G4_CORE_VIS_G4_VIS_MANAGER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Core/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <classlib/utils/Callback.h>
# include <G4VVisManager.hh>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTwig;
class VisG4VisMonitor;
class VisG4VisQtTimer;
class VisG4VisScene;
class VisG4VisSystem;
class VisG4VisSceneHandler;
class VisG4VisViewer;
class G4VSceneHandler;
class G4Scene;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** IGUANA custom Geant4 visualisation manager.  We overtake the whole
    visualisation system in Geant4 and customise it to work the way we
    want.  At the moment this is merely nominal compliance, we do not
    in practise try to be compliant with the Geant4 visualisation
    logic.  We only do enough to keep Geant4 happy such that we can
    make use of the useful functionality in the #G4VVisSceneHandler
    base class.  No other graphics systems are registered to Geant4 as
    we are by and large incompatible with them, and on the other hand
    provide most of the functionality in a different way.

    This class also channels geometry change notifications to any
    interest party.  (FIXME: other state notifications?) */
class VIS_G4_CORE_API VisG4VisManager : public IgStateElement, public G4VVisManager
{
    IG_DECLARE_STATE_ELEMENT (VisG4VisManager);
public:
    VisG4VisManager (IgState *state);
    ~VisG4VisManager (void);

    void		notify (lat::Callback onChange);

protected:
    virtual void	GeometryHasChanged (void);

private:
    virtual void	Draw (const G4Circle &, const G4Transform3D &);
    virtual void	Draw (const G4NURBS &, const G4Transform3D &);
    virtual void	Draw (const G4Polyhedron &, const G4Transform3D &);
    virtual void	Draw (const G4Polyline &, const G4Transform3D &);
    virtual void	Draw (const G4Polymarker &, const G4Transform3D &);
    virtual void	Draw (const G4Scale &, const G4Transform3D &);
    virtual void	Draw (const G4Square &, const G4Transform3D &);
    virtual void	Draw (const G4Text &, const G4Transform3D &);
    virtual void 	Draw2D (const G4Circle&, const G4Transform3D& objectTransformation = G4Transform3D());
    virtual void 	Draw2D (const G4NURBS&, const G4Transform3D& objectTransformation = G4Transform3D());
    virtual void 	Draw2D (const G4Polyhedron&, const G4Transform3D& objectTransformation = G4Transform3D());
    virtual void 	Draw2D (const G4Polyline&, const G4Transform3D& objectTransformation = G4Transform3D());
    virtual void 	Draw2D (const G4Polymarker&, const G4Transform3D& objectTransformation = G4Transform3D());
    virtual void 	Draw2D (const G4Square&, const G4Transform3D& objectTransformation = G4Transform3D());
    virtual void 	Draw2D (const G4Text&, const G4Transform3D& objectTransformation = G4Transform3D());
    virtual void 	Draw2D (const G4Text &);
    virtual void	Draw (const G4VHit &);
    virtual void	Draw (const G4VTrajectory &,int);
    virtual void	Draw (const G4LogicalVolume &,
			      const G4VisAttributes &, const G4Transform3D &);
    virtual void	Draw (const G4VPhysicalVolume &,
			      const G4VisAttributes &, const G4Transform3D &);
    virtual void	Draw (const G4VSolid &,
			      const G4VisAttributes&, const G4Transform3D &);
    virtual void 	DispatchToModel (const G4VTrajectory &, G4int i_mode = 0);
    virtual G4bool 	FilterTrajectory (const G4VTrajectory &);
    virtual G4bool 	FilterHit (const G4VHit &);
    
    typedef std::vector<lat::Callback> CallbackList;

    IgState			*m_state;
    CallbackList		m_callbacks;

    // undefined semantics
    VisG4VisManager (const VisG4VisManager &);
    VisG4VisManager &operator= (const VisG4VisManager &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_CORE_VIS_G4_VIS_MANAGER_H
