#ifndef  VIS_MUON_CSC_VIS_CSCSTRIP_DIGI_TWIG_H
# define  VIS_MUON_CSC_VIS_CSCSTRIP_DIGI_TWIG_H

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

class CSCStripDigi;
class CSCDetId;
class VisCSCStackPlots;
class VisCSCChamberSelectorImpl;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCSCStripDigiTwig : public VisQueuedTwig
{
public:
    VisCSCStripDigiTwig (IgState *state, IgTwig *parent, 
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
    virtual void       	update (IgSpareRep *rep);
private:
    std::string	       	m_text;
    const std::string	m_friendlyName;
    const std::string	m_moduleLabel;
    const std::string  	m_instanceName;
    const std::string 	m_processName;
    std::vector< std::pair<CSCDetId, CSCStripDigi> > m_digis;
    edm::ESHandle<CSCGeometry>  m_pDD;
    std::set<CSCDetId> m_dets;
    VisCSCStackPlots* m_stackplots;
    const static edm::ParameterSet* m_pset;
    bool m_StripStyleSolidFlag;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MUON_CSC_VIS_CSCSTRIP_DIGI_TWIG_H
