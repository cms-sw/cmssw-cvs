#ifndef VIS_EVENT_SETUP_VIS_CSCGEOMETRY_TWIG_H
# define VIS_EVENT_SETUP_VIS_CSCGEOMETRY_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisAnnotateGeometry.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

namespace edm 
{
    class Event;
    class EventSetup;
}

class SoSeparator;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCSCGeometryTwig : public VisQueuedTwig
{
public:
    VisCSCGeometryTwig (IgState *state, IgTwig *parent, 
			const std::string &name = "", 
			const int endcap = 0,
			const int station = 0, 
			const int ring = 0, 
			const int chamber = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual ~VisCSCGeometryTwig (void);

    virtual void      	onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup);

    using VisQueuedTwig::update;
    virtual void        update (IgTextRep *rep);
    virtual void       	update (Ig3DRep *rep);
    virtual void        update (IgRPhiRep *rep);
    virtual void        update (IgRZRep *rep);

private:    
    void 		invalidate (void);
    
    const int 		m_endcap;
    const int 		m_station;
    const int 		m_ring;
    const int 		m_chamber;
    SoSeparator	       *m_3dDet;
    SoSeparator	       *m_rzDet;
    SoSeparator	       *m_rphiDet;
    bool		m_3Ddone;
    bool		m_rzdone;
    bool		m_rphidone;
    VisAnnotateGeometry m_annotate;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_EVENT_SETUP_VIS_CSCGEOMETRY_TWIG_H
