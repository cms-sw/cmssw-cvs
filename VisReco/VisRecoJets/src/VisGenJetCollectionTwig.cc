//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisRecoJets/interface/VisGenJetCollectionTwig.h"
#include "VisReco/VisRecoJets/interface/VisGenJetTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
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
#include "Iguana/Studio/interface/IgDocumentData.h"
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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/GenJets");
	
	if (! eventTwig)
	    eventTwig = parent;

	return new VisGenJetCollectionTwig (state, eventTwig, "[N/A] GenJets (" + name + ")", 
					    friendlyName, modLabel, instanceName, processName);
    }
}
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisGenJetCollectionTwig::VisGenJetCollectionTwig (IgState *state, IgTwig *parent,
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
      m_scale (state, lat::CreateCallback (this, &VisGenJetCollectionTwig::scaleChanged)),
      m_cut (state, lat::CreateCallback (this, &VisGenJetCollectionTwig::scaleChanged)),
      m_escale (state, lat::CreateCallback (this, &VisGenJetCollectionTwig::scaleChanged)),
      m_annotation (state, lat::CreateCallback (this, &VisGenJetCollectionTwig::scaleChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }	
    edm::TypeID genJetsID (typeid (reco::GenJetCollection));
    tfService->registerTwig (genJetsID.friendlyClassName (), &createThisTwig);
}

void
VisGenJetCollectionTwig::scaleChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }


void
VisGenJetCollectionTwig::onNewEvent (const edm::Event &event,
				     const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    if (children ()) destroy ();
    
    m_text = QString ("Run # %1, event # %2")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ()).latin1 ();
    
    std::vector<edm::Handle<reco::GenJetCollection> > jets;
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
    
    m_jets.clear ();

    if (! jets.empty ()) 
    {
	std::vector<edm::Handle<reco::GenJetCollection> >::iterator i;
	std::vector<edm::Handle<reco::GenJetCollection> >::iterator iEnd;
	for (i = jets.begin (), iEnd = jets.end (); i != iEnd; ++i) 
	{
	    const reco::GenJetCollection& c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    
	    this->name (nameStr.toStdString ());
	
	    for (reco::GenJetCollection::const_iterator j = c.begin (), jEnd = c.end (); j != jEnd; ++j) 
	    {
		m_jets.push_back (*j);
		QString name = QString ("GenJet with Et = %1 GeV")
			       .arg ((*j).et (), 0, 'f', 2);
		
		new VisGenJetTwig (state (), this, name.latin1 (), event, eventSetup, (*j));
	    }
	}
    }
            
    IgRepSet::invalidate (this->parent (), IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisGenJetCollectionTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;

    text << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
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
    
    int nJets = 0;
    if (! m_jets.empty ()) 
    {
	try 
	{
	    for (reco::GenJetCollection::const_iterator it = m_jets.begin (), itEnd = m_jets.end (); 
		 it != itEnd; ++it)
	    {
		double jetEt = (*it).et ();
		if (jetEt > m_cut.value ())
		{
		    double eEm = (*it).emEnergy ();
		    double eHad = (*it).hadEnergy ();
		    double eInv = (*it).invisibleEnergy ();
		    double eAux = (*it).auxiliaryEnergy ();
	
		    text << "<TR align = right>"
			 << "<TD>" << std::setw (3) << nJets++ << "</TD>"
			 << "<TD>" << std::setw (2) << std::setprecision (3) << jetEt << "</TD>"
			 << "<TD>" << std::setw (2) << std::setprecision (3) << eEm << "</TD>"
			 << "<TD>" << std::setw (6) << std::setprecision (3) << eHad << "</TD>"
			 << "<TD>" << std::setw (5) << std::setprecision (3) << eInv << "</TD>"
			 << "<TD>" << std::setw (5) << std::setprecision (3) << eAux << "</TD>"
			 << "<TD>" << std::setw (5) << std::setprecision (3) << (*it).eta () << "</TD>"
			 << "<TD>" << std::setw (5) << std::setprecision (3) << (*it).phi () << "</TD></TR>";
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
    
    text << "</table>";
    
    // Send it over.
    rep->setText (text.str ());
}

/** Update object */
void
VisGenJetCollectionTwig::update (Ig3DRep *rep)
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
	unsigned colour [10] = { 0xff000000, 0xffff0000, 0x00ff0000, // red yellow green
				 0xff00ff00, 0x00ffff00, 0xd0dfff00, // magenta cyan
				 0xff000fff, 0x00ffff00, 0xe00fff00,
				 0xff00ee00};
	int i = 0;
	
	for (reco::GenJetCollection::const_iterator it = m_jets.begin (); 
	     it != m_jets.end (); it++)
	{
	    if (i == 10) i = 0;
	    
	    SoSeparator *sep = new SoSeparator;
	    jetsSep->addChild (sep);
	    SoMaterial *mat = new SoMaterial;
	    float rgbcomponents [4];
	    IgSbColorMap::unpack (colour [i++], rgbcomponents);
	    mat->diffuseColor.setValue (rgbcomponents);
	    sep->addChild (mat);
	    
 	    double jetEt = (*it).et ();
 	    double jetPhi = (*it).phi();
	    if (jetPhi < 0) jetPhi += 2 * M_PI;
	    double jetEta = (*it).eta ();
 	    double jetTheta = 2.0 * atan (exp (-jetEta));
	    
 	    QString label = QString ("Et = %1 GeV")
 			    .arg (jetEt, 0, 'f', 2);
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
    catch (...)
    {   
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (state ()))
	{
	    sbar->setMessage ("EXCEPTION VisGenJetTwig::update (Ig3DRep *rep) : Cannot show GenJets.");
	}
    }
}

/** Update object */
void
VisGenJetCollectionTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    SoSeparator *contents = new SoSeparator;
    
    SoSeparator *jetsSep = new SoSeparator;
    contents->addChild (jetsSep);
    
    SoSeparator *labelSep = new SoSeparator;
    contents->addChild (labelSep);
    
    if (! m_jets.empty ()) 
    {
	try 
	{
	    unsigned colour [10] = { 0xff000000, 0xffff0000, 0x00ff0000, // red yellow green
				     0xff00ff00, 0x00ffff00, 0xd0dfff00, // magenta cyan
				     0xff000fff, 0x00ffff00, 0xe00fff00,
				     0xff00ee00};
	    int i = 0;
	
	    for (reco::GenJetCollection::const_iterator it = m_jets.begin (), itEnd = m_jets.end (); 
		 it != itEnd; ++it)
	    {
		double jetEt = (*it).et ();
		if (jetEt > m_cut.value ()) 
		{
		    if (i == 10) i = 0;
	    
		    SoSeparator *sep = new SoSeparator;
		    jetsSep->addChild (sep);
		    SoMaterial *mat = new SoMaterial;
		    float rgbcomponents [4];
		    IgSbColorMap::unpack (colour [i++], rgbcomponents);
		    mat->diffuseColor.setValue (rgbcomponents);
		    sep->addChild (mat);
	    
		    double jetPhi = (*it).phi();
		    if (jetPhi < 0) jetPhi += 2 * M_PI;
		    double jetEta = (*it).eta ();
	    
		    QString label = QString ("Et = %1 GeV")
				    .arg (jetEt, 0, 'f', 2);
		    SoSeparator *labelSep = new SoSeparator;
		    SoText2  *jetLabel = new SoText2;
		    jetLabel->string = label.latin1 ();
		    SoTranslation *jetLabelTranslation = new SoTranslation;
	    
		    jetLabelTranslation->translation = SbVec3f (jetPhi, jetEt / m_scale.value () + 0.01, jetEta);
	    
		    labelSep->addChild (jetLabelTranslation);
		    labelSep->addChild (jetLabel);
		    sep->addChild (labelSep);
	    
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
VisGenJetCollectionTwig::update (IgRPhiRep *rep) 
{}

void
VisGenJetCollectionTwig::update (IgRZRep *rep) 
{}

