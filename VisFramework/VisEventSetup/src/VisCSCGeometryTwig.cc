//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisEventSetup/interface/VisCSCGeometryTwig.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCSCGeometryTwig::VisCSCGeometryTwig (IgState *state, IgTwig *parent,
					const std::string &name,
					const int endcap, const int station,
					const int ring, const int chamber)
    : VisQueuedTwig (state, parent, name),
      m_endcap (endcap),
      m_station (station),
      m_ring (ring),
      m_chamber (chamber),
      m_3dDet (NULL),
      m_rzDet (NULL),
      m_rphiDet (NULL),
      m_3Ddone (false),
      m_rzdone (false),
      m_rphidone (false),
      m_annotate (state, lat::CreateCallback (this, &VisCSCGeometryTwig::invalidate))
{}

/** Clean the scene graph. */
VisCSCGeometryTwig::~VisCSCGeometryTwig (void) 
{
    if (m_3dDet != NULL) m_3dDet->unref ();
    if (m_rzDet != NULL) m_rzDet->unref ();
    if (m_rphiDet != NULL) m_rphiDet->unref ();
}

void
VisCSCGeometryTwig::onNewEvent (const edm::Event &event,
				const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    if (esService->muonGeomChanged ())
	invalidate ();
}

void
VisCSCGeometryTwig::invalidate (void)
{      
    if (m_3dDet != NULL) m_3dDet->unref ();
    if (m_rzDet != NULL) m_rzDet->unref ();
    if (m_rphiDet != NULL) m_rphiDet->unref ();
    m_3Ddone = false;    
    m_rzdone = false;
    m_rphidone = false;

    VisQueuedTwig::onBaseInvalidate ();
}

void
VisCSCGeometryTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "CMS CSC Muon Geometry: ";
    text << this->name () << "<br>";
    
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>#</TH>"
	 << "<TH>id(hex)</TH>"
	 << "<TH>id(dec)</TH>"
	 << "<TH>Endcap/Station/Ring/Chamber</TH>"
	 << "<TH>Position</TH>"
	 << "</TR>";

    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);
    
    text << esService->cscTextRep (m_chamber, m_ring, m_station, m_endcap);
    text << "</table>";

    rep->setText (text.str ());
}

void
VisCSCGeometryTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    if (! m_3Ddone)
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	m_3dDet = esService->buildCSCChambers (VisEventSetupService::FullDet, m_chamber, m_ring, m_station, m_endcap);
	if (m_3dDet == NULL) return;

	if (m_annotate.value ())
	{	    
	    SoSeparator *sep = esService->annotateCSCChambers (VisEventSetupService::FullDet, m_chamber, m_ring, m_station, m_endcap);
	    if (sep != NULL) m_3dDet->addChild (sep);	    
	}
	
	m_3dDet->ref ();
	m_3Ddone = true;	
    }
    rep->node ()->addChild (m_3dDet);
}
 
void
VisCSCGeometryTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_rphidone)
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	m_rphiDet = esService->buildCSCChambers (VisEventSetupService::RPhiDet, m_chamber, m_ring, m_station, m_endcap);
	if (m_rphiDet == NULL) return;

	if (m_annotate.value ())
	{	    
	    SoSeparator *sep = esService->annotateCSCChambers (VisEventSetupService::RPhiDet, m_chamber, m_ring, m_station, m_endcap);
	    if (sep != NULL) m_rphiDet->addChild (sep);	    
	}

	m_rphiDet->ref ();
	m_rphidone = true;
    }
    rep->node ()->addChild (m_rphiDet);
}
 
void
VisCSCGeometryTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_rzdone)
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	m_rzDet = esService->buildCSCChambers (VisEventSetupService::RZDet, m_chamber, m_ring, m_station, m_endcap);
	if (m_rzDet == NULL) return;

	if (m_annotate.value ()) 
	{	    
	    SoSeparator *sep = esService->annotateCSCChambers (VisEventSetupService::RZDet, m_chamber, m_ring, m_station, m_endcap);
	    if (sep != NULL) m_rzDet->addChild (sep);
	}
	
	m_rzDet->ref ();
	m_rzdone = true;
    }
    rep->node ()->addChild (m_rzDet);
}
