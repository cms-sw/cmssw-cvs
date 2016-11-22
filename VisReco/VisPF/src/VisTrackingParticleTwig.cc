#include "VisReco/VisPF/interface/VisTrackingParticleTwig.h"

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
#include "Iguana/Inventor/interface/IgParticleChar.h"

#include "Iguana/Studio/interface/IgDocumentData.h"

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>

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
	return new VisTrackingParticleTwig(state, parent, name, friendlyName, modLabel, instanceName, processName);
    }	
}

VisTrackingParticleTwig::VisTrackingParticleTwig(IgState *state, IgTwig *parent, 
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
	
    edm::TypeID pfTracksID(typeid(TrackingParticleCollection));
    
    tfService->registerTwig(pfTracksID.friendlyClassName(), &createThisTwig);
}

void VisTrackingParticleTwig::onNewEvent(const edm::Event &event,
					 const edm::EventSetup &eventSetup)
{
    VisQueuedTwig::onNewEvent(event, eventSetup);

    std::vector<edm::Handle<TrackingParticleCollection> > trackCollections;
    //std::vector<edm::Handle<TrackingVertexCollection> > vertexCollections;

    m_tracks.clear();
    
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

	//event.getManyByType(vertexCollections);
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
	for ( std::vector<edm::Handle<TrackingParticleCollection> >::iterator i = trackCollections.begin();
	      i != trackCollections.end(); ++i )
	{
	    const TrackingParticleCollection& c = *(*i);
	    
	    for ( std::vector<TrackingParticle>::const_iterator itp = c.begin();
		  itp != c.end(); ++itp )
	    {
		double x = (*itp).parentVertex()->position().x()/100.0;
		double y = (*itp).parentVertex()->position().y()/100.0;
		double z = (*itp).parentVertex()->position().z()/100.0;

		if ( (*itp).pt() > 1.0 )
		{
		    std::pair<TrackingParticle, SbVec3f> p((*itp), SbVec3f(x,y,z));
		    m_tracks.push_back(p);
		}
	    }
	} 
    }

    //std::cout<<"DEBUG: "<< m_tracks.size() <<" TrackingParticles"<<std::endl;
    

    /*
    if (! vertexCollections.empty ())
    {
	for (std::vector<edm::Handle<TrackingVertexCollection> >::iterator i = vertexCollections.begin (); 
	     i != vertexCollections.end (); ++i) 
	{
	    const TrackingVertexCollection& c = *(*i);

	    for (std::vector<TrackingVertex>::const_iterator itv = c.begin();
		 itv != c.end(); ++itv)
	    {
		m_vertices.push_back((*itv));
	    }    
	}
    }
    */
    VisQueuedTwig::onBaseInvalidate();
}

void  VisTrackingParticleTwig::update(Ig3DRep *rep)
{
    VisQueuedTwig::update(rep);
    rep->clear();

    if ( ! m_tracks.empty () ) 
    {
	SoSeparator *sep = new SoSeparator;
	
	try 
	{
	    std::vector<std::pair<TrackingParticle, SbVec3f> >::const_iterator i;

	    for ( i = m_tracks.begin(); i != m_tracks.end(); ++i ) 
	    {
		
		/*
		  Core dump when I try to get parentVertex() from TrackingParticle
		double x0 = (*i).parentVertex()->position().x () / 100.0;
		double y0 = (*i).parentVertex()->position().y () / 100.0;
		double z0 = (*i).parentVertex()->position().z () / 100.0;
		*/
	  
		IgSoIdealTrack *kit = new IgSoIdealTrack;
			    
		//float pt = i->first.pt();
		
		double x0 = (i->second)[0];
		double y0 = (i->second)[1];
		double z0 = (i->second)[2];
		
		double px = i->first.px();
		double py = i->first.py();
		double pz = i->first.pz();
		
		switch (i->first.pdgId())
		{	
		case 11:
		    kit->initialise (x0, y0, z0, px, py, pz, 
				     0.0, IgParticleChar::getByName ("electron"));
		    sep->addChild (kit);
		    break;
		
		case -11:
		    kit->initialise (x0, y0, z0, px, py, pz, 
				     0.0, IgParticleChar::getByName ("positron"));
		    sep->addChild (kit);
		    break;
	
		case 211:
		    kit->initialise(x0, y0, z0, px, py, pz, 
				    0.0, IgParticleChar::getByName ("antipion"));
		    sep->addChild(kit);
		    break;
		case -211:
		    kit->initialise(x0, y0, z0, px, py, pz, 
				    0.0, IgParticleChar::getByName ("pion"));		    
		    sep->addChild (kit);
		    break;

		case 22:
		    {
			SoSeparator *gammas = new SoSeparator;
			SoMaterial *gammaMaterial = new SoMaterial;
			gammaMaterial->ambientColor.setValue (0.000, 0.000, 0.000); 
			gammaMaterial->diffuseColor.setValue (1.000, 1.000, 0.000);  
			gammaMaterial->emissiveColor.setValue (1.000, 1.000, 0.000); 
			gammaMaterial->specularColor.setValue (0.000, 0.000, 0.000); 
			gammaMaterial->shininess = 0.0;
			gammaMaterial->transparency = 0.0;
			gammas->addChild (gammaMaterial);
                         
			IgSoSimpleTrajectory *track = new IgSoSimpleTrajectory;
			
			track->controlPoints.set1Value (0, SbVec3f (x0, y0, z0));
			track->markerPoints.set1Value (0, SbVec3f (x0, y0, z0));
			
			double x = x0;
		        double y = y0;
			double z = z0;
			
			do 
			{
			    x += 0.1*px;
			    y += 0.1*py;
			    z += 0.1*pz;
			}
			while ( sqrt(x*x+y*y) < 1.24 && fabs(z) < 3.14 );
				
			SbVec3f pos = SbVec3f(x,y,z);
			
			track->controlPoints.set1Value (1, pos);
			track->markerPoints.set1Value (1, pos);
			gammas->addChild (track);
                          
			sep->addChild (gammas);
                          
			break;
		    }
		   
		    
		default:
		    break;
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
	
    
}

void  VisTrackingParticleTwig::update(IgLegoRep *rep)
{
     VisQueuedTwig::update(rep);
}

void  VisTrackingParticleTwig::update(IgTextRep *rep)
{
    VisQueuedTwig::update(rep);

    std::ostringstream  text;
    text<< m_tracks.size() <<" TrackingParticles";
    
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>P<SUB>T</TH>"
	 << "<TH>PDG Code</TH>"
	 << "</TR>";

    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if (! m_tracks.empty()) 
    {
	int nTracks = 0;
	
	try 
	{
	    std::vector<std::pair<TrackingParticle, SbVec3f> >::const_iterator i;
	    
	    for ( i = m_tracks.begin(); i != m_tracks.end(); ++i ) 
	    {
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nTracks++ << "</TD>"
		     << "<TD>" << std::setw (3) << i->first.pt() << "</TD>"
		     << "<TD>" << std::setw (3) << i->first.pdgId() << "</TD></TR>";
		
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

    rep->setText(text.str());
}

void  VisTrackingParticleTwig::update(IgRPhiRep *rep)
{
    VisQueuedTwig::update (rep);
    rep->clear ();

    if ( ! m_tracks.empty () ) 
    {
	SoSeparator *sep = new SoSeparator;
	
	try 
	{
	    std::vector<std::pair<TrackingParticle, SbVec3f> >::const_iterator i;

	    for ( i = m_tracks.begin(); i != m_tracks.end(); ++i ) 
	    {
		  
		IgSoIdealTrack *kit = new IgSoIdealTrack;
			    
		//float pt = i->first.pt();
		
		double x0 = (i->second)[0];
		double y0 = (i->second)[1];
		double z0 = (i->second)[2];
		
		double px = i->first.px();
		double py = i->first.py();
		double pz = i->first.pz();
		
		switch (i->first.pdgId())
		{	
		case 11:
		    kit->initialise (x0, y0, z0, px, py, pz, 
				     0.0, IgParticleChar::getByName ("electron"));
		    sep->addChild (kit);
		    break;
		
		case -11:
		    kit->initialise (x0, y0, z0, px, py, pz, 
				     0.0, IgParticleChar::getByName ("positron"));
		    sep->addChild (kit);
		    break;
	
		case 211:
		    kit->initialise(x0, y0, z0, px, py, pz, 
				    0.0, IgParticleChar::getByName ("antipion"));
		    sep->addChild(kit);
		    break;
		case -211:
		    kit->initialise(x0, y0, z0, px, py, pz, 
				    0.0, IgParticleChar::getByName ("pion"));		    
		    sep->addChild (kit);
		    break;

		case 22:
		    {
			SoSeparator *gammas = new SoSeparator;
			SoMaterial *gammaMaterial = new SoMaterial;
			gammaMaterial->ambientColor.setValue (0.000, 0.000, 0.000); 
			gammaMaterial->diffuseColor.setValue (1.000, 1.000, 0.000);  
			gammaMaterial->emissiveColor.setValue (1.000, 1.000, 0.000); 
			gammaMaterial->specularColor.setValue (0.000, 0.000, 0.000); 
			gammaMaterial->shininess = 0.0;
			gammaMaterial->transparency = 0.0;
			gammas->addChild (gammaMaterial);
                         
			IgSoSimpleTrajectory *track = new IgSoSimpleTrajectory;
			
			track->controlPoints.set1Value (0, SbVec3f (x0, y0, z0));
			track->markerPoints.set1Value (0, SbVec3f (x0, y0, z0));
			
			double x = x0;
		        double y = y0;
			double z = z0;
			
			do 
			{
			    x += 0.1*px;
			    y += 0.1*py;
			    z += 0.1*pz;
			}
			while ( sqrt(x*x+y*y) < 1.24 && fabs(z) < 3.14 );
			
	
			SbVec3f pos = SbVec3f(x,y,z);
			
			track->controlPoints.set1Value (1, pos);
			track->markerPoints.set1Value (1, pos);
			gammas->addChild (track);
                          
			sep->addChild (gammas);
                          
			break;
	
		    }
		   
		    
		default:
		    break;
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
	

}

void  VisTrackingParticleTwig::update(IgRZRep *rep)
{
    VisQueuedTwig::update(rep);
    rep->clear();
}


