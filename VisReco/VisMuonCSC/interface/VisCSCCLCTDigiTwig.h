#ifndef VIS_MUON_CSC_VIS_CSCCLCTDIGI_TWIG_H
# define VIS_MUON_CSC_VIS_CSCCLCTDIGI_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "VisReco/VisMuonCSC/interface/VisCSCChamberDrawer.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "Geometry/CSCGeometry/interface/CSCGeometry.h"
# include <vector>
# include <set>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class CSCCLCTDigi;
class CSCDetId;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCSCCLCTDigiTwig : public VisQueuedTwig
{
public:
    VisCSCCLCTDigiTwig (IgState *state, IgTwig *parent, 
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
    const std::string	m_friendlyName;
    const std::string  	m_moduleLabel;
    const std::string	m_instanceName;
    const std::string	m_processName;
    std::vector< std::pair<CSCDetId, CSCCLCTDigi> > m_digis;
    edm::ESHandle<CSCGeometry>  m_pDD;
    std::set<CSCDetId> m_dets;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MUON_CSC_VIS_CSCCLCTDIGI_TWIG_H
