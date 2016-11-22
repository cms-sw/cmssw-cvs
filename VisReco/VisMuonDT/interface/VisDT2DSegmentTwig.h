#ifndef VIS_MUON_DT_VIS_DT2SEGMENT_TWIG_H
# define VIS_MUON_DT_VIS_DT2SEGMENT_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "Geometry/DTGeometry/interface/DTGeometry.h"

# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class DTSLRecSegment2D;
class DTLayerId;
class DTWireId;
class SoSeparator;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisDT2DSegmentTwig : public VisQueuedTwig
{
public:
    VisDT2DSegmentTwig (IgState *state, IgTwig *parent, 
			const std::string &name = "",
			const std::string &friendlyName = "",
			const std::string &moduleLabel = "",
			const std::string &instanceName = "",
			const std::string &processName = "");

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual void	onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup);
    using VisQueuedTwig::update;
    virtual void	update (Ig3DRep *rep);
    virtual void       	update (IgLegoRep *rep);
    virtual void       	update (IgTextRep *rep);
    virtual void       	update (IgRPhiRep *rep);
    virtual void       	update (IgRZRep *rep);

private:
    void 		addMaterial (SoSeparator *sep);
    std::string	       	m_text;
    const std::string	m_friendlyName;
    const std::string	m_moduleLabel;
    const std::string	m_instanceName;
    const std::string 	m_processName;
    std::vector< DTSLRecSegment2D >             m_recseg;
    edm::ESHandle<DTGeometry>  			m_pDD;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MUON_DT_VIS_DTRECHITS_TWIG_H

