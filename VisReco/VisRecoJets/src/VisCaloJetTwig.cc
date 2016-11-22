//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT

#include "VisReco/VisRecoJets/interface/VisCaloJetTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "DataFormats/Common/interface/SortedCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/TypeID.h"
#include "Iguana/Inventor/interface/IgSoCircularHist.h"
#include "Iguana/Inventor/interface/IgSoRectColHist.h"
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
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoSelection.h>
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
	IgTwig *rootTwig = IgDocumentData::get (state)->root ();
	IgTwig *eventTwig = 0;
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/CaloJets");
	
	if (! eventTwig)
	    eventTwig = parent;

	return new VisCaloJetTwig (state, eventTwig, "[N/A] CaloJet (" + name + ")", 
				   friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCaloJetTwig::VisCaloJetTwig (IgState *state, IgTwig *parent,
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
      m_binning (state, lat::CreateCallback (this, &VisCaloJetTwig::twigChanged)),
      m_annotation (state, lat::CreateCallback (this, &VisCaloJetTwig::twigChanged)),
      m_scale (state, lat::CreateCallback (this, &VisCaloJetTwig::twigChanged)),
      m_cut (state, lat::CreateCallback (this, &VisCaloJetTwig::twigChanged)),
      m_escale (state, lat::CreateCallback (this, &VisCaloJetTwig::twigChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }	
    edm::TypeID caloJetsID (typeid (reco::CaloJetCollection));
    tfService->registerTwig (caloJetsID.friendlyClassName (), &createThisTwig);
}

void
VisCaloJetTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisCaloJetTwig::onNewEvent (const edm::Event &event,
			    const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_text = QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ())
	     .arg (event.luminosityBlock ())
	     .arg (event.orbitNumber ())
	     .arg (event.bunchCrossing ())
	     .latin1 ();
    
    std::vector<edm::Handle<reco::CaloJetCollection> > jets;
//std::vector<edm::Handle<CaloTowerCollection> > towers;
    try 
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, jets);
	}
	//event.getManyByType (towers);
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
//    m_towers.clear ();

    if (! jets.empty ()) 
    {
	std::vector<edm::Handle<reco::CaloJetCollection> >::iterator i;
	std::vector<edm::Handle<reco::CaloJetCollection> >::iterator iEnd;
	for (i = jets.begin (), iEnd = jets.end (); i != iEnd; ++i) 
	{
	    const reco::CaloJetCollection& c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    
	    this->name (nameStr.toStdString ());
	
	    for (reco::CaloJetCollection::const_iterator j = c.begin (), jEnd = c.end (); j != jEnd; ++j) 
	    {
		m_jets.push_back (*j);
	    }
	}
    }
//     if (! towers.empty ())
//     {
// 	std::vector<edm::Handle<CaloTowerCollection> >::iterator i;
// 	std::vector<edm::Handle<CaloTowerCollection> >::iterator iEnd;
// 	for (i = towers.begin (), iEnd = towers.end (); i != iEnd; ++i) 
// 	{
// 	    const CaloTowerCollection& c = *(*i);
	
// 	    for (CaloTowerCollection::const_iterator j = c.begin (), jEnd = c.end (); j != jEnd; ++j) 
// 	    {
// 		std::pair <CaloTowerDetId, CaloTower> p;
// 		p.first = (*j).id ();
// 		p.second = (*j);
// 		m_towers.push_back (p);
// 	    }
// 	}
//     }
    VisQueuedTwig::onBaseInvalidate ();
}

void
VisCaloJetTwig::update (IgTextRep *rep)
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
    
    int nJets = 0;
    if (! m_jets.empty ()) 
    {
	try 
	{
	    for (std::vector< reco::CaloJet >::const_iterator it = m_jets.begin (), itEnd = m_jets.end (); 
		 it != itEnd; ++it)
	    {
		double jetEt = (*it).et ();
		if (jetEt > m_cut.value ())
		{
		    double jetPhi = (*it).phi();
		    double jetEta = (*it).eta ();
	
		    text << "<TR align = right>"
			 << "<TD>" << std::setw (3) << nJets++ << "</TD>"
			 << "<TD>" << std::setw (2) << std::setprecision (3) << jetEt << "</TD>"
			 << "<TD>" << std::setw (6) << std::setprecision (3) << jetEta << "</TD>"
			 << "<TD>" << std::setw (5) << std::setprecision (3) << jetPhi << "</TD>"
			 << "</TR>";
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
VisCaloJetTwig::update (Ig3DRep *rep)
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
	    unsigned colour [10] = { 0xff000000, 0x00ff0000, 0xffff0000, // red green yellow
				     0xff00ff00, 0x00ffff00, 0xd0dfff00, // magenta cyan
				     0xff000fff, 0x00ffff00, 0xe00fff00,
				     0xff00ee00};
	    int i = 0;
	
	    for (std::vector< reco::CaloJet >::const_iterator it = m_jets.begin (), itEnd = m_jets.end (); 
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

		    SoFont *font = new SoFont;
		    font->name = "Times-Roman";
		    font->size = 14.0;
		    sep->addChild (font);
	    
		    double jetPhi = (*it).phi();
		    (jetPhi < 0) ? jetPhi += 2 * M_PI : jetPhi;
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
VisCaloJetTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    SoSeparator *contents = new SoSeparator;
    
    SoSeparator *jetsSep = new SoSeparator;
    contents->addChild (jetsSep);
    
    SoSeparator *labelSep = new SoSeparator;
    contents->addChild (labelSep);
    
    if (! m_jets.empty ()) 
    {
	try 
	{
	    unsigned colour [10] = { 0xff000000, 0x00ff0000, 0xffff0000, // red green yellow 
				     0xff00ff00, 0x00ffff00, 0xd0dfff00, // magenta cyan
				     0xff000fff, 0x00ffff00, 0xe00fff00,
				     0xff00ee00};
	    int i = 0;
	
	    for (std::vector< reco::CaloJet >::const_iterator jit = m_jets.begin (), jitEnd = m_jets.end (); 
		 jit != jitEnd; ++jit)
	    {
		double jetEt = (*jit).et ();
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

		    SoFont *font = new SoFont;
		    font->name = "Arial";
		    font->size = 14.0;
		    sep->addChild (font);
	    
		    double jetPhi = (*jit).phi();
		    (jetPhi < 0) ? jetPhi += 2 * M_PI : jetPhi;
		    double jetEta = (*jit).eta ();
	    
		    QString label = QString ("Et = %1 GeV")
				    .arg (jetEt, 0, 'f', 2);
		    SoSeparator *labelSep = new SoSeparator;
		    SoText2  *jetLabel = new SoText2;
		    jetLabel->string = label.latin1 ();
		    SoTranslation *jetLabelTranslation = new SoTranslation;
		    double jetLabelHight = 0.0;

		    std::vector<CaloTowerDetId> caloTowerIds = (*jit).getTowerIndices ();		    
		    std::vector<CaloTowerPtr> caloTowers = (*jit).getCaloConstituents ();

		    if (! caloTowers.empty ()) 
		    {
			for (std::vector<CaloTowerPtr>::const_iterator cit = caloTowers.begin (), citEnd = caloTowers.end (); 
			     cit != citEnd; ++cit) 
			{
			    if ((*cit).isNonnull () && (*cit).isAvailable ())
			    {				
				double eta = (*cit)->eta ();
				double phi = (*cit)->phi ();
				if (phi < 0) phi += 2 * M_PI;
				double et = (*cit)->et ();
				et > jetLabelHight ? jetLabelHight = et : jetLabelHight;
			
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

		    jetLabelHight == 0 ? jetLabelHight = jetEta / m_scale.value () + 0.2 : jetLabelHight = jetLabelHight / m_scale.value () + 0.2;
	    
		    jetLabelTranslation->translation = SbVec3f (jetPhi, jetLabelHight, jetEta);
	    
		    labelSep->addChild (jetLabelTranslation);
		    labelSep->addChild (jetLabel);
		    sep->addChild (labelSep);
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
VisCaloJetTwig::update (IgRPhiRep *rep) 
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    SoSeparator *sep = new SoSeparator;    
    SoSeparator *emSep = new SoSeparator;    
    SoSeparator *hadSep = new SoSeparator;
    sep->addChild (emSep);
    sep->addChild (hadSep);

    SoFont *font = new SoFont;
    font->name = "Arial";
    font->size = 14.0;
    sep->addChild (font);
    
    SoMaterial *mat = new SoMaterial;
    mat->ambientColor.setValue (0.5, 1.0, 0.5);
    mat->diffuseColor.setValue (0.5, 1.0, 0.5);
    mat->specularColor.setValue (0.5, 1.0, 0.5);
    mat->emissiveColor.setValue (0.5, 1.0, 0.5);
    emSep->addChild (mat);
    
    SoMaterial *hadMat = new SoMaterial;
    hadMat->ambientColor.setValue (0.5, 0.5, 1.0);
    hadMat->diffuseColor.setValue (0.5, 0.5, 1.0);
    hadMat->specularColor.setValue (0.5, 0.5, 1.0);
    hadMat->emissiveColor.setValue (0.5, 0.5, 1.0);
    hadSep->addChild (hadMat);

    int nbrOfBins = 72;
    int binNumber;

    std::vector<float> bufferEm (nbrOfBins);
    std::vector<float> bufferHad (nbrOfBins);
    if (! m_jets.empty ()) 
    {
	try 
	{
	    unsigned colour [10] = { 0xff000000, 0x00ff0000, 0xffff0000, // red green yellow 
				     0xff00ff00, 0x00ffff00, 0xd0dfff00, // magenta cyan
				     0xff000fff, 0x00ffff00, 0xe00fff00,
				     0xff00ee00};
	    int i = 0;
	
	    for (std::vector< reco::CaloJet >::const_iterator jit = m_jets.begin (), jitEnd = m_jets.end (); 
		 jit != jitEnd; ++jit)
	    {
		double jetEt = (*jit).et ();
		if (jetEt > m_cut.value ()) 
		{
		    if (i == 10) i = 0;
	    
		    SoSeparator *jetSep = new SoSeparator;
		    sep->addChild (jetSep);
		    SoMaterial *mat = new SoMaterial;
		    float rgbcomponents [4];
		    IgSbColorMap::unpack (colour [i++], rgbcomponents);
		    mat->diffuseColor.setValue (rgbcomponents);
		    jetSep->addChild (mat);
	    
		    double jetPhi = (*jit).phi();
		    (jetPhi < 0) ? jetPhi += 2 * M_PI : jetPhi;
		    double jetEta = (*jit).eta ();
	    
		    QString label = QString ("Et = %1 GeV")
				    .arg (jetEt, 0, 'f', 2);
		    SoSeparator *labelSep = new SoSeparator;
		    SoText2  *jetLabel = new SoText2;
		    jetLabel->string = label.latin1 ();
		    SoTranslation *jetLabelTranslation = new SoTranslation;
		    double jetLabelHight = 0.0;

		    std::vector<CaloTowerPtr> caloTowers = (*jit).getCaloConstituents ();

		    if (! caloTowers.empty ()) 
		    {
			bool doubleWidth = false;
			
			for (std::vector<CaloTowerPtr>::const_iterator cit = caloTowers.begin (), citEnd = caloTowers.end (); 
			     cit != citEnd; ++cit) 
			{
			    if ((*cit).isNonnull () && (*cit).isAvailable ())
			    {				
				double phi = (*cit)->phi ();
				if (phi < 0) phi += 2 * M_PI;

				float et = (*cit)->et ();
				float em = (*cit)->emEt ();
				float eta = (*cit)->eta ();
				int iphi = (*cit)->id ().iphi ();
				binNumber = iphi - 1;
				if ((fabs (eta) > 1.74f) && binNumber < 71)
				    doubleWidth = true;
				else 
				    doubleWidth = false;

				if (binNumber >= 0 && binNumber < 72)
				{
				    if (m_binning.value () == "project")
				    {				
					if (et > bufferHad [binNumber])
					    bufferHad [binNumber] = et;
					if (em > bufferEm [binNumber])
					    bufferEm [binNumber] = em;
					if (doubleWidth)
					{
					    if (et > bufferHad [binNumber + 1])
						bufferHad [binNumber + 1] = et;
					    if (em > bufferEm [binNumber + 1])
						bufferEm [binNumber + 1] = em;
					}				    
				    }
				    else
				    {
					bufferHad [binNumber] += et;
					bufferEm [binNumber] += em;
					if (doubleWidth)
					{
					    bufferHad [binNumber + 1] += et;
					    bufferEm [binNumber + 1] += em;
					}
				    }
				}
				et > jetLabelHight ? jetLabelHight = et : jetLabelHight;
			    }
			}
		    }
		    
		    jetLabelHight == 0 ? jetLabelHight = jetEta / m_scale.value () + 0.2 : jetLabelHight = jetLabelHight / m_scale.value () + 0.2;
	    
		    jetLabelTranslation->translation = SbVec3f (jetPhi, jetLabelHight, jetEta);
	    
		    labelSep->addChild (jetLabelTranslation);
		    labelSep->addChild (jetLabel);
		    sep->addChild (labelSep);
		}

	    }
	    if (*max_element (bufferHad.begin (), bufferHad.end ()) > 0.0)
	    {
		IgSoCircularHist *tower = new IgSoCircularHist;
		tower->minRadius = 1.9;
		tower->maxRadius = -1;
		tower->scaleFactor = m_escale.value ();
		tower->numberOfBins = nbrOfBins;
		tower->energies.setValues (0, nbrOfBins, &bufferHad [0]);
		tower->logScale = false;
		tower->showAnnotations = m_annotation.value ();
		tower->layer = 5.6;
		hadSep->addChild (tower);
	    }
	    if (*max_element (bufferEm.begin (), bufferEm.end ()) > 0.0)
	    {
		IgSoCircularHist *emtower = new IgSoCircularHist;
		emtower->minRadius = 1.9;
		emtower->maxRadius = -1;
		emtower->scaleFactor = m_escale.value ();
		emtower->numberOfBins = nbrOfBins;
		emtower->energies.setValues (0, nbrOfBins, &bufferEm [0]);
		emtower->logScale = false;
		emtower->showAnnotations = m_annotation.value ();
		emtower->layer = 5.8;
		emSep->addChild (emtower);
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
    rep->node ()->addChild (sep);
}

void
VisCaloJetTwig::update (IgRZRep *rep) 
{}

