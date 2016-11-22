#ifndef VIS_MUON_CSC_VIS_CSCSEGMENT_TWIG_H
# define VIS_MUON_CSC_VIS_CSCSEGMENT_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "Geometry/CSCGeometry/interface/CSCGeometry.h"
# include "FWCore/ParameterSet/interface/ParameterSet.h"
# include <vector>
# include <set>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class CSCSegment;
class CSCDetId;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCSCSegmentTwig : public VisQueuedTwig
{
public:
    VisCSCSegmentTwig (IgState *state, IgTwig *parent, 
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
    std::string	       	m_text;
    const std::string  	m_friendlyName;
    const std::string	m_moduleLabel;
    const std::string	m_instanceName;
    const std::string 	m_processName;
    std::vector<CSCSegment> 	m_segs;
    edm::ESHandle<CSCGeometry>	m_pDD;
    std::set<CSCDetId> m_dets;
    const static edm::ParameterSet* m_pset;
    float m_lineThickness;
    float m_lineThicknessEnvelope;
    int   m_lineColor;
    int   m_lineColorEnvelope;
    bool  m_envelopeFlag; //! true to enable connecting lines between segments
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MUON_CSC_VIS_CSCSEGMENT_TWIG_H
