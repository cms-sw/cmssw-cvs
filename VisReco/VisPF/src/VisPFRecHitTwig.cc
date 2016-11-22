#include "VisReco/VisPF/interface/VisPFRecHitTwig.h"

#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "Iguana/Models/interface/IgTextRep.h"

#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"

#include "Iguana/Framework/interface/IgRepSet.h"

#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoCrystalHit.h"
#include "Iguana/Inventor/interface/IgSoCircularHist.h"
#include "Iguana/Inventor/interface/IgSoRZHist.h"
#include "Iguana/Inventor/interface/IgSoTower.h"

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>

#include "Math/GenVector/PositionVector3D.h"

#include "DataFormats/ParticleFlowReco/interface/PFLayer.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"

#include <qstring.h>
#include <classlib/utils/DebugAids.h>
#include <algorithm>
#include <iostream>
#include <iomanip>

namespace 
{
    VisQueuedTwig* createThisTwig(IgState *state, IgTwig *parent,
				  const std::string &name,
				  const std::string &friendlyName,
				  const std::string &modLabel,
				  const std::string &instanceName,
				  const std::string &processName)
    {
	return new VisPFRecHitTwig(state, parent, name, friendlyName, modLabel, instanceName, processName);
    }
}

VisPFRecHitTwig::VisPFRecHitTwig(IgState *state, IgTwig *parent, 
				   const std::string &name,
				   const std::string &friendlyName, 
				   const std::string &moduleLabel, 
				   const std::string &instanceName, 
				   const std::string &processName) 
    : VisQueuedTwig(state, parent, name), 
      m_text("No Info"),
      m_name(name),
      m_friendlyName(friendlyName),
      m_moduleLabel(moduleLabel),
      m_instanceName(instanceName),
      m_processName(processName)
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get(state);
    
    if (! tfService)
    {
	tfService = new VisTwigFactroyService(state);
    }
	
    edm::TypeID pfRecHitsID(typeid (reco::PFRecHitCollection));
    
    tfService->registerTwig(pfRecHitsID.friendlyClassName(), &createThisTwig);
}

void VisPFRecHitTwig::onNewEvent(const edm::Event &event,
				  const edm::EventSetup &eventSetup)
{
    VisQueuedTwig::onNewEvent(event, eventSetup);

    m_EBrechits.clear();
    m_HBrechits.clear();
    m_EErechits.clear();
    
    std::vector<edm::Handle<reco::PFRecHitCollection> > rechitCollections;

    try
    {
	eventSetup.get<IdealGeometryRecord>().get(m_pDD);
    }

    catch (cms::Exception& e)
    {
	if (this->m_onCmsException)
	    this->m_onCmsException(&e);
    }

    catch (lat::Error &e) 
    {
	if (this->m_onError)
	    this->m_onError(&e);
    }

    catch (std::exception &e) 
    {
	if (this->m_onException)
	    this->m_onException(&e);
    }

    catch (...) 
    {
	if (this->m_onUnhandledException)
	    this->m_onUnhandledException();
    }

    try
    {
	if ( (! m_friendlyName.empty ()) || 
	     (! m_moduleLabel.empty ())  || 
	     (! m_instanceName.empty ()) || 
	     (! m_processName.empty ()) )
	{
	    VisEventSelector visSel(m_friendlyName, m_moduleLabel, 
				    m_instanceName, m_processName);
	    
	    event.getMany(visSel, rechitCollections);
	}

	else
	{
	    event.getManyByType(rechitCollections);
	}	   
    }
    
    catch (cms::Exception& e)
    {
	if (this->m_onCmsException)
	    this->m_onCmsException(&e);
    }

    catch (lat::Error &e) 
    {
	if (this->m_onError)
	    this->m_onError(&e);
    }

    catch (std::exception &e) 
    {
	if (this->m_onException)
	    this->m_onException(&e);
    }

    catch (...) 
    {
	if (this->m_onUnhandledException)
	    this->m_onUnhandledException();
    }
    
    if (! rechitCollections.empty())
    {	
	for (std::vector<edm::Handle<reco::PFRecHitCollection> >::iterator i = 
		 rechitCollections.begin(), iEnd = rechitCollections.end(); 
	     i != iEnd; ++i) 
	{
	    const reco::PFRecHitCollection& c = *(*i);

	    for (std::vector<reco::PFRecHit>::const_iterator ipfrh = c.begin(), ipfrhEnd = c.end();
		 ipfrh != ipfrhEnd; ++ipfrh)
	    {
		if ( (*ipfrh).layer() == PFLayer::ECAL_BARREL )
		    m_EBrechits.push_back((*ipfrh));
		
		if ( (*ipfrh).layer() == PFLayer::ECAL_ENDCAP )
		    m_EErechits.push_back((*ipfrh));

		if ( (*ipfrh).layer() == PFLayer::HCAL_BARREL1 )
		    m_HBrechits.push_back((*ipfrh));
	    }    
	}
    }

    else
	std::cout<<"DEBUG: rechitCollections.empty()"<<std::endl;
    
    VisQueuedTwig::onBaseInvalidate();
}


void  VisPFRecHitTwig::update(IgTextRep *rep)
{
    VisQueuedTwig::update(rep);

    std::ostringstream  text;

    text << m_name << " from ";
    text << m_text << "<br>";
    text << "Total: " << m_EBrechits.size () << " EB PFRecHits.<br>";
    text << "Total: " << m_HBrechits.size () << " HB PFRecHits.<br>";
    text << "Total: " << m_EErechits.size () << " EE PFRecHits.<br>"; 
    
    text << "<table width='100%' border=1>"
         << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Detector</TH>"
	 << "<TH>Energy (GeV)</TH>"
	 << "<TH>Eta</TH>"
	 << "<TH>Phi (deg)</TH>"
         << "</TR>";
 
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    int nHits;
    
    try
    {
	std::vector<reco::PFRecHit>::const_iterator iRH;
	
	if ( ! m_EBrechits.empty() )
	{    
	    nHits = 0;
	    
	    for ( iRH  = m_EBrechits.begin();
		  iRH != m_EBrechits.end(); ++iRH)
	    {	
		double energy = (*iRH).energy();
		double eta = (*iRH).position().Eta();
		double phi = (*iRH).position().Phi();
	    
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nHits++ << "</TD>"
		     << "<TD> EB </TD>" 
		     << "<TD>" << std::setw (5) << std::setprecision (3) << energy << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << eta << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << phi << "</TD>"
		     << "</TR>";
	    }
	}

	if ( ! m_HBrechits.empty() )
	{   
	    nHits = 0;
	    
	    for ( iRH  = m_HBrechits.begin();
		  iRH != m_HBrechits.end(); ++iRH)
	    {	
		double energy = (*iRH).energy();
		double eta = (*iRH).position().Eta();
		double phi = (*iRH).position().Phi();
	    
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nHits++ << "</TD>"
		     << "<TD> HB </TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << energy << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << eta << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << phi << "</TD>"
		     << "</TR>";
	    }
	}
	
	if ( ! m_EErechits.empty() )
	{   
	    nHits = 0;
	    
	    for ( iRH  = m_EErechits.begin();
		  iRH != m_EErechits.end(); ++iRH)
	    {	
		double energy = (*iRH).energy();
		double eta = (*iRH).position().Eta();
		double phi = (*iRH).position().Phi();
	    
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nHits++ << "</TD>"
		     << "<TD> EE </TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << energy << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << eta << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << phi << "</TD>"
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

    text << "</table>";
   
    rep->setText(text.str());
}

void  VisPFRecHitTwig::update(Ig3DRep *rep)
{
    VisQueuedTwig::update(rep);      
    rep->clear();
    
    SoSeparator* sep = new SoSeparator;
    
    try
    {
	std::vector<reco::PFRecHit>::const_iterator iR;
	
	if ( ! m_EBrechits.empty() && m_pDD.isValid() )
	{	    
	
	    SoMaterial* EBmat = new SoMaterial;
	    EBmat->diffuseColor.setValue(1.0, 0.0, 153.0 / 255.0);
	    sep->addChild(EBmat);

	    for ( iR  = m_EBrechits.begin();
		  iR != m_EBrechits.end(); ++iR)
	    {
		const CaloCellGeometry* cell = (*m_pDD).getGeometry((*iR).detId());

		const CaloCellGeometry::CornersVec& corners = cell->getCorners();
		assert(corners.size () == 8);
		
		IgSoCrystalHit* eb_crystalHit = new IgSoCrystalHit;
		eb_crystalHit->energy.setValue((*iR).energy());		    
		eb_crystalHit->scale.setValue(1.0);
		eb_crystalHit->relativeWidth.setValue(1.0);
		eb_crystalHit->drawCrystal.setValue(true);
		eb_crystalHit->drawHit.setValue(true);
		    
		eb_crystalHit->front1.setValue(corners[3].x()/100.0, corners[3].y()/100.0, corners[3].z()/100.0);
		eb_crystalHit->front2.setValue(corners[2].x()/100.0, corners[2].y()/100.0, corners[2].z()/100.0);
		eb_crystalHit->front3.setValue(corners[1].x()/100.0, corners[1].y()/100.0, corners[1].z()/100.0);
		eb_crystalHit->front4.setValue(corners[0].x()/100.0, corners[0].y()/100.0, corners[0].z()/100.0);
		    
		eb_crystalHit->back1.setValue(corners[7].x()/100.0, corners[7].y()/100.0, corners[7].z()/100.0);
		eb_crystalHit->back2.setValue(corners[6].x()/100.0, corners[6].y()/100.0, corners[6].z()/100.0);
		eb_crystalHit->back3.setValue(corners[5].x()/100.0, corners[5].y()/100.0, corners[5].z()/100.0);
		eb_crystalHit->back4.setValue(corners[4].x()/100.0, corners[4].y()/100.0, corners[4].z()/100.0);
		    
		sep->addChild(eb_crystalHit); 
	    }	    
	}

	/*
	  This needs a closer look:
	  - One hit persists through all the events
	  - They seem to be on HO instead of HB (though it looks like they correspond to regular HBRecHits)
	  - Some lie off the end of the barrel (this seems to be because they appear to correspond to HERecHits)
	*/

	/*
	if ( ! m_HBrechits.empty() && m_pDD.isValid() )
	{ 
	    SoMaterial* HBmat = new SoMaterial;
	    HBmat->diffuseColor.setValue(0.0, 0.4, 1.0);
	    sep->addChild(HBmat);

	    for ( iR  = m_HBrechits.begin();
		  iR != m_HBrechits.end(); ++iR)
	    {
		const CaloCellGeometry* cell = (*m_pDD).getGeometry((*iR).detId());

		const CaloCellGeometry::CornersVec& corners = cell->getCorners();
		assert(corners.size () == 8);
		
		IgSoCrystalHit* hb_crystalHit = new IgSoCrystalHit;
		hb_crystalHit->energy.setValue((*iR).energy());		    
		hb_crystalHit->scale.setValue(1.0);
		hb_crystalHit->relativeWidth.setValue(0.9);
		hb_crystalHit->drawCrystal.setValue(false);
		hb_crystalHit->drawHit.setValue(true);

		hb_crystalHit->front1.setValue(corners[0].x()/100.0, corners[0].y()/100.0, corners[0].z()/100.0);
		hb_crystalHit->front2.setValue(corners[1].x()/100.0, corners[1].y()/100.0, corners[1].z()/100.0);
		hb_crystalHit->front3.setValue(corners[2].x()/100.0, corners[2].y()/100.0, corners[2].z()/100.0);
		hb_crystalHit->front4.setValue(corners[3].x()/100.0, corners[3].y()/100.0, corners[3].z()/100.0);
		    
		hb_crystalHit->back1.setValue(corners[4].x()/100.0, corners[4].y()/100.0, corners[4].z()/100.0);
		hb_crystalHit->back2.setValue(corners[5].x()/100.0, corners[5].y()/100.0, corners[5].z()/100.0);
		hb_crystalHit->back3.setValue(corners[6].x()/100.0, corners[6].y()/100.0, corners[6].z()/100.0);
		hb_crystalHit->back4.setValue(corners[7].x()/100.0, corners[7].y()/100.0, corners[7].z()/100.0);
		    
		sep->addChild(hb_crystalHit); 
	    }	    
	}
	*/

	if ( ! m_EErechits.empty() )
	{   
	    SoMaterial* EEmat = new SoMaterial;
	    EEmat->diffuseColor.setValue(1.0, 0.0, 153.0 / 255.0);
	    sep->addChild(EEmat);

	    for ( iR  = m_EErechits.begin();
		  iR != m_EErechits.end(); ++iR)
	    {
		const CaloCellGeometry* cell = (*m_pDD).getGeometry((*iR).detId());

		const CaloCellGeometry::CornersVec& corners = cell->getCorners();
		assert(corners.size () == 8);
		
		IgSoCrystalHit* ee_crystalHit = new IgSoCrystalHit;
		ee_crystalHit->energy.setValue((*iR).energy());		    
		ee_crystalHit->scale.setValue(1.0);
		ee_crystalHit->relativeWidth.setValue(0.9);
		ee_crystalHit->drawCrystal.setValue(false);
		ee_crystalHit->drawHit.setValue(true);

		ee_crystalHit->front1.setValue(corners[3].x()/100.0, corners[3].y()/100.0, corners[3].z()/100.0);
		ee_crystalHit->front2.setValue(corners[2].x()/100.0, corners[2].y()/100.0, corners[2].z()/100.0);
		ee_crystalHit->front3.setValue(corners[1].x()/100.0, corners[1].y()/100.0, corners[1].z()/100.0);
		ee_crystalHit->front4.setValue(corners[0].x()/100.0, corners[0].y()/100.0, corners[0].z()/100.0);
		    
		ee_crystalHit->back1.setValue(corners[7].x()/100.0, corners[7].y()/100.0, corners[7].z()/100.0);
		ee_crystalHit->back2.setValue(corners[6].x()/100.0, corners[6].y()/100.0, corners[6].z()/100.0);
		ee_crystalHit->back3.setValue(corners[5].x()/100.0, corners[5].y()/100.0, corners[5].z()/100.0);
		ee_crystalHit->back4.setValue(corners[4].x()/100.0, corners[4].y()/100.0, corners[4].z()/100.0);
		    
		sep->addChild(ee_crystalHit); 
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

    rep->node ()->addChild (sep);
 
}

void  VisPFRecHitTwig::update(IgLegoRep *rep)
{
    VisQueuedTwig::update(rep);
    rep->clear();
    
    SoSeparator* sep = new SoSeparator();
    
    try
    {
	if ( ! m_EBrechits.empty() )
	{
	    std::vector<reco::PFRecHit>::const_iterator iRH;
	    
	    for ( iRH  = m_EBrechits.begin();
		  iRH != m_EBrechits.end(); ++iRH)
	    {
		IgSoTower* tower = new IgSoTower();
		
		double energy = (*iRH).energy();
		tower->energy = energy;
		 
		double eta = (*iRH).position().Eta();
		double phi = (*iRH).position().Phi();
		(phi < 0.0) ? phi = 2 * M_PI + phi : phi;
		tower->position = SbVec2f(phi, eta);
		
		tower->scaleFactor = 1.0;
		tower->etaWidth = 0.0174;
		tower->phiWidth = 0.0174;
		tower->emFraction = 1.0;

		sep->addChild(tower);
	    }   
	} 

    	/*
	if ( ! m_HBrechits.empty() )
	{ 
	    std::vector<reco::PFRecHit>::const_iterator iRH;
	    
	    for ( iRH  = m_HBrechits.begin();
		  iRH != m_HBrechits.end(); ++iRH)
	    {
		IgSoTower* tower = new IgSoTower();
		
		double energy = (*iRH).energy();
		tower->energy = energy;
		
		double eta = (*iRH).position().Eta();
		double phi = (*iRH).position().Phi();
		(phi < 0.0) ? phi = 2 * M_PI + phi : phi;
		tower->position = SbVec2f(phi, eta);
	
		tower->scaleFactor = 1.0;
		tower->emFraction = 0.0;

		sep->addChild(tower);
	    }   
	}
	*/
	if ( ! m_EErechits.empty() )
	{
	    std::vector<reco::PFRecHit>::const_iterator iRH;
	    
	    for ( iRH  = m_EErechits.begin();
		  iRH != m_EErechits.end(); ++iRH)
	    {
		IgSoTower* tower = new IgSoTower();
		
		double energy = (*iRH).energy();
		tower->energy = energy;
		
		double eta = (*iRH).position().Eta();
		double phi = (*iRH).position().Phi();
		(phi < 0.0) ? phi = 2 * M_PI + phi : phi;
		tower->position = SbVec2f(phi, eta);
	
		tower->scaleFactor = 1.0;
		tower->etaWidth = 0.0174;
		tower->phiWidth = 0.0174;
		tower->emFraction = 1.0;

		sep->addChild(tower);
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
     
    rep->node()->addChild(sep);
}


void  VisPFRecHitTwig::update(IgRPhiRep *rep)
{
    VisQueuedTwig::update(rep);
    rep->clear();

    SoSeparator* sep = new SoSeparator;    
    SoSeparator* posSep = new SoSeparator;    
    SoSeparator* negSep = new SoSeparator;
    sep->addChild(posSep);
    sep->addChild(negSep);

    int binNumber;

    try
    {
	if ( ! m_EBrechits.empty() )
	{
	    SoMaterial* posEBmat = new SoMaterial;
	    posEBmat->ambientColor.setValue(1.0, 0.0, 153.0 / 255.0);
	    posEBmat->diffuseColor.setValue(1.0, 0.0, 153.0 / 255.0);
	    posEBmat->specularColor.setValue(1.0, 0.0, 153.0 / 255.0);
	    posEBmat->emissiveColor.setValue(1.0, 0.0, 153.0 / 255.0);
	    posSep->addChild(posEBmat);
    
	    SoMaterial* negEBmat = new SoMaterial;
	    negEBmat->ambientColor.setValue(51.0 / 255.0, 1.0, 102.0 / 255.0);
	    negEBmat->diffuseColor.setValue(51.0 / 255.0, 1.0, 102.0 / 255.0);
	    negEBmat->specularColor.setValue(51.0 / 255.0, 1.0, 102.0 / 255.0);
	    negEBmat->emissiveColor.setValue(51.0 / 255.0, 1.0, 102.0 / 255.0);
	    negSep->addChild(negEBmat);

	    int nbrOfBins = 360;

	    std::vector<float> bufferPositive(nbrOfBins);
	    std::vector<float> bufferNegative(nbrOfBins);
	    
	    std::vector<reco::PFRecHit>::const_iterator iRH;
	    
	    for ( iRH  = m_EBrechits.begin();
		  iRH != m_EBrechits.end(); ++iRH)
	    {		

		double energy = (*iRH).energy();
		double phi = (*iRH).position().Phi();
		(phi < 0.0) ? phi = 2 * M_PI + phi : phi;

		binNumber = (int) floor (phi / (2 * M_PI / nbrOfBins));
 
		(energy >= 0.0) ? bufferPositive[binNumber] += energy :  bufferNegative[binNumber] += energy;
	     
	    }
	    
	    std::vector<float>::const_iterator first = bufferPositive.begin();
	    std::vector<float>::const_iterator last  = bufferPositive.end();
	    
	    if ( *max_element(first, last) > 0.0 )
	    {
		IgSoCircularHist* posEBrechits = new IgSoCircularHist;
		posEBrechits->minRadius = 1.29;
		posEBrechits->maxRadius = -1;
		posEBrechits->scaleFactor = 1.0;
		posEBrechits->numberOfBins = nbrOfBins;
		posEBrechits->energies.setValues(0, nbrOfBins, &bufferPositive[0]);
		posEBrechits->logScale = false;
		posEBrechits->layer = 5.5;
		posSep->addChild(posEBrechits);
	    }
	    
	    std::vector<float>::const_iterator nfirst = bufferNegative.begin();
	    std::vector<float>::const_iterator nlast  = bufferNegative.end();
	    
	    if ( fabs(*max_element(nfirst, nlast)) > 0.0 )
	    {
		IgSoCircularHist* negEBrechits = new IgSoCircularHist;
		negEBrechits->minRadius = 1.29;
		negEBrechits->maxRadius = -1;
		negEBrechits->scaleFactor = 1.0;
		negEBrechits->numberOfBins = nbrOfBins;
		negEBrechits->energies.setValues(0, nbrOfBins, &bufferNegative[0]);
		negEBrechits->logScale = false;
		negEBrechits->layer = 5.0;

		negSep->addChild(negEBrechits);
	    }
	}

	/*	
	if ( ! m_HBrechits.empty() )
	{
	    SoMaterial* posHBmat = new SoMaterial;
	    posHBmat->ambientColor.setValue(0.0, 0.4, 1.0);
	    posHBmat->diffuseColor.setValue(0.0, 0.4, 1.0);
	    posHBmat->specularColor.setValue(0.0, 0.4, 1.0);
	    posHBmat->emissiveColor.setValue(0.0, 0.4, 1.0);
	    posSep->addChild(posHBmat);
    
	    SoMaterial* negHBmat = new SoMaterial;
	    negHBmat->ambientColor.setValue(1.0, 0.0, 0.0);
	    negHBmat->diffuseColor.setValue(1.0, 0.0, 0.0);
	    negHBmat->specularColor.setValue(1.0, 0.0, 0.0);
	    negHBmat->emissiveColor.setValue(1.0, 0.0, 0.0);
	    negSep->addChild(negHBmat);

	    int nbrOfBins = 72;

	    std::vector<float> bufferPositive(nbrOfBins);
	    std::vector<float> bufferNegative(nbrOfBins);

	    std::vector<reco::PFRecHit>::const_iterator iRH;

	    for ( iRH  = m_HBrechits.begin();
		  iRH != m_HBrechits.end(); ++iRH)
	    {		
		double energy = (*iRH).energy();
		double phi = (*iRH).position().Phi();
		(phi < 0.0) ? phi = 2 * M_PI + phi : phi;

		binNumber = (int) floor (phi / (2 * M_PI / nbrOfBins));
 
		(energy >= 0.0) ? bufferPositive[binNumber] += energy :  bufferNegative[binNumber] += energy;
	    }
	    
	    std::vector<float>::const_iterator first = bufferPositive.begin();
	    std::vector<float>::const_iterator last  = bufferPositive.end();
	    
	    if ( *max_element(first, last) > 0.0 )
	    {
		IgSoCircularHist* posHBrechits = new IgSoCircularHist;
		posHBrechits->minRadius = 1.9;
		posHBrechits->maxRadius = -1;
		posHBrechits->scaleFactor = 1.0;
		posHBrechits->numberOfBins = nbrOfBins;
		posHBrechits->energies.setValues(0, nbrOfBins, &bufferPositive[0]);
		posHBrechits->logScale = false;
		posHBrechits->layer = 5.5;
		posSep->addChild(posHBrechits);
	    }
	    
	    std::vector<float>::const_iterator nfirst = bufferNegative.begin();
	    std::vector<float>::const_iterator nlast  = bufferNegative.end();
	    
	    if ( fabs(*max_element(nfirst, nlast)) > 0.0 )
	    {
		IgSoCircularHist* negHBrechits = new IgSoCircularHist;
		negHBrechits->minRadius = 1.9;
		negHBrechits->maxRadius = -1;
		negHBrechits->scaleFactor = 1.0;
		negHBrechits->numberOfBins = nbrOfBins;
		negHBrechits->energies.setValues(0, nbrOfBins, &bufferNegative[0]);
		negHBrechits->logScale = false;
		negHBrechits->layer = 5.0;

		negSep->addChild(negHBrechits);
	    }
	}
	*/
    }
    
    catch (cms::Exception& e)
    {
	if (this->m_onCmsException)
	    this->m_onCmsException(&e);
    }
    
    catch (lat::Error &e) 
    {
	if (this->m_onError)
	    this->m_onError(&e);
    }

    catch (std::exception &e) 
    {
	if (this->m_onException)
	    this->m_onException(&e);
    }	

    catch (...) 
    {
	if (this->m_onUnhandledException)
	    this->m_onUnhandledException();
    }
    
    rep->node()->addChild(sep);
}
    
void  VisPFRecHitTwig::update(IgRZRep *rep)
{
    VisQueuedTwig::update(rep);
    rep->clear();
}

