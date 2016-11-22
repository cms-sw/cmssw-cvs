#include "VisReco/VisPF/interface/VisPFRecTrackTwig.h"

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
#include "Iguana/Inventor/interface/IgSoArrow.h"
#include "Iguana/Inventor/interface/IgSoSimpleTrajectory.h"
#include "Iguana/Inventor/interface/IgSoIdealTrack.h"
#include "Iguana/Studio/interface/IgDocumentData.h"

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>

#include <qstring.h>
#include <classlib/utils/DebugAids.h>
#include <algorithm>
#include <iostream>
#include <iomanip>

#include "DataFormats/ParticleFlowReco/interface/PFBrem.h"

namespace 
{
    VisQueuedTwig* createThisTwig(IgState *state, IgTwig *parent,
				  const std::string &name,
				  const std::string &friendlyName,
				  const std::string &modLabel,
				  const std::string &instanceName,
				  const std::string &processName)
    {
	return new VisPFRecTrackTwig(state, parent, name, friendlyName, modLabel, instanceName, processName);
    }	
}

VisPFRecTrackTwig::VisPFRecTrackTwig(IgState *state, IgTwig *parent, 
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
	
    edm::TypeID pfTracksID(typeid (reco::PFRecTrackCollection));
    
    tfService->registerTwig(pfTracksID.friendlyClassName(), &createThisTwig);
}

void VisPFRecTrackTwig::onNewEvent(const edm::Event &event,
				  const edm::EventSetup &eventSetup)
{
    VisQueuedTwig::onNewEvent(event, eventSetup);

    m_tracks.clear();
    
    m_trajectoryPositions.clear();
    m_trajectoryDirections.clear();
    
    //m_bremTrajectoryPositions.clear();
    //m_bremTrajectoryDirections.clear();
    

    std::vector<edm::Handle<reco::PFRecTrackCollection> > trackCollections;

    try
    {
	if ( (! m_friendlyName.empty ()) || 
	     (! m_moduleLabel.empty ())  || 
	     (! m_instanceName.empty ()) || 
	     (! m_processName.empty ()) )
	{
	    VisEventSelector visSel(m_friendlyName, m_moduleLabel, 
				    m_instanceName, m_processName);
	    
	    event.getMany(visSel, trackCollections);
	}

	else
	{
	    event.getManyByType(trackCollections);
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
    
    if (! trackCollections.empty())
    {
	for (std::vector<edm::Handle<reco::PFRecTrackCollection> >::iterator i = 
		 trackCollections.begin(), iEnd = trackCollections.end(); 
	     i != iEnd; ++i) 
	{
	    const reco::PFRecTrackCollection& c = *(*i);

	    for (std::vector<reco::PFRecTrack>::const_iterator ipft = c.begin(), ipftEnd = c.end();
		 ipft != ipftEnd; ++ipft)
	    {
		m_tracks.push_back((*ipft));

		//std::cout<<"DEBUG: There are "<< (*ipft).PFRecBrem().size() <<" PFBrems"<<std::endl;
		//int nbr = 0;
	
		/*
		for ( std::vector<reco::PFBrem>::const_iterator ib  = (*ipft).PFRecBrem().begin();
		      ib != (*ipft).PFRecBrem().end(); ++ib )
		{
		    const std::vector<reco::PFTrajectoryPoint>& bremPoints = (*ib).trajectoryPoints();
		    
		    std::vector<SbVec3f> bremPos;
		    std::vector<SbVec3f> bremDir;	

		    //std::cout<<"DEBUG: Brem "<< nbr++ <<std::endl;
		    
		    for ( unsigned int b = 0; b < bremPoints.size(); ++b )
		    {
			if ( bremPoints[b].isValid() )
			{
			    double x = bremPoints[b].position().X()/100.0;
			    double y = bremPoints[b].position().Y()/100.0;
			    double z = bremPoints[b].position().Z()/100.0;
		
			    float dirx = bremPoints[b].momentum().X();
			    float diry = bremPoints[b].momentum().Y();
			    float dirz = bremPoints[b].momentum().Z();
			    
			    //std::cout<<"  DEBUG: x,y,z -> "<< x <<","<< y <<","<< z <<std::endl;
			    //std::cout<<"  DEBUG: px,py,pz -> "<< dirx <<","<< diry <<","<< dirz <<std::endl;
			    
			    SbVec3f dir(dirx, diry, dirz);
			    dir.normalize();
		    
			    bremPos.push_back(SbVec3f(x,y,z));
			    bremDir.push_back(dir);
			}
	
		    } // end of iteration over brem trajectory points

		    m_bremTrajectoryPositions.push_back(bremPos);	
		    m_bremTrajectoryDirections.push_back(bremDir);

		} // end of iteration over brems
		*/

		std::vector<SbVec3f> traPos;
		std::vector<SbVec3f> traDir;
		
		const std::vector<reco::PFTrajectoryPoint>& points = (*ipft).trajectoryPoints();

		for ( unsigned int ipt = 0; ipt < points.size(); ++ipt )
		{
		    if ( points[ipt].isValid() )
		    {
			double x = points[ipt].position().X()/100.0;
			double y = points[ipt].position().Y()/100.0;
			double z = points[ipt].position().Z()/100.0;
		
			float dirx = points[ipt].momentum().X();
			float diry = points[ipt].momentum().Y();
			float dirz = points[ipt].momentum().Z();
			SbVec3f dir(dirx, diry, dirz);
			dir.normalize();
		    
			traPos.push_back(SbVec3f(x,y,z));
			traDir.push_back(dir);
		    }
		}

		unsigned int algoType = (*ipft).algoType();
		SbColor color;
		
		if ( algoType == 1 )
		    color = SbColor(1.0, 52.0/255.0, 179.0/255.0);
		
		else if ( algoType == 2 )
		    color = SbColor(1.0, 0.0, 1.0);
		
		else if ( algoType == 3)
		    color = SbColor(1.0, 181.0/255.0, 197.0/255.0);
		                    
		else 
		    color = SbColor(1.0, 131.0/255.0, 250.0/255.0);
		
		std::pair<SbColor, std::vector<SbVec3f> > posPair(color, traPos);
		std::pair<SbColor, std::vector<SbVec3f> > dirPair(color, traDir);
		    
		m_trajectoryPositions.push_back(posPair);
		m_trajectoryDirections.push_back(dirPair);
	    
	    } // end of iteration over tracks

	} // end of iteration over track collection
    }
    
    else
	std::cout<<"DEBUG: trackCollections.empty()"<<std::endl;
	
    VisQueuedTwig::onBaseInvalidate();
}


void  VisPFRecTrackTwig::update(Ig3DRep *rep)
{
    VisQueuedTwig::update(rep);
    rep->clear();

    SoSeparator* contents = new SoSeparator;
    
    if ( ! m_trajectoryPositions.empty() && ! m_trajectoryDirections.empty() )
    {
	std::vector<std::pair<SbColor, std::vector<SbVec3f> > >::const_iterator ipos 
	    = m_trajectoryPositions.begin();

	std::vector<std::pair<SbColor, std::vector<SbVec3f> > >::const_iterator idir 
	    = m_trajectoryDirections.begin();
		
	for ( unsigned int itrack = 0; 
	      itrack < m_trajectoryPositions.size(); 
	      ++itrack )
	{
	    SoMaterial* mat1 = new SoMaterial;
	    mat1->diffuseColor.setValue(m_trajectoryPositions[itrack].first);
	    contents->addChild(mat1);
 
	    IgSoSimpleTrajectory* obj1 = new IgSoSimpleTrajectory;
	    
	    int nState = 0;
	    
	    std::vector<SbVec3f>::const_iterator idd = (*idir).second.begin();
	    
	    for ( std::vector<SbVec3f>::const_iterator ipp = (*ipos).second.begin(); 
		  ipp != (*ipos).second.end(); ++ipp )
	    {
		obj1->controlPoints.set1Value(nState, (*ipp));
		obj1->markerPoints.set1Value(nState, (*ipp));

		nState++;
		idd++;
	    }
	    
	    obj1->linePattern = 0xdfff;
	    obj1->lineWidth = 3.0;
	    
	    contents->addChild(obj1);
	    
	    ipos++;
	    idir++;
	}
    }

    /*
    if ( ! m_bremTrajectoryPositions.empty() && ! m_bremTrajectoryDirections.empty() )
    {
	SoMaterial* mat2 = new SoMaterial;

	mat2->diffuseColor.setValue(SbColor(1.0, 1.0, 0.0));
	contents->addChild(mat2);

	std::vector<std::vector<SbVec3f> >::const_iterator ipos = m_bremTrajectoryPositions.begin();
	std::vector<std::vector<SbVec3f> >::const_iterator idir = m_bremTrajectoryDirections.begin();
	
	for ( unsigned int itrack = 0; 
	      itrack < m_bremTrajectoryPositions.size(); 
	      ++itrack )
	{
	    //std::cout<<"Brem track #"<< itrack <<std::endl;
	    //std::cout<<"----------------"<<std::endl;
	    
	    IgSoSimpleTrajectory* obj2 = new IgSoSimpleTrajectory;
	    
	    int nState = 0;
	    
	    std::vector<SbVec3f>::const_iterator idd = (*idir).begin();
	    
	    for ( std::vector<SbVec3f>::const_iterator ipp = (*ipos).begin(); 
		  ipp != (*ipos).end(); ++ipp )
	    {
		obj2->controlPoints.set1Value(nState, (*ipp));
		obj2->markerPoints.set1Value(nState, (*ipp));

		//std::cout<<"Pos: "<< (*ipp)[0] <<","<< (*ipp)[1] <<","<< (*ipp)[2] <<std::endl;
		//std::cout<<"Dir: "<< (*idd)[0] <<","<< (*idd)[1] <<","<< (*idd)[2] <<std::endl;
				
		nState++;
		idd++;
	    }
	    
	    obj2->linePattern = 0xCCCC;
	    obj2->lineWidth = 2.0;
	    
	    contents->addChild(obj2);
	    
	    ipos++;
	    idir++;
	}
    }
    */

    rep->node()->addChild(contents);
}

void  VisPFRecTrackTwig::update(IgLegoRep *rep)
{
     VisQueuedTwig::update(rep);
}

void  VisPFRecTrackTwig::update(IgTextRep *rep)
{
    VisQueuedTwig::update(rep);

    std::ostringstream  text;

    text << m_name << " from ";
    text << m_text << "<br>";
    text << "Total: " << m_tracks.size () << " PFRecTracks.<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Algorithm</TH>"
	 << "<TH>P<SUB>T</TH>"
	 << "<TH>P (GeV)</TH>"
	 << "<TH>Rho</TH>"
	 << "<TH>Z</TH>"
	 << "<TH># tracker measurements</TH>"
	 << "<TH># points total</TH>"
	 << "<TH>Charge</TH>"
	 << "</TR>";

    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
 
    try
    {
	if ( ! m_tracks.empty() )
	{
	    int nTracks = 0;
	
	    for ( std::vector<reco::PFRecTrack>::iterator iT = m_tracks.begin();
		  iT != m_tracks.end(); ++iT )
	    {
		std::string algoString;
		unsigned int algoType = (*iT).algoType();
		std::string color;
		
		if ( algoType == 1 )
		{    
		    algoString = "KF";
		    color = "<TD bgcolor=\"#ff34b3\">";
		}
		
		else if ( algoType == 2 )
		{
		    algoString = "GSF";
		    color = "<TD bgcolor=\"#ff00ff\">";
		}
		
		else if ( algoType == 3)
		{
		    algoString = "KF_ELCAND";
		    color = "<TD bgcolor=\"#ffb5c5\">";
		}
		
		else
		{
		    algoString == "Unknown";
		    color = "<TD bgcolor=\"#ff83fa\">";
		}
		
		const reco::PFTrajectoryPoint& closestApproach = 
		    (*iT).trajectoryPoint(reco::PFTrajectoryPoint::ClosestApproach);

		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nTracks++ << "</TD>"
		     << color << std::setw (2) << std::setprecision (3) << algoString << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << closestApproach.momentum().Pt() << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << closestApproach.momentum().P() << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << closestApproach.position().Rho() << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << closestApproach.position().Z() << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*iT).nTrajectoryMeasurements() << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*iT).nTrajectoryPoints() << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*iT).charge () << "</TD>"
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

void  VisPFRecTrackTwig::update(IgRPhiRep *rep)
{
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator* contents = new SoSeparator;
    
    if ( ! m_trajectoryPositions.empty() && ! m_trajectoryDirections.empty() )
    {
	std::vector<std::pair<SbColor, std::vector<SbVec3f> > >::const_iterator ipos 
	    = m_trajectoryPositions.begin();

	std::vector<std::pair<SbColor, std::vector<SbVec3f> > >::const_iterator idir 
	    = m_trajectoryDirections.begin();
	
	for ( unsigned int itrack = 0; 
	      itrack != m_trajectoryPositions.size(); 
	      ++itrack )
	{
	    SoMaterial* mat = new SoMaterial;

	    mat->diffuseColor.setValue(m_trajectoryPositions[itrack].first);
	    contents->addChild(mat);

	    IgSoSimpleTrajectory* obj = new IgSoSimpleTrajectory;
	    
	    int nState = 0;
	    float zlayer = 0.1;
	    
	    std::vector<SbVec3f>::const_iterator idd = (*idir).second.begin();
	    
	    for ( std::vector<SbVec3f>::const_iterator ipp = (*ipos).second.begin(); 
		  ipp != (*ipos).second.end(); ++ipp )
	    {
		SbVec3f rzpos((*ipp)[0], (*ipp)[1], zlayer);
		obj->controlPoints.set1Value(nState, rzpos);
		obj->markerPoints.set1Value(nState, rzpos);

		nState++;
		idd++;
	    }
	    
	    obj->linePattern = 0xffff;
	    obj->lineWidth = 3.0;
	    
	    contents->addChild(obj);
	    
	    ipos++;
	    idir++;
	}
    }

    /*
    if ( ! m_bremTrajectoryPositions.empty() && ! m_bremTrajectoryDirections.empty() )
    {
	SoMaterial* mat2 = new SoMaterial;

	mat2->diffuseColor.setValue(SbColor(1.0, 1.0, 0.0));
	contents->addChild(mat2);

	std::vector<std::vector<SbVec3f> >::const_iterator ipos = m_bremTrajectoryPositions.begin();
	std::vector<std::vector<SbVec3f> >::const_iterator idir = m_bremTrajectoryDirections.begin();
	
	for ( unsigned int itrack = 0; 
	      itrack < m_bremTrajectoryPositions.size(); 
	      ++itrack )
	{
	    IgSoSimpleTrajectory* obj2 = new IgSoSimpleTrajectory;
	    
	    int nState = 0;
	    float zlayer = 0.1;
    
	    std::vector<SbVec3f>::const_iterator idd = (*idir).begin();
	    
	    for ( std::vector<SbVec3f>::const_iterator ipp = (*ipos).begin(); 
		  ipp != (*ipos).end(); ++ipp )
	    {
		SbVec3f rzpos((*ipp)[0], (*ipp)[1], zlayer);
		obj2->controlPoints.set1Value(nState, rzpos);
		obj2->markerPoints.set1Value(nState, rzpos);

		nState++;
		idd++;
	    }
	    
	    obj2->linePattern = 0xCCCC;
	    obj2->lineWidth = 2.0;
	    
	    contents->addChild(obj2);
	    
	    ipos++;
	    idir++;
	}
    }
    */

    rep->node()->addChild(contents);
}

void  VisPFRecTrackTwig::update(IgRZRep *rep)
{
    VisQueuedTwig::update(rep);
    rep->clear();
}


