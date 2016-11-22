#ifndef VIS_EVENT_SETUP_VIS_CALO_GEOMETRY_TWIG_H
# define VIS_EVENT_SETUP_VIS_CALO_GEOMETRY_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisAnnotateGeometry.h"
# include "VisFramework/VisFrameworkBase/interface/VisLegoScale.h"

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

class VisCaloGeometryTwig : public VisQueuedTwig
{
public:
    VisCaloGeometryTwig (IgState *state, IgTwig *parent, 
			 const std::string &name = "",
			 const std::string detId = "", 
			 const int subdetn = -1);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual ~VisCaloGeometryTwig (void);

    virtual void      	onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup);

    using VisQueuedTwig::update;
    virtual void        update (IgTextRep *rep);
    virtual void       	update (Ig3DRep *rep);
    virtual void        update (IgRPhiRep *rep);
    virtual void        update (IgRZRep *rep);
    virtual void        update (IgLegoRep *rep);

private:    
    void 		invalidate (void);
    
    const std::string 	m_detId;
    const int 		m_subdetn;
    SoSeparator	       *m_3dDet;
    SoSeparator	       *m_rzDet;
    SoSeparator	       *m_rphiDet;
    SoSeparator	       *m_legoDet;    
    bool		m_3Ddone;
    bool		m_rzdone;
    bool		m_rphidone;
    bool		m_legodone;    
    VisAnnotateGeometry m_annotate;
    VisLegoScale	m_scale;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_EVENT_SETUP_VIS_CALO_GEOMETRY_TWIG_H
