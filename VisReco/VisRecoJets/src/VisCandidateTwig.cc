//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisRecoJets/interface/VisCandidateTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoArrow.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <qstring.h>
#include <sstream>
#include <iomanip>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

namespace 
{
    VisQueuedTwig *
    createThisTwig (IgState *state, IgTwig *parent,
		    const std::string &name,
		    const std::string &friendlyName,
		    const std::string &modLabel,
		    const std::string &instanceName,
		    const std::string &processName)
    {
	IgTwig *rootTwig = IgDocumentData::get (state)->root ();
	IgTwig *eventTwig = 0;
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Candidates");
	
	if (! eventTwig)
	    eventTwig = parent;
	return new VisCandidateTwig (state, eventTwig, "[N/A] Candidate (" + name + ")", 
				     friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCandidateTwig::VisCandidateTwig (IgState *state, IgTwig *parent,
				    const std::string &name /* = "" */,
				    const std::string &friendlyName /* = "" */,
				    const std::string &moduleLabel /* = "" */,
				    const std::string &instanceName /* = "" */,
				    const std::string &processName /* = "" */)
    : VisQueuedTwig (state, parent, name),
      m_text (name),
      m_friendlyName (friendlyName),
      m_moduleLabel (moduleLabel),
      m_instanceName (instanceName),
      m_processName (processName),
      m_scale (state, lat::CreateCallback (this, &VisCandidateTwig::scaleChanged)),
      m_cut (state, lat::CreateCallback (this, &VisCandidateTwig::scaleChanged)),
      m_escale (state, lat::CreateCallback (this, &VisCandidateTwig::scaleChanged)),
      m_annotation (state, lat::CreateCallback (this, &VisCandidateTwig::scaleChanged))
{    
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }	
    edm::TypeID candID (typeid (reco::CandidateCollection));
    tfService->registerTwig (candID.friendlyClassName (), &createThisTwig);
}

void
VisCandidateTwig::scaleChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }


void
VisCandidateTwig::onNewEvent (const edm::Event &event,
			      const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_text = QString ("Run # %1, event # %2")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ()).latin1 ();
    m_candidates.clear ();
    
    edm::Handle<reco::CandidateCollection> collections;
    try 
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.get (visSel, collections);
	}
    }
    catch (cms::Exception& e)
    {
	if (this->m_onCmsException)
	    this->m_onCmsException (&e);
    }
    catch (lat::Error &e) 
    {
	if (this->m_onError)
	    this->m_onError (&e);
    }
    catch (std::exception &e) 
    {
	if (this->m_onException)
	    this->m_onException (&e);
    }
    catch (...) 
    {
	if (this->m_onUnhandledException)
	    this->m_onUnhandledException ();
    }
    if (collections.isValid ())
    {
	QString sizeStr = (QString ("%1").arg (collections->size ()));
	QString nameStr = QString::fromStdString (this->name ());
	int ib = nameStr.find ("[");
	int ie = nameStr.find ("]");
	nameStr.replace (ib + 1, ie - 1, sizeStr);	    
	this->name (nameStr.toStdString ());

	for (reco::CandidateCollection::const_iterator it = collections->begin (), itEnd = collections->end (); it != itEnd; ++it) 
	{
	    m_candidates.push_back ((*it).clone ());
	}
    }

    VisQueuedTwig::onBaseInvalidate ();
}

void
VisCandidateTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;

    text << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>E<SUB>T (GeV)</TH>"
	 << "<TH>eta</TH>"
	 << "<TH>phi [rad]</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
    
    int n = 0;
    if (! m_candidates.empty ()) 
    {
	try 
	{
	    for (std::vector<const reco::Candidate *>::const_iterator it = m_candidates.begin (), itEnd = m_candidates.end (); it != itEnd; ++it) 
	    {
		double et = (*it)->et ();	    
		double phi = (*it)->phi ();
		double eta = (*it)->eta ();
	
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << n++ << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << et << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << eta << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << phi << "</TD>"
		     << "</TR>";
	    }
	}
	catch (cms::Exception& e)
	{
	    if (this->m_onCmsException)
		this->m_onCmsException (&e);
	}
	catch (lat::Error &e) 
	{
	    if (this->m_onError)
		this->m_onError (&e);
	}
	catch (std::exception &e) 
	{
	    if (this->m_onException)
		this->m_onException (&e);
	}
	catch (...) 
	{
	    if (this->m_onUnhandledException)
		this->m_onUnhandledException ();
	}
    }	
       
    text << "</table>";
    
    // Send it over.
    rep->setText (text.str ());
}

/** Update object */
void
VisCandidateTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *contents = new SoSeparator;
    SoSeparator *candSep = new SoSeparator;
    contents->addChild (candSep);
    
    SoSeparator *labelSep = new SoSeparator;
    contents->addChild (labelSep);
    
    if (! m_candidates.empty ()) 
    {
	try 
	{
// 	    unsigned colour [10] = { 0xff000000, 0x00ff0000, 0xffff0000, // red green yellow
// 				     0xff00ff00, 0x00ffff00, 0xd0dfff00, // magenta cyan
// 				     0xff000fff, 0x00ffff00, 0xe00fff00,
// 				     0xff00ee00};
// 	    int i = 0;
	
	    for (std::vector<const reco::Candidate *>::const_iterator it = m_candidates.begin (), itEnd = m_candidates.end (); it != itEnd; ++it)
	    {
		double et = (*it)->et ();
		if (et > m_cut.value ()) 
		{
		    SbVec3f tipPos (SbVec3f ((*it)->px () / 100.0, (*it)->py () / 100.0, (*it)->pz () / 100.0));
		    SbVec3f tailPos (SbVec3f ((*it)->vx () / 100.0, (*it)->vy () / 100.0, (*it)->vz () / 100.0));
		    IgSoArrow* arrow = new IgSoArrow;
		    arrow->tail.setValue (tailPos);
		    arrow->tip.setValue (tipPos);	
		    candSep->addChild (arrow);
		}
	    }
	}
	catch (cms::Exception& e)
	{
	    if (this->m_onCmsException)
		this->m_onCmsException (&e);
	}
	catch (lat::Error &e) 
	{
	    if (this->m_onError)
		this->m_onError (&e);
	}
	catch (std::exception &e) 
	{
	    if (this->m_onException)
		this->m_onException (&e);
	}
	catch (...) 
	{
	    if (this->m_onUnhandledException)
		this->m_onUnhandledException ();
	}
    }

    rep->node ()->addChild (contents);
}

/** Update object */
void
VisCandidateTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    SoSeparator *contents = new SoSeparator;
    
    SoSeparator *candSep = new SoSeparator;
    contents->addChild (candSep);
    
    SoSeparator *labelSep = new SoSeparator;
    contents->addChild (labelSep);
    
    if (! m_candidates.empty ()) 
    {
	try 
	{
	    unsigned colour [10] = { 0xff000000, 0x00ff0000, 0xffff0000, // red green yellow 
				     0xff00ff00, 0x00ffff00, 0xd0dfff00, // magenta cyan
				     0xff000fff, 0x00ffff00, 0xe00fff00,
				     0xff00ee00};
	    int i = 0;

	    for (std::vector<const reco::Candidate *>::const_iterator it = m_candidates.begin (), itEnd = m_candidates.end (); it != itEnd; ++it)
	    {
		double et = (*it)->et ();
		if (et > m_cut.value ()) 
		{
		    if (i == 10) i = 0;
	    
		    SoSeparator *sep = new SoSeparator;
		    candSep->addChild (sep);
		    SoMaterial *mat = new SoMaterial;
		    float rgbcomponents [4];
		    IgSbColorMap::unpack (colour [i++], rgbcomponents);
		    mat->diffuseColor.setValue (rgbcomponents);
		    sep->addChild (mat);

		    SoFont *font = new SoFont;
		    font->name = "Times-Roman";
		    font->size = 14.0;
		    sep->addChild (font);
	    
		    double eta = (*it)->eta ();
		    double phi = (*it)->phi ();
		    if (phi < 0) phi += 2 * M_PI;

		    if (m_annotation.value ())
		    {			
			QString label = QString ("Et = %1 GeV")
					.arg (et, 0, 'f', 2);
			SoSeparator *labelSep = new SoSeparator;
			SoText2  *labelText = new SoText2;
			labelText->string = label.latin1 ();
			SoTranslation *labelTranslation = new SoTranslation;
			double labelHight = et / m_scale.value () + 0.2;
	    
			labelTranslation->translation = SbVec3f (phi, labelHight, eta); //m_scale.value ()
	    
			labelSep->addChild (labelTranslation);
			labelSep->addChild (labelText);
			sep->addChild (labelSep);
		    }
	    
		    SoSeparator *tsep = new SoSeparator;
		    
		    SoTranslation *localTransl = new SoTranslation;
		    localTransl->translation.setValue (SbVec3f (phi, et / (2 * m_scale.value ()), eta));
		    
		    SoCube *towerBox = new SoCube;
		    towerBox->height = et / m_scale.value () + 0.001;
		    if (fabs(eta) > 1.74) towerBox->width = 0.176f;
		    else towerBox->width  = 0.089;
		    towerBox->depth  = esService->phi4eta (fabs(eta)) + 0.002;
		    
		    tsep->addChild (localTransl);
		    tsep->addChild (towerBox);
		    sep->addChild (tsep);
		}
	    }
	}
	catch (cms::Exception& e)
	{
	    if (this->m_onCmsException)
		this->m_onCmsException (&e);
	}
	catch (lat::Error &e) 
	{
	    if (this->m_onError)
		this->m_onError (&e);
	}
	catch (std::exception &e) 
	{
	    if (this->m_onException)
		this->m_onException (&e);
	}
	catch (...) 
	{
	    if (this->m_onUnhandledException)
		this->m_onUnhandledException ();
	}
    }

    rep->node ()->addChild (contents);
}

void
VisCandidateTwig::update (IgRPhiRep *rep) 
{}

void
VisCandidateTwig::update (IgRZRep *rep) 
{}

