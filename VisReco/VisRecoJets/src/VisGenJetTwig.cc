//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisRecoJets/interface/VisGenJetTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/TypeID.h"
#include "Iguana/Inventor/interface/IgSoTower.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoJet.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoText2.h>
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
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisGenJetTwig::VisGenJetTwig (IgState *state, IgTwig *parent,
			      const std::string &name /* = "" */,
			      const edm::Event &event, 
			      const edm::EventSetup &eventSetup,
			      const reco::GenJet &jet)
    : VisQueuedTwig (state, parent, name),
      m_event (event),
      m_eventSetup (eventSetup),
      m_jet (jet),
      m_scale (state, lat::CreateCallback (this, &VisGenJetTwig::scaleChanged)),
      m_cut (state, lat::CreateCallback (this, &VisGenJetTwig::scaleChanged)),
      m_escale (state, lat::CreateCallback (this, &VisGenJetTwig::scaleChanged)),
      m_annotation (state, lat::CreateCallback (this, &VisGenJetTwig::scaleChanged))
{}

void
VisGenJetTwig::scaleChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisGenJetTwig::onNewEvent (const edm::Event &event,
			   const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);
}

void
VisGenJetTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;

    text << QString ("Run # %1, event # %2")
	.arg (m_event.id ().run ())
	.arg (m_event.id ().event ()).latin1 () << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>E<SUB>t (GeV)</TH>"
	 << "<TH>E<SUB>em (GeV)</TH>"
	 << "<TH>E<SUB>had</TH>"
	 << "<TH>E<SUB>inv</TH>"
	 << "<TH>E<SUB>aux</TH>"
	 << "<TH>Eta</TH>"
	 << "<TH>Phi</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    try 
    {
	double jetEt = m_jet.et ();
	if (jetEt > m_cut.value ())
	{
	    double eEm = m_jet.emEnergy ();
	    double eHad = m_jet.hadEnergy ();
	    double eInv = m_jet.invisibleEnergy ();
	    double eAux = m_jet.auxiliaryEnergy ();
	
	    text << "<TR align = right>"
		 << "<TD>" << std::setw (2) << std::setprecision (3) << jetEt << "</TD>"
		 << "<TD>" << std::setw (2) << std::setprecision (3) << eEm << "</TD>"
		 << "<TD>" << std::setw (6) << std::setprecision (3) << eHad << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << eInv << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << eAux << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << m_jet.eta () << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << m_jet.phi () << "</TD></TR>";

	    text << "</table>";
	} 
	else 
	{
	    text << "</table><br>The jet energy " << jetEt << "GeV is below an energy cut (" << m_cut.value () << "<br>";
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
    
    // Send it over.
    rep->setText (text.str ());
}

/** Update object */
void
VisGenJetTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *contents = new SoSeparator;
    
    SoSeparator *jetsSep = new SoSeparator;
    contents->addChild (jetsSep);
    
    SoSeparator *labelSep = new SoSeparator;
    contents->addChild (labelSep);
    
    try 
    {
	double jetEt = m_jet.et ();
	if (jetEt > m_cut.value ()) 
	{
	    SoSeparator *sep = new SoSeparator;
	    jetsSep->addChild (sep);
	    SoMaterial *mat = new SoMaterial;
	    float rgbcomponents [4];
	    IgSbColorMap::unpack (0x66330000, rgbcomponents);
	    mat->diffuseColor.setValue (rgbcomponents);
	    sep->addChild (mat);
	    
	    double jetPhi = m_jet.phi();
	    if (jetPhi < 0) jetPhi += 2 * M_PI;
	    double jetEta = m_jet.eta();
	    double jetTheta = 2.0 * atan (exp (-jetEta));
	    
	    QString label = QString ("Et = %1 GeV")
			    .arg (jetEt, 0, 'g', 2);
	    SoSeparator *labelSep = new SoSeparator;
	    SoText2  *jetLabel = new SoText2;
	    jetLabel->string = label.latin1 ();
	    SoTranslation *jetLabelTranslation = new SoTranslation;
	    
	    double ct = cos (jetTheta);
	    double st = sin (jetTheta);
	    double cp = cos (jetPhi);
	    double sp = sin (jetPhi);
	    
	    float length1 = ct ? 4.0 / fabs (ct) : 4.0;
	    float length2 = st ? 2.0 / fabs (st) : 2.0;
	    float bodyHeight = length1 < length2 ? length1 : length2;
	    float hatHeight = 2.0 * jetEt / 100.0;
	    jetLabelTranslation->translation = SbVec3f ((bodyHeight + hatHeight) * st * cp,
							(bodyHeight + hatHeight) * st * sp,
							(bodyHeight + hatHeight) * ct);
	    
	    labelSep->addChild (jetLabelTranslation);
	    labelSep->addChild (jetLabel);
	    sep->addChild (labelSep);
	    
	    IgSoJet *recoJet = new IgSoJet;
	    recoJet->theta.setValue (jetTheta);
	    recoJet->phi.setValue (jetPhi);
	    recoJet->energy.setValue (jetEt);
	    sep->addChild (recoJet); 
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

    rep->node ()->addChild (contents);
}

/** Update object */
void
VisGenJetTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    SoSeparator *contents = new SoSeparator;
    
    SoSeparator *jetsSep = new SoSeparator;
    contents->addChild (jetsSep);
    
    SoSeparator *labelSep = new SoSeparator;
    contents->addChild (labelSep);
    
    try 
    {
	double jetEt = m_jet.et ();
	if (jetEt > m_cut.value ()) 
	{
	    SoSeparator *sep = new SoSeparator;
	    jetsSep->addChild (sep);
	    SoMaterial *mat = new SoMaterial;
	    float rgbcomponents [4];
	    IgSbColorMap::unpack (0x66330000, rgbcomponents);
	    mat->diffuseColor.setValue (rgbcomponents);
	    sep->addChild (mat);
	    
	    double jetPhi = m_jet.phi();
	    if (jetPhi < 0) jetPhi += 2 * M_PI;
	    double jetEta = m_jet.eta ();
	    
	    QString label = QString ("Et = %1 GeV")
			    .arg (jetEt, 0, 'g', 2);
	    SoSeparator *labelSep = new SoSeparator;
	    SoText2  *jetLabel = new SoText2;
	    jetLabel->string = label.latin1 ();
	    SoTranslation *jetLabelTranslation = new SoTranslation;
	    
	    jetLabelTranslation->translation = SbVec3f (jetPhi, jetEt / m_scale.value () + 0.01, jetEta);
	    
	    labelSep->addChild (jetLabelTranslation);
	    labelSep->addChild (jetLabel);
	    sep->addChild (labelSep);
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

    rep->node ()->addChild (contents);
}

void
VisGenJetTwig::update (IgRPhiRep *rep) 
{}

void
VisGenJetTwig::update (IgRZRep *rep) 
{}

