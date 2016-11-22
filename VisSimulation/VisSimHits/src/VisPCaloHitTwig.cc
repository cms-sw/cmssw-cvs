//<<<<<< INCLUDES                                                       >>>>>>

#include "VisSimulation/VisSimHits/interface/VisPCaloHitTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "VisFramework/VisEventSetup/interface/VisSubdetector.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Inventor/interface/IgSoCircularHist.h"
#include "Iguana/Inventor/interface/IgSoCrystalHit.h"
#include "Iguana/Inventor/interface/IgSoRZHist.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgQtLock.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <qstring.h>
#include <iostream>
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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Simulated Data/Sim Calo Hits");
	
	if (! eventTwig) 
	    eventTwig = parent;
		
	return new VisPCaloHitTwig (state, eventTwig, "[N/A] PCaloHit (" + name + ")", friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisPCaloHitTwig::VisPCaloHitTwig (IgState *state, IgTwig *parent, const std::string &name /* = ""*/,
				  const std::string &friendlyName /* = ""*/,
				  const std::string &moduleLabel /* = ""*/,
				  const std::string &instanceName /* = ""*/,
				  const std::string &processName /* = "" */)
    : VisQueuedTwig (state, parent, name),
      m_text (name),
      m_friendlyName (friendlyName),
      m_moduleLabel (moduleLabel),
      m_instanceName (instanceName),
      m_processName (processName),
      m_minEnergy (0.1)
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID caloJetsID (typeid (edm::PCaloHitContainer));
    tfService->registerTwig (caloJetsID.friendlyClassName (), &createThisTwig);
    
    const edm::ParameterSet *pset = VisConfigurationService::pSet ();
    if (pset)
    {
	m_minEnergy = pset->getUntrackedParameter<double> ("VisPCaloHitTwig_minEnergy", 0.1);
    }
}

void
VisPCaloHitTwig::onNewEvent (const edm::Event &event,
			     const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_hits.clear ();
    m_text = (QString ("Run %1, Event %2, LS %3, Orbit %4, BX %5")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ())
	      .arg (event.luminosityBlock ())
	      .arg (event.orbitNumber ())
	      .arg (event.bunchCrossing ())
	      .latin1 ());

    std::vector<edm::Handle<edm::PCaloHitContainer> > hitCollections;
    
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, hitCollections);
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisPCaloHitTwig::onNewEvent ");
	e.append (this->name ());    

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

    if (! hitCollections.empty ())
    {
	for (std::vector<edm::Handle<edm::PCaloHitContainer> >::iterator i = hitCollections.begin (), iEnd = hitCollections.end (); i != iEnd; ++i) 
	{
	    const edm::PCaloHitContainer& c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    
	    this->name (nameStr.toStdString ());

	    for (std::vector<PCaloHit>::const_iterator isim = c.begin (), isimEnd = c.end ();
		 isim != isimEnd; ++isim)
	    {
		m_hits.push_back ((*isim));
	    }    
	}
    }

    IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisPCaloHitTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "Total " << m_hits.size () << " hits from ";
    text << m_text << "<br>";

    text << "Above 0.1 GeV:<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Energy</TH>"
	 << "<TH>Time of deposit</TH>"
	 << "<TH>DetID</TH>"
	 << "<TH>Track ID</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if ((! m_hits.empty ())) 
    {
	int nHits = 0;
	try 
	{
	    for (std::vector<PCaloHit>::const_iterator i = m_hits.begin (); i != m_hits.end (); ++i) 
	    {
		if ((*i).energy () >= m_minEnergy)
		{
		    text << "<TR align = right>"
			 << "<TD>" << std::setw (3) << nHits++ << "</TD>"
			 << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).energy () << "</TD>"
			 << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).time () << "</TD>"
			 << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).id () << "</TD>"
			 << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).geantTrackId () << "</TD></TR>";
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
	text << "</table>";
    }

    // Send it over.
    IgQtLock ();
    rep->setText (text.str ());
}

void
VisPCaloHitTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    IgQtLock ();
    rep->clear ();

    if (! m_hits.empty ())
    {
	SoSeparator *sep = new SoSeparator; // global

	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (0.0, 0.0, 1.0);
	sep->addChild (mat);

	try 
	{
	    VisEventSetupService *esService = VisEventSetupService::get (state ());
	    ASSERT (esService);
	    
	    for (std::vector<PCaloHit>::const_iterator i = m_hits.begin (); i != m_hits.end (); ++i) 
	    {
		double energy = (*i).energy ();

		if (energy >= m_minEnergy) 
		{
		    const DetId detUnitId ((*i).id ());

		    const CaloCellGeometry::CornersVec& corners = esService->getCorners (detUnitId);
		    ASSERT (corners.size () == 8);

		    IgSoCrystalHit *crystalHit = new IgSoCrystalHit;
		    crystalHit->energy.setValue (energy);
		    crystalHit->scale.setValue (1.0);
		    crystalHit->relativeWidth.setValue (1.0);
		    crystalHit->drawCrystal.setValue (true);
		    crystalHit->drawHit.setValue (true);

		    switch (detUnitId.det ()) 
		    {
		    case DetId::Ecal:
// 			// surface_style_15
// 			mat->ambientColor.setValue (0.115997, 0.111996, 0.109998);
// 			mat->diffuseColor.setValue (0.521988, 0.503984, 0.494989);
// 			mat->specularColor.setValue (0.599991, 0.599991, 0.599991);
		    
			crystalHit->front1.setValue (corners [3].x () / 100.0, corners [3].y () / 100.0, corners [3].z () / 100.0);
			crystalHit->front2.setValue (corners [2].x () / 100.0, corners [2].y () / 100.0, corners [2].z () / 100.0);
			crystalHit->front3.setValue (corners [1].x () / 100.0, corners [1].y () / 100.0, corners [1].z () / 100.0);
			crystalHit->front4.setValue (corners [0].x () / 100.0, corners [0].y () / 100.0, corners [0].z () / 100.0);
		    
			crystalHit->back1.setValue  (corners [7].x () / 100.0, corners [7].y () / 100.0, corners [7].z () / 100.0);
			crystalHit->back2.setValue  (corners [6].x () / 100.0, corners [6].y () / 100.0, corners [6].z () / 100.0);
			crystalHit->back3.setValue  (corners [5].x () / 100.0, corners [5].y () / 100.0, corners [5].z () / 100.0);
			crystalHit->back4.setValue  (corners [4].x () / 100.0, corners [4].y () / 100.0, corners [4].z () / 100.0);

			break;
		    case DetId::Hcal:
// 			// style 16
// 			mat->ambientColor.setValue (0.449982, 0.429993, 0.36998);
// 			mat->diffuseColor.setValue (0.337486, 0.322495, 0.277485);
// 			mat->specularColor.setValue (0.499992, 0.499992, 0.499992);
		    
			crystalHit->front1.setValue (corners [0].x () / 100.0, corners [0].y () / 100.0, corners [0].z () / 100.0);
			crystalHit->front2.setValue (corners [1].x () / 100.0, corners [1].y () / 100.0, corners [1].z () / 100.0);
			crystalHit->front3.setValue (corners [2].x () / 100.0, corners [2].y () / 100.0, corners [2].z () / 100.0);
			crystalHit->front4.setValue (corners [3].x () / 100.0, corners [3].y () / 100.0, corners [3].z () / 100.0);
		    
			crystalHit->back1.setValue  (corners [4].x () / 100.0, corners [4].y () / 100.0, corners [4].z () / 100.0);
			crystalHit->back2.setValue  (corners [5].x () / 100.0, corners [5].y () / 100.0, corners [5].z () / 100.0);
			crystalHit->back3.setValue  (corners [6].x () / 100.0, corners [6].y () / 100.0, corners [6].z () / 100.0);
			crystalHit->back4.setValue  (corners [7].x () / 100.0, corners [7].y () / 100.0, corners [7].z () / 100.0);

			break;
		    case DetId::Calo:
			// type [DetId::Calo] = "Calo";
			break;
		    default:
			break;
		    }
		    sep->addChild (crystalHit);
		}
	    }	    
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisPCaloHitTwig::update(Ig3DRep*) ");
	    e.append (this->name ());    

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
	rep->node ()->addChild (sep);
    }
}

void
VisPCaloHitTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

void
VisPCaloHitTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    IgQtLock ();

    rep->clear ();
    
    if (! m_hits.empty ())
    {
	int nbrOfBins = 0;
	int binNumber;
	double radius = 0;
	float layer = 0;

	try 
	{
	    VisEventSetupService *esService = VisEventSetupService::get (state ());
	    ASSERT (esService);
	    
	    std::vector<PCaloHit>::const_iterator i = m_hits.begin ();
	    
	    // create a DetId from the detUnitId
	    const DetId detUnitId ((*i).id ());

	    if ((VisSubdetector::detName (detUnitId.det ()) == "Hcal" && VisSubdetector::subDetName (detUnitId.det (), detUnitId.subdetId ()) == "HcalBarrel") ||
		(VisSubdetector::detName (detUnitId.det ()) == "Ecal" && VisSubdetector::subDetName (detUnitId.det (), detUnitId.subdetId ()) == "EcalBarrel"))
	    {	
		SoSeparator *sep = new SoSeparator;    

		if (VisSubdetector::detName (detUnitId.det ()) == "Hcal" && VisSubdetector::subDetName (detUnitId.det (), detUnitId.subdetId ()) == "HcalBarrel")
		{	    
		    SoMaterial *mat = new  SoMaterial;
		    mat->diffuseColor.setValue (0.6, 0.2, 0.8); // Medium Violet Mangenta
		    sep->addChild (mat);
		
		    nbrOfBins = 72;
		    radius = 1.9;
		    layer = 10.0;
		}
		else if (VisSubdetector::detName (detUnitId.det ()) == "Ecal" && VisSubdetector::subDetName (detUnitId.det (), detUnitId.subdetId ()) == "EcalBarrel")
		{
		    SoMaterial *mat = new  SoMaterial;
		    mat->diffuseColor.setValue (1.0 , 0.0, 0.6);
		    sep->addChild (mat);

		    nbrOfBins = 360;
		    radius = 1.3;
		    layer = 11.0;
		}

		std::vector<float> bufferPositive (nbrOfBins);

		std::cout << VisSubdetector::detName (detUnitId.det ()) << VisSubdetector::subDetName (detUnitId.det (), detUnitId.subdetId ()) << std::endl;
	    
		for (i = m_hits.begin (); i != m_hits.end (); ++i) 
		{
		    double energy = (*i).energy ();

		    if (energy >= m_minEnergy && nbrOfBins > 0) 
		    {
			// create a DetId from the detUnitId
			const DetId detUnitId ((*i).id ());
			GlobalPoint pos = esService->getCellPosition (detUnitId);
			if (! radius)
			{
			    radius = pos.mag () / 100.0;
			}

			double phi = pos.phi ();
			binNumber = (int) floor (phi / (2 * M_PI / nbrOfBins));

			//(phi < 0) ? phi = 2 * M_PI + phi : phi;
			if (energy >= 0.0)
			    bufferPositive [binNumber] += energy;
		    }
		}
		
		if (*max_element (bufferPositive.begin (), bufferPositive.end ()) > 0.0)
		{
		    IgSoCircularHist *recHits = new IgSoCircularHist;
		    recHits->minRadius = radius;
		    recHits->maxRadius = -1;
		    recHits->scaleFactor = 1.0;
		    recHits->numberOfBins = nbrOfBins;
		    recHits->energies.setValues (0, nbrOfBins, &bufferPositive [0]);
		    recHits->logScale = false;
		    recHits->showAnnotations = true;
		    recHits->layer = layer;
		    sep->addChild (recHits);
		}	
		rep->node ()->addChild (sep);
	    }    
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisPCaloHitTwig::update(Ig3DRep*) ");
	    e.append (this->name ());    

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
}

void
VisPCaloHitTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    IgQtLock ();

    rep->clear ();
    
    if (! m_hits.empty ())
    {
	SoSeparator *sep = new SoSeparator;    
	SoSeparator *posSep = new SoSeparator;    
	SoSeparator *negSep = new SoSeparator;
	sep->addChild (posSep);
	sep->addChild (negSep);
    
	SoMaterial *mat = new SoMaterial;
	posSep->addChild (mat);
    
	SoMaterial *negMat = new SoMaterial;
	negMat->diffuseColor.setValue (51.0 / 255.0, 1.0, 102.0 / 255.0); // Light Green Teal
	negSep->addChild (negMat);

	int nbrOfBins = 0;
	int binNumber;
	double radius = 0;
	float etaMax = 1.479;
	float deltaEta = 0.087;
	float layer = 0;
	
	const DetId detID (m_hits [0].id ());
	
	if (VisSubdetector::detName (detID.det ()) == "Hcal" && VisSubdetector::subDetName (detID.det (), detID.subdetId ()) == "HcalBarrel")
	{	    
	    mat->diffuseColor.setValue (153.0 / 255.0, 51.0 / 255.0, 204.0 / 255.0); // Medium Violet Mangenta
	    nbrOfBins = 2 * 16;
	    etaMax = 1.305;
	    radius = 1.9;
	    layer = -5.0;
	}
	else if (VisSubdetector::detName (detID.det ()) == "Ecal" && VisSubdetector::subDetName (detID.det (), detID.subdetId ()) == "EcalBarrel")
	{
	    mat->diffuseColor.setValue (1.0 , 0.0, 153.0 / 255.0);
	    nbrOfBins = 2 * 17 * 5;
	    etaMax = 1.479; // max eta of ECAL Barrel	
	    deltaEta = 1.521 / 18.0 / 6.0;
	    radius = 1.280;
	    layer = -5.0;
	}

	std::vector<float> bufferUpperPositive (nbrOfBins);
	std::vector<float> bufferUpperNegative (nbrOfBins);
	std::vector<float> bufferLowerPositive (nbrOfBins);
	std::vector<float> bufferLowerNegative (nbrOfBins);

	try 
	{
	    VisEventSetupService *esService = VisEventSetupService::get (state ());
	    ASSERT (esService);

	    if (nbrOfBins > 0) 
	    {		
		for (std::vector<PCaloHit>::const_iterator i = m_hits.begin (); i != m_hits.end (); ++i) 
		{
		    double energy = (*i).energy ();

		    if (energy >= m_minEnergy) 
		    {
			// create a DetId from the detUnitId
			const DetId detUnitId ((*i).id ());

			double energy = (*i).energy () * 100.0;
			GlobalPoint pos = esService->getCellPosition (detUnitId);
			if (! radius)
			{
			    radius = pos.mag () / 100.0;
			}

			double eta = pos.eta ();
			double y = pos.y ();		    

			// if the hit is in the barrel 
			if (! ((eta < -etaMax) || (eta > etaMax)))
			{
			    if (eta <= 0.0) 
			    {
				binNumber = nbrOfBins - 1 - (int) floor ((etaMax - eta) / deltaEta);
			    }
			    else 
			    {
				binNumber = nbrOfBins - 1 - (int) (nbrOfBins / 2 + floor (eta / deltaEta));
			    }
			
			    if (energy >= 0)
			    {
				if (y >= 0)
				{
				    bufferUpperPositive [binNumber] += energy;
				}
				else
				{
				    bufferLowerPositive [binNumber] += energy;
				}
			    }
			    else
			    {
				if (y >= 0)
				{
				    bufferUpperNegative [binNumber] -= energy;
				}
				else
				{
				    bufferLowerNegative [binNumber] -= energy;
				}                   
			    }               
			}
		    }
		}

		std::vector<float>::const_iterator first = bufferUpperPositive.begin ();
		std::vector<float>::const_iterator last = bufferUpperPositive.end ();
	    
		if (*max_element (first, last) > 0.0)
		{
		    IgSoRZHist *upper = new IgSoRZHist;
		    upper->minRadius = radius;
		    upper->maxRadius = -1;
		    upper->energies.setValues (0, nbrOfBins, &bufferUpperPositive [0]);
		    upper->logScale = false;
		    upper->showAnnotations = false;
		    upper->layer = layer;
		    upper->upper = true;
		    upper->deltaEta = deltaEta;
		    posSep->addChild (upper);
		}
	    
		std::vector<float> ::const_iterator nfirst = bufferUpperNegative.begin ();
		std::vector<float> ::const_iterator nlast = bufferUpperNegative.end ();
	    
		if (*max_element (nfirst, nlast) > 0.0)
		{
		    IgSoRZHist *lower = new IgSoRZHist;
		    lower->minRadius = radius;
		    lower->maxRadius = -1;
		    lower->energies.setValues (0, nbrOfBins, &bufferUpperNegative [0]);
		    lower->logScale = false;
		    lower->showAnnotations = false;
		    lower->layer = layer;
		    lower->upper = false;	
		    lower->deltaEta = deltaEta;
		    negSep->addChild (lower);
		}

		std::vector<float>::const_iterator lfirst = bufferLowerPositive.begin ();
		std::vector<float>::const_iterator llast = bufferLowerPositive.end ();
	    
		if (*max_element (lfirst, llast) > 0.0)
		{
		    IgSoRZHist *upperNeg = new IgSoRZHist;
		    upperNeg->minRadius = radius;
		    upperNeg->maxRadius = -1;
		    upperNeg->energies.setValues (0, nbrOfBins, &bufferLowerPositive [0]);
		    upperNeg->logScale = false;
		    upperNeg->showAnnotations = false;
		    upperNeg->layer = layer;
		    upperNeg->upper = true;	
		    upperNeg->deltaEta = deltaEta;
		    posSep->addChild (upperNeg);
		}
	    
		std::vector<float> ::const_iterator lnfirst = bufferLowerNegative.begin ();
		std::vector<float> ::const_iterator lnlast = bufferLowerNegative.end ();
	    
		if (*max_element (lnfirst, lnlast) > 0.0)
		{
		    IgSoRZHist *lowerNeg = new IgSoRZHist;
		    lowerNeg->minRadius = radius;
		    lowerNeg->maxRadius = -1;
		    lowerNeg->energies.setValues (0, nbrOfBins, &bufferLowerNegative [0]);
		    lowerNeg->logScale = false;
		    lowerNeg->showAnnotations = false;
		    lowerNeg->layer = layer;	
		    lowerNeg->upper = false;	
		    lowerNeg->deltaEta = deltaEta;
		    negSep->addChild (lowerNeg);
		}
	    }	    
	}
	catch (...) 
	{
	    std::cout << "No PCaloHits." << std::endl;
	}
    
	rep->node ()->addChild (sep);
    }
}
