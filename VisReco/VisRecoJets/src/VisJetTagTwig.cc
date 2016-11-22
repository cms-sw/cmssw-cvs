//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisRecoJets/interface/VisJetTagTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/TypeID.h"
#include "Iguana/Inventor/interface/IgSoJet.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <qstring.h>
#include <classlib/utils/Error.h>
#include <exception>
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
	return new VisJetTagTwig (state, parent, "Jet Tag collection (" + name + ")", 
				  friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisJetTagTwig::VisJetTagTwig (IgState *state, IgTwig *parent,
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
      m_processName (processName)
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }	
    edm::TypeID jetTagID (typeid (reco::JetTagCollection));
    tfService->registerTwig (jetTagID.friendlyClassName (), &createThisTwig);
}

void
VisJetTagTwig::onNewEvent (const edm::Event &event,
			   const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_text = QString ("Run # %1, event # %2")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ()).latin1 ();
    
    std::vector<edm::Handle<reco::JetTagCollection> > jets;

    try 
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, jets);
	}
// 	else
// 	{
// 	    event.getManyByType (jets);
// 	}
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
    
    m_jetTags.clear ();

    if (! jets.empty ()) 
    {
	std::vector<edm::Handle<reco::JetTagCollection> >::iterator i;
	for (i = jets.begin (); i != jets.end (); i++) 
	{
	    const reco::JetTagCollection& c = *(*i);
	
	    for (reco::JetTagCollection::const_iterator j = c.begin (); j != c.end (); j++) 
	    {
		//const reco::CaloJet *caloJet = dynamic_cast<const reco::CaloJet*>(&(j->jet ()));
		const reco::CaloJet caloJet = *(j->first.castTo<reco::CaloJetRef>());
// 		const edm::RefVector<reco::TrackCollection> tc = j->tracks ();
// 		std::pair< reco::CaloJet, edm::RefVector<reco::TrackCollection> > p;
// 		//p.first = (*caloJet);
// 		p.first = caloJet;
// 		p.second = tc;
		
		m_jetTags.push_back (caloJet);		
	    }
	}
    }
    
    VisQueuedTwig::onBaseInvalidate ();
}

void
VisJetTagTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;

    text << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>P<SUB>T (GeV)</TH>"
	 << "<TH>eta</TH>"
	 << "<TH>phi [rad]</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
    
    int nJets = 0;
      
    for (std::vector<reco::CaloJet>::const_iterator it = m_jetTags.begin (), itEnd = m_jetTags.end (); 
	 it != itEnd; ++it)
    {
 	double jetEt = (*it).pt ();
	double jetPhi = (*it).phi();
	double jetEta = (*it).eta ();
	
	text << "<TR align = right>"
	     << "<TD>" << std::setw (3) << nJets++ << "</TD>"
	     << "<TD>" << std::setw (2) << std::setprecision (3) << jetEt << "</TD>"
	     << "<TD>" << std::setw (6) << std::setprecision (3) << jetEta << "</TD>"
	     << "<TD>" << std::setw (5) << std::setprecision (3) << jetPhi << "</TD>"
	     << "</TR>";
    }
       
    text << "</table>";
    
    // Send it over.
    rep->setText (text.str ());
}

/** Update object */
void
VisJetTagTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    SoGroup *contents = rep->node ();
    contents->removeAllChildren ();
    
    SoSeparator *jetsSep = new SoSeparator;
    contents->addChild (jetsSep);
    
    SoSeparator *labelSep = new SoSeparator;
    contents->addChild (labelSep);
    
    try 
    {
	unsigned colour [10] = { 0xff000000, 0x00ff0000, 0xffff0000, // red green yellow
				 0xff00ff00, 0x00ffff00, 0xd0dfff00, // magenta cyan
				 0xff000fff, 0x00ffff00, 0xe00fff00,
				 0xff00ee00};
	int i = 0;
	
	for (std::vector<reco::CaloJet>::const_iterator it = m_jetTags.begin (), itEnd = m_jetTags.end (); 
	     it != itEnd; ++it)
	{
	    if (i == 10) i = 0;
	    
	    SoSeparator *sep = new SoSeparator;
	    jetsSep->addChild (sep);
	    SoMaterial *mat = new SoMaterial;
	    float rgbcomponents [4];
	    IgSbColorMap::unpack (colour [i++], rgbcomponents);
	    mat->diffuseColor.setValue (rgbcomponents);
	    sep->addChild (mat);

	    SoFont *font = new SoFont;
	    font->name = "Times-Roman";
	    font->size = 14.0;
	    sep->addChild (font);
	    
	    double jetEt = (*it).pt ();
	    double jetPhi = (*it).phi();
            if (jetPhi < 0) jetPhi += 2 * M_PI;
	    double jetEta = (*it).eta ();
	    double jetTheta = 2.0 * atan (exp (-jetEta));
	    
	    QString label = QString ("Pt = %1 GeV")
			    .arg (jetEt, 0, 'f', 2);
	    SoSeparator *labelSep = new SoSeparator;
	    SoText2 *jetLabel = new SoText2;
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
    catch (...)
    {   
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (state ()))
	{
	    sbar->setMessage ("EXCEPTION VisJetTagTwig::update (Ig3DRep *rep) : Cannot show JetTags.");
	}
    }
}

/** Update object */
void
VisJetTagTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *jetsSep = new SoSeparator;
    rep->node ()->addChild (jetsSep);
    
    SoSeparator *labelSep = new SoSeparator;
    rep->node ()->addChild (labelSep);
    
    try 
    {
	unsigned colour [10] = { 0xff000000, 0x00ff0000, 0xffff0000, // red green yellow 
				 0xff00ff00, 0x00ffff00, 0xd0dfff00, // magenta cyan
				 0xff000fff, 0x00ffff00, 0xe00fff00,
				 0xff00ee00};
	int i = 0;
	
	for (std::vector<reco::CaloJet>::const_iterator it = m_jetTags.begin (), itEnd = m_jetTags.end (); 
	     it != itEnd; ++it)
	{
	    if (i == 10) i = 0;
	    
	    SoSeparator *sep = new SoSeparator;
	    jetsSep->addChild (sep);
	    SoMaterial *mat = new SoMaterial;
	    float rgbcomponents [4];
	    IgSbColorMap::unpack (colour [i++], rgbcomponents);
	    mat->diffuseColor.setValue (rgbcomponents);
	    sep->addChild (mat);

	    SoFont *font = new SoFont;
	    font->name = "Times-Roman";
	    font->size = 14.0;
	    sep->addChild (font);
	    
	    double jetPhi = (*it).phi();
	    if (jetPhi < 0) jetPhi += 2 * M_PI;
	    double jetEt = (*it).pt ();
	    double jetEta = (*it).eta ();
	    
	    QString label = QString ("Pt = %1 GeV")
			    .arg (jetEt, 0, 'f', 2);
	    SoSeparator *labelSep = new SoSeparator;
	    SoText2  *jetLabel = new SoText2;
	    jetLabel->string = label.latin1 ();
	    SoTranslation *jetLabelTranslation = new SoTranslation;
	    double jetLabelHight = 0.0;

// 	    std::vector<CaloTowerDetId> caloTowers = (*it).getTowerIndices ();
	    
// 	    for (std::vector<CaloTowerDetId>::const_iterator j = caloTowers.begin (); 
// 		 j != caloTowers.end (); j++) 
// 	    {
// 		for (std::vector <std::pair < CaloTowerDetId, CaloTower> >::const_iterator i = m_towers.begin ();
// 		     i != m_towers.end (); i++)
// 		{
// 		    if ((*i).first == (*j))
// 		    {
// 			double eta = (*i).second.eta ();
// 			double phi = (*i).second.phi ();
// 			if (phi < 0) phi += 2 * M_PI;
// 			double et = (*i).second.et ();
// 			et > jetLabelHight ? jetLabelHight = et : jetLabelHight;
			
// 			SoSeparator *tsep = new SoSeparator;
		    
// 			SoTranslation *localTransl = new SoTranslation;
// 			localTransl->translation.setValue (SbVec3f (phi, et / (2 * 20.0), eta)); // m_scale.value ()
		    
// 			SoCube *towerBox = new SoCube;
// 			towerBox->height = et / 20.0 + 0.001; // m_scale.value ()
// 			if (fabs(eta) > 1.74) towerBox->width = 0.176f;
// 			else towerBox->width  = 0.089;
// 			towerBox->depth  = VisHcalGrid::phi4eta (fabs(eta)) + 0.002;
		    
// 			tsep->addChild (localTransl);
// 			tsep->addChild (towerBox);
// 			sep->addChild (tsep);
// 		    }
//                 } 
// 	    }
	    jetLabelHight == 0 ? jetLabelHight = jetEta / 20.0 + 0.2 : jetLabelHight = jetLabelHight / 20.0 + 0.2;
	    
	    jetLabelTranslation->translation = SbVec3f (jetPhi, jetLabelHight, jetEta); //m_scale.value ()
	    
	    labelSep->addChild (jetLabelTranslation);
	    labelSep->addChild (jetLabel);
	    sep->addChild (labelSep);
	}
    }
    
    catch (...)
    {   
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (state ()))
	{
	    sbar->setMessage ("EXCEPTION in VisJetTagTwig::update (IgLegoRep *rep): Cannot show JetTags.");
	}
    }    
}

void
VisJetTagTwig::update (IgRPhiRep *rep) 
{}

void
VisJetTagTwig::update (IgRZRep *rep) 
{}

