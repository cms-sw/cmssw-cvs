//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisEventSetup/interface/VisTrackerGeometryTwig.h"
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

VisTrackerGeometryTwig::VisTrackerGeometryTwig (IgState *state, IgTwig *parent,
						const std::string &name /* = "" */)
    : VisQueuedTwig (state, parent, name),
      m_3dDet (NULL),
      m_rzDet (NULL),
      m_rphiDet (NULL),
      m_3Ddone (false),
      m_rzdone (false),
      m_rphidone (false),
      m_annotate (state, lat::CreateCallback (this, &VisTrackerGeometryTwig::invalidate))
{}

/** Clean the scene graph. */
VisTrackerGeometryTwig::~VisTrackerGeometryTwig (void) 
{
    if (m_3dDet != NULL) m_3dDet->unref ();
    if (m_rzDet != NULL) m_rzDet->unref ();
    if (m_rphiDet != NULL) m_rphiDet->unref ();
}

void
VisTrackerGeometryTwig::invalidate (void)
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
VisTrackerGeometryTwig::onNewEvent (const edm::Event &event,
				    const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    if (esService->trackerGeomChanged ())
	invalidate ();
}

void
VisTrackerGeometryTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "CMS Tracker Geometry: ";
    text << this->name () << "<br>";
    
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>#</TH>"
	 << "<TH>Det ID</TH>"
	 << "<TH>Position</TH>"
	 << "</TR>";

    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);
    
    text << esService->trackerTextRep (this->name ());
    text << "</table>";
    rep->setText (text.str ());
}

void
VisTrackerGeometryTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_3Ddone)
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	m_3dDet = esService->buildTracker (VisEventSetupService::FullDet, this->name ());

	if (m_3dDet == NULL) return;

	if (m_annotate.value ())
	{	    
	    SoSeparator *sep = esService->annotateTracker (VisEventSetupService::FullDet, this->name ());
	    if (sep != NULL) m_3dDet->addChild (sep);	    
	}

	m_3dDet->ref ();
	m_3Ddone = true;	
    }   
    rep->node ()->addChild (m_3dDet);
}

void
VisTrackerGeometryTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    if (! m_rphidone)
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	m_rphiDet = esService->buildTracker (VisEventSetupService::RPhiDet, this->name ());

	if (m_rphiDet == NULL) return;

	if (m_annotate.value ())
	{	    
	    SoSeparator *sep = esService->annotateTracker (VisEventSetupService::RPhiDet, this->name ());
	    if (sep != NULL) m_rphiDet->addChild (sep);	    
	}
	
	m_rphiDet->ref ();
	m_rphidone = true;
    }    
    rep->node ()->addChild (m_rphiDet);
}

void
VisTrackerGeometryTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    if (! m_rzdone)
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	m_rzDet = esService->buildTracker (VisEventSetupService::RZDet, this->name ());

	if (m_rzDet == NULL) return;

	if (m_annotate.value ())
	{
	    SoSeparator *sep = esService->annotateTracker (VisEventSetupService::RZDet, this->name ());
	    if (sep != NULL) m_rzDet->addChild (sep);
	}

	m_rzDet->ref ();	
	m_rzdone = true;	
    }
    rep->node ()->addChild (m_rzDet);
}
