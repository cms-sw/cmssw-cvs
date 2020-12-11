#ifndef VIS_MUON_DT_VIS_DTDIGI_TWIG_H
# define VIS_MUON_DT_VIS_DTDIGI_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgTwig.h"
# include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
# include "FWCore/Framework/interface/ESHandle.h"
# include "Geometry/DTGeometry/interface/DTGeometry.h"

# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class DTDigi;
class DTLayerId;
class SoSeparator;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisDTDigiTwig : public VisQueuedTwig
{
public:
    VisDTDigiTwig (IgState *state, IgTwig *parent, 
		   const std::string &name = "",
		   const std::string &friendlyName = "",
		   const std::string &moduleLabel = "",
		   const std::string &instanceName = "",
		   const std::string &processName = "",
		   //		   int layer = 0, 
		   unsigned color = 0x66ff0000,
		   int wheel = 10,
		   int sector = 0);

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

    void		annotate (bool flag);

private:
    void	      	addWire (SoSeparator *separator, const std::pair<DTLayerId, DTDigi> &digi);
    SoSeparator	       *addText (const std::pair<DTLayerId, DTDigi> &digi);
    void	      	addDescription (SoSeparator *separator, const std::pair<DTLayerId, DTDigi> &digi);

    std::string	       	m_text;
    const std::string	m_friendlyName;
    const std::string	m_moduleLabel;
    const std::string	m_instanceName;
    const std::string	m_processName;
    int						m_layer;
    unsigned					m_rgba;
    int 			                m_wheel;
    int 			                m_sector;
    std::vector< std::pair<DTLayerId, DTDigi> > m_digis;
    edm::ESHandle<DTGeometry>  			m_pDD;
    bool 			                m_annotate;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MUON_DT_VIS_DTDIGI_TWIG_H
