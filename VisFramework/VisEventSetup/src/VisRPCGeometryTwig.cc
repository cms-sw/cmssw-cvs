//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisEventSetup/interface/VisRPCGeometryTwig.h"
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

VisRPCGeometryTwig::VisRPCGeometryTwig (IgState *state, IgTwig *parent, 
					const std::string &name,  
					const int region,
					const int wheel,
					const int sector, 
					const int station)
    : VisQueuedTwig (state, parent, name),
      m_region (region),
      m_wheel (wheel),
      m_sector (sector),
      m_station (station),
      m_3dDet (NULL),
      m_rzDet (NULL),
      m_rphiDet (NULL),
      m_3Ddone (false),
      m_rzdone (false),
      m_rphidone (false),
      m_annotate (state, lat::CreateCallback (this, &VisRPCGeometryTwig::invalidate))
{}

/** Clean the scene graph. */
VisRPCGeometryTwig::~VisRPCGeometryTwig (void) 
{
    if (m_3dDet != NULL) m_3dDet->unref ();
    if (m_rzDet != NULL) m_rzDet->unref ();
    if (m_rphiDet != NULL) m_rphiDet->unref ();
}

void
VisRPCGeometryTwig::invalidate (void)
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
VisRPCGeometryTwig::onNewEvent (const edm::Event &event,
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
VisRPCGeometryTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "CMS RPC Muon Geometry: ";
    text << this->name () << "<br>";
    
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>#</TH>"
	 << "<TH>Barrel/Wheel/Sector/Station</TH>"
	 << "<TH>Position</TH>"
	 << "</TR>";

    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);
    
    text << esService->rpcTextRep (m_region, m_wheel, m_station, m_sector);
    text << "</table>";
    rep->setText (text.str ());
}

void
VisRPCGeometryTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_3Ddone)
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	m_3dDet = esService->buildRPCChambers (VisEventSetupService::FullDet, m_region, m_wheel, m_sector, m_station);

	if (m_3dDet == NULL) return;

	if (m_annotate.value ()) 
	{
	    SoSeparator *sep = esService->annotateRPCChambers (VisEventSetupService::FullDet, m_region, m_wheel, m_sector, m_station);
	    if (sep != NULL) m_3dDet->addChild (sep);
	}
	
	m_3dDet->ref ();
	m_3Ddone = true;	
    }
    rep->node ()->addChild (m_3dDet);
}

void
VisRPCGeometryTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_rphidone)
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	m_rphiDet = esService->buildRPCChambers (VisEventSetupService::RPhiDet, m_region, m_wheel, m_sector, m_station);

	if (m_rphiDet == NULL) return;

	if (m_annotate.value ())
	{
	    SoSeparator *sep = esService->annotateRPCChambers (VisEventSetupService::RPhiDet, m_region, m_wheel, m_sector, m_station);
	    if (sep != NULL) m_rphiDet->addChild (sep);	    
	}
	
	m_rphiDet->ref ();
	m_rphidone = true;
    }
    rep->node ()->addChild (m_rphiDet);
}

void
VisRPCGeometryTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_rzdone)
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	m_rzdone = esService->buildRPCChambers (VisEventSetupService::RZDet, m_region, m_wheel, m_sector, m_station);

	if (m_rzDet == NULL) return;

	if (m_annotate.value ())
	{
	    SoSeparator *sep = esService->annotateRPCChambers (VisEventSetupService::RZDet, m_region, m_wheel, m_sector, m_station);
	    if (sep != NULL) m_rzDet->addChild (sep);
	}
	
	m_rzDet->ref ();	
	m_rzdone = true;	
    }
    rep->node ()->addChild (m_rzDet);
}
