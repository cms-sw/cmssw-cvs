//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisEventSetup/interface/VisCaloGeometryTwig.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
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

VisCaloGeometryTwig::VisCaloGeometryTwig (IgState *state, IgTwig *parent,
					  const std::string &name,
					  const std::string detId, const int subdetn)
    : VisQueuedTwig (state, parent, name),
      m_detId (detId),
      m_subdetn (subdetn),
      m_3dDet (NULL),
      m_rzDet (NULL),
      m_rphiDet (NULL),
      m_legoDet (NULL),
      m_3Ddone (false),
      m_rzdone (false),
      m_rphidone (false),
      m_legodone (false),
      m_annotate (state, lat::CreateCallback (this, &VisCaloGeometryTwig::invalidate)),
      m_scale (state, lat::CreateCallback (this, &VisCaloGeometryTwig::invalidate))
{}

/** Clean the scene graph. */
VisCaloGeometryTwig::~VisCaloGeometryTwig (void) 
{
    if (m_3dDet != NULL) m_3dDet->unref ();
    if (m_rzDet != NULL) m_rzDet->unref ();
    if (m_rphiDet != NULL) m_rphiDet->unref ();
    if (m_legoDet != NULL) m_legoDet->unref ();
}

void
VisCaloGeometryTwig::onNewEvent (const edm::Event &event,
				 const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    if (esService->caloGeomChanged ())
	invalidate ();
}

void
VisCaloGeometryTwig::invalidate (void)
{      
    if (m_3dDet != NULL) m_3dDet->unref ();
    if (m_rzDet != NULL) m_rzDet->unref ();
    if (m_rphiDet != NULL) m_rphiDet->unref ();
    if (m_legoDet != NULL) m_legoDet->unref ();
    m_3Ddone = false;    
    m_rzdone = false;
    m_rphidone = false;
    m_legodone = false;    

    VisQueuedTwig::onBaseInvalidate ();
}

void
VisCaloGeometryTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "CMS Calo Geometry: ";
    text << this->name () << "<br>";
    
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>#</TH>"
	 << "<TH>id(dec)</TH>"
	 << "<TH>Position</TH>"
	 << "</TR>";

    if (m_subdetn != -1)
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);
	text << esService->caloTextRep (m_detId, m_subdetn);
    }
    else
	text << "<TR align = right><TD COLSPAN = 3>Only LEGO view is available for this twig.</TD></TR>";
    text << "</table>";

    rep->setText (text.str ());
}

void
VisCaloGeometryTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (m_subdetn != -1)
    {
	if (! m_3Ddone)
	{
	    VisEventSetupService *esService = VisEventSetupService::get (state ());
	    ASSERT (esService);

	    m_3dDet = esService->buildCaloDet (VisEventSetupService::FullDet, m_detId, m_subdetn);
	    if (m_3dDet == NULL) return;

	    if (m_annotate.value ())
	    {	    
		SoSeparator *sep = esService->annotateCaloDet (VisEventSetupService::FullDet, m_detId, m_subdetn);
		if (sep != NULL) m_3dDet->addChild (sep);	    
	    }
	
	    m_3dDet->ref ();
	    m_3Ddone = true;	
	}
	rep->node ()->addChild (m_3dDet);
    }    
}
 
void
VisCaloGeometryTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (m_subdetn != -1)
    {
	if (! m_rphidone)
	{
	    VisEventSetupService *esService = VisEventSetupService::get (state ());
	    ASSERT (esService);

	    m_rphiDet = esService->buildCaloDet (VisEventSetupService::RPhiDet, m_detId, m_subdetn);
	    if (m_rphiDet == NULL) return;

	    if (m_annotate.value ())
	    {	    
		SoSeparator *sep = esService->annotateCaloDet (VisEventSetupService::RPhiDet, m_detId, m_subdetn);
		if (sep != NULL) m_rphiDet->addChild (sep);	    
	    }

	    m_rphiDet->ref ();
	    m_rphidone = true;
	}
	rep->node ()->addChild (m_rphiDet);
    }    
}
 
void
VisCaloGeometryTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (m_subdetn != -1)
    {
	if (! m_rzdone)
	{
	    VisEventSetupService *esService = VisEventSetupService::get (state ());
	    ASSERT (esService);

	    m_rzDet = esService->buildCaloDet (VisEventSetupService::RZDet, m_detId, m_subdetn);
	    if (m_rzDet == NULL) return;

	    if (m_annotate.value ()) 
	    {	    
		SoSeparator *sep = esService->annotateCaloDet (VisEventSetupService::RZDet, m_detId, m_subdetn);
		if (sep != NULL) m_rzDet->addChild (sep);
	    }
	
	    m_rzDet->ref ();
	    m_rzdone = true;
	}
	rep->node ()->addChild (m_rzDet);
    }
}

void
VisCaloGeometryTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);    
    rep->clear ();

    if ((! m_legodone) && (m_subdetn == -1))
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	m_legoDet = esService->buildLegoDet (m_scale.value ());
	m_legoDet->ref ();
	m_legodone = true;
    }

    if (m_legoDet != NULL) 
	rep->node ()->addChild (m_legoDet);
}
