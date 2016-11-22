#include "VisReco/VisPF/interface/VisPFClusterTwig.h"


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

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>

#include "DataFormats/ParticleFlowReco/interface/PFLayer.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"

#include "DataFormats/ParticleFlowReco/interface/PFRecHitFraction.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"

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
	return new VisPFClusterTwig(state, parent, name, friendlyName, modLabel, instanceName, processName);
    }	
}


VisPFClusterTwig::VisPFClusterTwig(IgState *state, IgTwig *parent, 
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
	
    edm::TypeID pfClustersID(typeid (reco::PFClusterCollection));
    
    tfService->registerTwig(pfClustersID.friendlyClassName(), &createThisTwig);
}

void VisPFClusterTwig::onNewEvent(const edm::Event &event,
				  const edm::EventSetup &eventSetup)
{
    VisQueuedTwig::onNewEvent(event, eventSetup);

    m_EBclusters.clear();
    m_HBclusters.clear();
    m_EEclusters.clear();

    m_EBrechits.clear();
    m_HBrechits.clear();
    m_EErechits.clear();

    std::vector<edm::Handle<reco::PFClusterCollection> > clusterCollections;

    try
    {
	eventSetup.get<IdealGeometryRecord>().get(m_pDD);
    }

    catch (cms::Exception& e)
    {
	if (this->m_onCmsException)
	    this->m_onCmsException(&e);
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
	    
	    event.getMany(visSel, clusterCollections);
	}

	else
	{
	    event.getManyByType(clusterCollections);
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

    if ( ! clusterCollections.empty() )
    {
	for (std::vector<edm::Handle<reco::PFClusterCollection> >::iterator i = 
		 clusterCollections.begin(), iEnd = clusterCollections.end(); 
	     i != iEnd; ++i) 
	{   
	    const reco::PFClusterCollection& c = *(*i);

	    for (std::vector<reco::PFCluster>::const_iterator ipfc = c.begin(), ipfcEnd = c.end();
		 ipfc != ipfcEnd; ++ipfc)
	    {
		if ( (*ipfc).layer() == PFLayer::ECAL_BARREL )
		{
		    m_EBclusters.push_back((*ipfc));

		    std::vector<reco::PFRecHitFraction>::const_iterator iR;
       
		    for ( iR  = (*ipfc).recHitFractions().begin();
			  iR != (*ipfc).recHitFractions().end();
			  ++iR )
		    {
			RecHit recHit;
	 
			recHit.fraction = (*iR).fraction();
			recHit.energy = (*iR).recHitRef()->energy();
			recHit.detId = (*iR).recHitRef()->detId();
	    
			m_EBrechits.push_back(recHit);
		    }
		}
		
		
		if ( (*ipfc).layer() == PFLayer::HCAL_BARREL1 )
		{
		    m_HBclusters.push_back((*ipfc));
		
		    std::vector<reco::PFRecHitFraction>::const_iterator iR;
       
		    for ( iR  = (*ipfc).recHitFractions().begin();
			  iR != (*ipfc).recHitFractions().end();
			  ++iR )
		    {
			RecHit recHit;
	 
			recHit.fraction = (*iR).fraction();
			recHit.energy = (*iR).recHitRef()->energy();
			recHit.detId = (*iR).recHitRef()->detId();
	    
			m_HBrechits.push_back(recHit);
		    }
		}
		

		if ( (*ipfc).layer() == PFLayer::ECAL_ENDCAP )
		{    
		    m_EEclusters.push_back((*ipfc));

		    std::vector<reco::PFRecHitFraction>::const_iterator iR;
       
		    for ( iR  = (*ipfc).recHitFractions().begin();
			  iR != (*ipfc).recHitFractions().end();
			  ++iR )
		    {
			RecHit recHit;
	 
			recHit.fraction = (*iR).fraction();
			recHit.energy = (*iR).recHitRef()->energy();
			recHit.detId = (*iR).recHitRef()->detId();
	    
			m_EErechits.push_back(recHit);
		    }
		}
		
	    }
	}
    }

    else
	std::cout<<"DEBUG: clusterCollections.empty()"<<std::endl;
    
   
    VisQueuedTwig::onBaseInvalidate();
}

void  VisPFClusterTwig::update(Ig3DRep *rep)
{ 
    VisQueuedTwig::update(rep);     
 
    rep->clear();

    SoSeparator* sep = new SoSeparator;
   
    try
    {
	std::vector<RecHit>::iterator iR;
	
	if ( ! m_EBrechits.empty() && m_pDD.isValid() )
	{
	    SoMaterial* EBmat = new SoMaterial;
	    EBmat->diffuseColor.setValue(1.0, 0.0, 153.0 / 255.0);
	    sep->addChild(EBmat);

	    for ( iR  = m_EBrechits.begin();
		  iR != m_EBrechits.end();
		  ++iR )
	    {	
		const CaloCellGeometry* cell = (*m_pDD).getGeometry((*iR).detId);
		    
		const CaloCellGeometry::CornersVec& corners = cell->getCorners();
		assert(corners.size () == 8);
		
		IgSoCrystalHit* eb_crystalHit = new IgSoCrystalHit;
		eb_crystalHit->energy.setValue((*iR).energy);		    
		eb_crystalHit->scale.setValue(1.0*(*iR).fraction);
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
	std::vector<reco::PFCluster>::const_iterator iC;
	
	if ( ! m_EBclusters.empty() && m_pDD.isValid() )
	{  
	    SoMaterial* EBmat = new SoMaterial;
	    EBmat->diffuseColor.setValue(1.0, 0.0, 153.0 / 255.0);
	    sep->addChild(EBmat);
  	     
	    for ( iC  = m_EBclusters.begin();
		  iC != m_EBclusters.end(); ++iC)
	    {
		double energy = (*iC).energy();
	      
		std::vector<reco::PFRecHitFraction>::const_iterator iR;
		
		if ( ! (*iC).recHitFractions().empty() )
		{   
		    for ( iR  = (*iC).recHitFractions().begin();
			  iR != (*iC).recHitFractions().end(); 
			  ++iR )
		    {
			const CaloCellGeometry* cell = (*m_pDD).getGeometry((*iR).recHitRef()->detId());
		    
			const CaloCellGeometry::CornersVec& corners = cell->getCorners();
			assert(corners.size () == 8);
		    
			IgSoCrystalHit* eb_crystalHit = new IgSoCrystalHit;
			eb_crystalHit->energy.setValue((*iR).recHitRef()->energy());		    
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
	    }
	}
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
		const CaloCellGeometry* cell = (*m_pDD).getGeometry((*iR).detId);

		const CaloCellGeometry::CornersVec& corners = cell->getCorners();
		assert(corners.size () == 8);
		
		IgSoCrystalHit* hb_crystalHit = new IgSoCrystalHit;
		hb_crystalHit->energy.setValue((*iR).energy);		    
		hb_crystalHit->scale.setValue(1.0*(*iR).fraction);
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
	
	if ( ! m_EErechits.empty() && m_pDD.isValid() )
	{
	    SoMaterial* EEmat = new SoMaterial;
	    EEmat->diffuseColor.setValue(1.0, 0.0, 153.0 / 255.0);
	    sep->addChild(EEmat);

	    for ( iR  = m_EErechits.begin();
		  iR != m_EErechits.end();
		  ++iR )
	    {	
		const CaloCellGeometry* cell = (*m_pDD).getGeometry((*iR).detId);
		    
		const CaloCellGeometry::CornersVec& corners = cell->getCorners();
		assert(corners.size () == 8);
		
		IgSoCrystalHit* ee_crystalHit = new IgSoCrystalHit;
		ee_crystalHit->energy.setValue((*iR).energy);		    
		ee_crystalHit->scale.setValue(1.0*(*iR).fraction);
		ee_crystalHit->relativeWidth.setValue(1.0);
		ee_crystalHit->drawCrystal.setValue(true);
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

    rep->node()->addChild(sep);
}

void  VisPFClusterTwig::update(IgLegoRep *rep)
{
     VisQueuedTwig::update(rep);
     
     rep->clear();
    
     SoSeparator* sep = new SoSeparator();

     try
     {
	 if ( ! m_EBclusters.empty() )
	 {
	     std::vector<reco::PFCluster>::const_iterator iC;
	     
	     for ( iC  = m_EBclusters.begin();
		   iC != m_EBclusters.end(); ++iC)
	     {
		 IgSoTower* tower = new IgSoTower();
		
		 double energy = (*iC).energy();
		 tower->energy = energy;
		 
		 double eta = (*iC).position().Eta();
		 double phi = (*iC).position().Phi();
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
	 if ( ! m_HBclusters.empty() )
	 { 
	     std::vector<reco::PFCluster>::const_iterator iC;
	    
	     for ( iC  = m_HBclusters.begin();
		   iC != m_HBclusters.end(); ++iC)
	     {
		 IgSoTower* tower = new IgSoTower();
		
		 double energy = (*iC).energy();
		 tower->energy = energy;
		
		 double eta = (*iC).position().Eta();
		 double phi = (*iC).position().Phi();
		 (phi < 0.0) ? phi = 2 * M_PI + phi : phi;
		 tower->position = SbVec2f(phi, eta);
	
		 tower->scaleFactor = 1.0;
		 tower->emFraction = 0.0;

		 sep->addChild(tower);
	     }   
	 }
	 */

	 if ( ! m_EEclusters.empty() )
	 {
	     std::vector<reco::PFCluster>::const_iterator iC;
	     
	     for ( iC  = m_EEclusters.begin();
		   iC != m_EEclusters.end(); ++iC)
	     {
		 IgSoTower* tower = new IgSoTower();
		
		 double energy = (*iC).energy();
		 tower->energy = energy;
		 
		 double eta = (*iC).position().Eta();
		 double phi = (*iC).position().Phi();
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

void  VisPFClusterTwig::update(IgTextRep *rep)
{
    VisQueuedTwig::update(rep);

    std::ostringstream  text;

    text << m_name << " from ";
 
    text << m_text << "<br>";
    text << "Total: " << m_EBclusters.size () << " EB PFClusters.<br>";
    text << "Total: " << m_HBclusters.size () << " HB PFClusters.<br>";
    text << "Total: " << m_EEclusters.size () << " EE PFClusters.<br>";

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
    
    int nClusters;

    try
    {
	std::vector<reco::PFCluster>::const_iterator iC;
	
	if ( ! m_EBclusters.empty() )
	{
	    nClusters = 0;
	    
	    for ( iC  = m_EBclusters.begin();
		  iC != m_EBclusters.end(); ++iC)
	    {
		double energy = (*iC).energy();
		double eta = (*iC).position().Eta();
		double phi = (*iC).position().Phi();

		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nClusters++ << "</TD>"
		     << "<TD> EB </TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << energy << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << eta << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << phi << "</TD>"
		     << "</TR>";
	    }	    
	}

	if ( ! m_HBclusters.empty() )
	{
	    nClusters = 0;
	    
	    for ( iC  = m_HBclusters.begin();
		  iC != m_HBclusters.end(); ++iC)
	    {
		double energy = (*iC).energy();
		double eta = (*iC).position().Eta();
		double phi = (*iC).position().Phi();

		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nClusters++ << "</TD>"
		     << "<TD> HB </TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << energy << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << eta << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << phi << "</TD>"
		     << "</TR>";
	    }	    
	}
	
	if ( ! m_EEclusters.empty() )
	{
	    nClusters = 0;
	    
	    for ( iC  = m_EEclusters.begin();
		  iC != m_EEclusters.end(); ++iC)
	    {
		double energy = (*iC).energy();
		double eta = (*iC).position().Eta();
		double phi = (*iC).position().Phi();

		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nClusters++ << "</TD>"
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

void  VisPFClusterTwig::update(IgRPhiRep *rep)
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
	if ( ! m_EBclusters.empty() )
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
	    
	    std::vector<reco::PFCluster>::const_iterator iC;
	    
	    for ( iC  = m_EBclusters.begin();
		  iC != m_EBclusters.end(); ++iC)
	    {		
		double energy = (*iC).energy();
		double phi = (*iC).position().Phi();
		(phi < 0.0) ? phi = 2 * M_PI + phi : phi;

		binNumber = (int) floor (phi / (2 * M_PI / nbrOfBins));
 
		(energy >= 0.0) ? bufferPositive[binNumber] += energy :  bufferNegative[binNumber] += energy;
	    }
	    
	    std::vector<float>::const_iterator first = bufferPositive.begin();
	    std::vector<float>::const_iterator last  = bufferPositive.end();
	    
	    if ( *max_element(first, last) > 0.0 )
	    {
		IgSoCircularHist* posEBclusters = new IgSoCircularHist;
		posEBclusters->minRadius = 1.29;
		posEBclusters->maxRadius = -1;
		posEBclusters->scaleFactor = 1.0;
		posEBclusters->numberOfBins = nbrOfBins;
		posEBclusters->energies.setValues(0, nbrOfBins, &bufferPositive[0]);
		posEBclusters->logScale = false;
		posEBclusters->layer = 5.5;
		posSep->addChild(posEBclusters);
	    }
	    
	    std::vector<float>::const_iterator nfirst = bufferNegative.begin();
	    std::vector<float>::const_iterator nlast  = bufferNegative.end();
	    
	    if ( fabs(*max_element(nfirst, nlast)) > 0.0 )
	    {
		IgSoCircularHist* negEBclusters = new IgSoCircularHist;
		negEBclusters->minRadius = 1.29;
		negEBclusters->maxRadius = -1;
		negEBclusters->scaleFactor = 1.0;
		negEBclusters->numberOfBins = nbrOfBins;
		negEBclusters->energies.setValues(0, nbrOfBins, &bufferNegative[0]);
		negEBclusters->logScale = false;
		negEBclusters->layer = 5.0;

		negSep->addChild(negEBclusters);
	    }
	}
	
	/*
	if ( ! m_HBclusters.empty() )
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

	    std::vector<reco::PFCluster>::const_iterator iC;

	    for ( iC  = m_HBclusters.begin();
		  iC != m_HBclusters.end(); ++iC)
	    {		
		double energy = (*iC).energy();
		double phi = (*iC).position().Phi();
		(phi < 0.0) ? phi = 2 * M_PI + phi : phi;

		binNumber = (int) floor (phi / (2 * M_PI / nbrOfBins));
 
		(energy >= 0.0) ? bufferPositive[binNumber] += energy :  bufferNegative[binNumber] += energy;
	    }
	    
	    std::vector<float>::const_iterator first = bufferPositive.begin();
	    std::vector<float>::const_iterator last  = bufferPositive.end();
	    
	    if ( *max_element(first, last) > 0.0 )
	    {
		IgSoCircularHist* posHBclusters = new IgSoCircularHist;
		posHBclusters->minRadius = 1.9;
		posHBclusters->maxRadius = -1;
		posHBclusters->scaleFactor = 1.0;
		posHBclusters->numberOfBins = nbrOfBins;
		posHBclusters->energies.setValues(0, nbrOfBins, &bufferPositive[0]);
		posHBclusters->logScale = false;
		posHBclusters->layer = 5.5;
		posSep->addChild(posHBclusters);
	    }
	    
	    std::vector<float>::const_iterator nfirst = bufferNegative.begin();
	    std::vector<float>::const_iterator nlast  = bufferNegative.end();
	    
	    if ( fabs(*max_element(nfirst, nlast)) > 0.0 )
	    {
		IgSoCircularHist* negHBclusters = new IgSoCircularHist;
		negHBclusters->minRadius = 1.9;
		negHBclusters->maxRadius = -1;
		negHBclusters->scaleFactor = 1.0;
		negHBclusters->numberOfBins = nbrOfBins;
		negHBclusters->energies.setValues(0, nbrOfBins, &bufferNegative[0]);
		negHBclusters->logScale = false;
		negHBclusters->layer = 5.0;

		negSep->addChild(negHBclusters);
	    }
	}
	*/
	
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

void  VisPFClusterTwig::update(IgRZRep *rep)
{
    VisQueuedTwig::update(rep);

    rep->clear();
}


