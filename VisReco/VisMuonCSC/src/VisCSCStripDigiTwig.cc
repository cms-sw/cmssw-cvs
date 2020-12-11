//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMuonCSC/interface/VisCSCStripDigiTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCConfigurationService.h"
#include "VisReco/VisMuonCSC/interface/VisCSCChamberDrawer.h"
#include "VisReco/VisMuonCSC/interface/VisCSCChamberSelectorImpl.h"
#include "VisReco/VisMuonCSC/interface/VisCSCStackPlots.h"
#include "VisReco/VisMuonCSC/interface/VisCSCStackPlotService.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisReco/VisMuonCSC/interface/VisCSCDigiTools.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigiCollection.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
//
#include "Geometry/CSCGeometry/interface/CSCLayer.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
//
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/GLModels/interface/IgSpareRep.h"
#include "Iguana/Inventor/interface/IgSoTower.h"
#include "Iguana/GLBrowsers/interface/IgTwigConfigElement.h"

#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <classlib/utils/DebugAids.h>
#include <qstring.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <functional>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
const edm::ParameterSet*      VisCSCStripDigiTwig::m_pset = 0;

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
	return new VisCSCStripDigiTwig (state, parent, "Strip digis (" + name + ")", 
					friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCSCStripDigiTwig::VisCSCStripDigiTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID digiCollID (typeid (CSCStripDigiCollection));
    tfService->registerTwig (digiCollID.friendlyClassName (), &createThisTwig);
    VisCSCStackPlotService *plotSvc = VisCSCStackPlotService::get(state);
    if (! plotSvc )
    {
        plotSvc = new VisCSCStackPlotService (state);
    }
    m_stackplots = plotSvc->getPlots();
    if (! m_pset)
    {
	edm::ParameterSet p;
	VisCSCConfigurationService visService(p);
	m_pset = visService.pSet ();
    }
    m_StripStyleSolidFlag = m_pset->getUntrackedParameter<bool>("VisCSCStripStyleFlag",false);

}

void
VisCSCStripDigiTwig::onNewEvent (const edm::Event &event,
				 const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_digis.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<CSCStripDigiCollection> > digiCollections;
    
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, digiCollections);
	}
	else
	{
	    event.getManyByType (digiCollections);
	}

	eventSetup.get<MuonGeometryRecord> ().get (m_pDD);
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

    if (! digiCollections.empty ())
    {
	std::vector<edm::Handle<CSCStripDigiCollection> >::iterator i;
	std::vector<edm::Handle<CSCStripDigiCollection> >::iterator iEnd;
	for (i = digiCollections.begin (), iEnd = digiCollections.end (); i != iEnd; ++i) 
	{
	    const CSCStripDigiCollection& c = *(*i);
    
	    // Loop over the DetUnits with digis
	    CSCStripDigiCollection::DigiRangeIterator detUnitIt;
	    CSCStripDigiCollection::DigiRangeIterator detUnitItEnd;
	    for (detUnitIt = c.begin (), detUnitItEnd = c.end (); detUnitIt != detUnitItEnd; ++detUnitIt)
	    {
		const CSCDetId &id = (*detUnitIt).first;
		const CSCStripDigiCollection::Range &range = (*detUnitIt).second;
		// Loop over the digis of this DetUnit
		for (CSCStripDigiCollection::const_iterator digiIt = range.first;
		     digiIt != range.second;
		     ++digiIt)
		{
		        std::pair< CSCDetId, CSCStripDigi > p( id, *digiIt );
		        p.first = id;
		        p.second = (*digiIt);

		        m_digis.push_back (p);
		} // for digis in layer
	    } // for layers 
	}
    }
    
    VisQueuedTwig::onBaseInvalidate ();
}

void
VisCSCStripDigiTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "Total " << m_digis.size () << " CSC strip digis: " << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Position</TH>"
	 << "<TH>Strip</TH>"
	 << "<TH>ADC Counts</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if ( ! m_digis.empty () )
    {
    	int nDigis = 0;
	try 
	{
	    for (std::vector<std::pair<CSCDetId, CSCStripDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); 
		 i != iEnd; ++i) 
	    {

                Surface::GlobalPoint pos( NAN, NAN, NAN );
                if (m_pDD.isValid ())
                {
		    pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
                }
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << ++nDigis << "</TD>"
		     << "<TD>" << pos.x () << ", " << pos.y () << ", " << pos.z () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.getStrip () << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3);

		std::vector<int> adcCounts = (*i).second.getADCCounts ();	    
		for (std::vector<int>::const_iterator ci = adcCounts.begin (), ciEnd = adcCounts.end (); ci != ciEnd; ++ci)
		    text << (*ci) << " ";
		text << "</TD></TR>";
	    }
 	    m_stackplots->setData(m_digis);
            m_stackplots->show();
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

void
VisCSCStripDigiTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
    m_dets.clear ();

    if (! m_digis.empty () && m_pDD.isValid ())
    {
	VisCSCChamberDrawer d;	
	const int thresholdOffset = 9; // sigma is about 3 => 3sigma 

	IgTwigConfigElement *element = IgTwigConfigElement::get(state());
	SoDrawStyle *ds = element->drawStyle("VisCSCStripDigiTwig/3D/Strip");
	SoMaterial *mat = element->material("VisCSCStripDigiTwig/3D/Strip");

        SoSeparator *sep = new SoSeparator;
	SoSeparator *pulses = new SoSeparator; // strips
	if (mat) pulses->addChild (mat);
	if (ds) pulses->addChild (ds);
	sep->addChild (pulses);// strips
	
	try 
	{
	    std::vector<SbVec3f> vtxPts;

	    for (std::vector<std::pair<CSCDetId, CSCStripDigi> >::const_iterator 
		     i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i)
	    {
		const CSCDetId& id = (*i).first;
		std::vector<int> adcCounts = (*i).second.getADCCounts ();
                int signalThreshold =  (adcCounts [0] + adcCounts [1])/2 + thresholdOffset; 


                if ( std::find_if(adcCounts.begin(),adcCounts.end(),bind2nd(std::greater<int>(),signalThreshold)) != adcCounts.end() )
                {
			// These are strips with signal
		        if (m_dets.find(id) == m_dets.end() )
                        {
                            m_dets.insert(id);	
			    // This is a frame around the (active) layer
			    d.getOutline(sep, m_pDD, id);

                        }
			
			//
			int strip_id = ((*i).second).getStrip();
			const CSCLayer *layer_p = m_pDD->layer (id);
			const CSCLayerGeometry *layerGeom = layer_p->geometry ();
			// tried both face and line set to represent stips: prefer line but keep possibility around
			// from Tim Cox
			if (m_StripStyleSolidFlag) // select faceSet to represent a stip
			{
			    MeasurementPoint mp(static_cast<float>(strip_id)-0.95, -0.5); // strip istrip, -y 
			    LocalPoint lp = layerGeom->topology()->localPosition(mp);
			    Surface::GlobalPoint pos = (m_pDD->idToDet (id))->surface ().toGlobal(lp);
			    vtxPts.push_back(SbVec3f(pos.x()/100.0, pos.y()/100.0, pos.z()/100.0));
			    mp = MeasurementPoint(static_cast<float>(strip_id)-0.95,  0.5); // strip istrip  +y 
                            lp = layerGeom->topology()->localPosition(mp);
			    pos = (m_pDD->idToDet (id))->surface ().toGlobal(lp);
			    vtxPts.push_back(SbVec3f(pos.x()/100.0, pos.y()/100.0, pos.z()/100.0));
			    mp = MeasurementPoint(static_cast<float>(strip_id)-0.05,  0.5); // strip istrip  +y 
                            lp = layerGeom->topology()->localPosition(mp);
			    pos = (m_pDD->idToDet (id))->surface ().toGlobal(lp);
			    vtxPts.push_back(SbVec3f(pos.x()/100.0, pos.y()/100.0, pos.z()/100.0));
			    mp = MeasurementPoint(static_cast<float>(strip_id)-0.05,  -0.5); // strip istrip  -y 
                            lp = layerGeom->topology()->localPosition(mp);
			    pos = (m_pDD->idToDet (id))->surface ().toGlobal(lp);
			    vtxPts.push_back(SbVec3f(pos.x()/100.0, pos.y()/100.0, pos.z()/100.0));
			}
			else // use a line down the middle of the strip
			{
			    MeasurementPoint mp(static_cast<float>(strip_id)-0.5, -0.5); // strip istrip, -y 
			    LocalPoint lp = layerGeom->topology()->localPosition(mp);
			    Surface::GlobalPoint pos = (m_pDD->idToDet (id))->surface ().toGlobal(lp);
			    vtxPts.push_back(SbVec3f(pos.x()/100.0, pos.y()/100.0, pos.z()/100.0));
			    mp = MeasurementPoint(static_cast<float>(strip_id)-0.5,  0.5); // strip istrip  +y 
                            lp = layerGeom->topology()->localPosition(mp);
			    pos = (m_pDD->idToDet (id))->surface ().toGlobal(lp);
			    vtxPts.push_back(SbVec3f(pos.x()/100.0, pos.y()/100.0, pos.z()/100.0));
			}
			
               }
	       SoSeparator *newHits = new SoSeparator;
	       SoCoordinate3 *coords = new SoCoordinate3;
	       coords->point.setValues(0,vtxPts.size(),&vtxPts[0]);
	       SoNonIndexedShape *set;
	       if (m_StripStyleSolidFlag) // faceSet/line for strip
	       {
		   std::vector<int> nv(vtxPts.size()/4,4);
		   SoFaceSet *fs = new SoFaceSet;
		   fs->numVertices.setValues(0,nv.size(),&nv[0]);
		   set = fs;
	       }
	       else
	       {
		   std::vector<int> nv(vtxPts.size()/2,2);
		   SoLineSet *ls = new SoLineSet;
		   ls->numVertices.setValues(0,nv.size(),&nv[0]);
		   set = ls;
	       }
	       newHits->addChild(coords);
	       newHits->addChild(set);
	       pulses->addChild(newHits);
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
    }// end of if (m_digis.empty)
}

void
VisCSCStripDigiTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;    

    if (! m_digis.empty () && m_pDD.isValid ())
    {
	SoSeparator *posSep = new SoSeparator;    
	SoSeparator *negSep = new SoSeparator;

	IgTwigConfigElement *element = IgTwigConfigElement::get(state());
	SoDrawStyle *ds = element->drawStyle("VisCSCStripDigiTwig/Lego/+ve");
	SoMaterial *mat = element->material("VisCSCStripDigiTwig/Lego/+ve");
	posSep->addChild (mat);
	posSep->addChild (ds);

	ds = element->drawStyle("VisCSCStripDigiTwig/Lego/-ve");
	mat = element->material("VisCSCStripDigiTwig/Lego/-ve");
	negSep->addChild (mat);
	negSep->addChild (ds);

	sep->addChild (posSep);
	sep->addChild (negSep);
	
	try 
	{
	    int n = 0;
	
	    for (std::vector<std::pair<CSCDetId, CSCStripDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); 
		 i != m_digis.end (); ++i) 
	    {
		Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
		double eta = pos.eta ();
		double phi = pos.phi ();
		if (phi < 0.0) phi = 2 * M_PI + phi; // correction in absence of one convention

		// int strip = (*i).second.getStrip ();
		std::vector<int> adcCounts = (*i).second.getADCCounts ();

		for (std::vector<int>::const_iterator k = adcCounts.begin (), kEnd = adcCounts.end (); 
		     k != adcCounts.end (); ++k)
		{
		    IgSoTower *tower = new IgSoTower;
		    tower->position = SbVec2f (phi, eta); // eta, phi
		    tower->scaleFactor = 20.0;
		    tower->energy = (*k);
		    posSep->addChild (tower);
		}
		++n;
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
VisCSCStripDigiTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    SoSeparator *sep = new SoSeparator; 
    if (! m_digis.empty () && m_pDD.isValid ())
    {
	const int thresholdOffset = 9; // sigma is about 3 => 3sigma 

	IgTwigConfigElement *element = IgTwigConfigElement::get(state());
	SoDrawStyle *ds = element->drawStyle("VisCSCStripDigiTwig/RPhi");
	SoMaterial *mat = element->material("VisCSCStripDigiTwig/RPhi");
	if (mat) sep->addChild (mat);
	if (sep) sep->addChild (ds);

	ds = element->drawStyle("VisCSCStripDigiTwig/RPhi/Strip");
	mat = element->material("VisCSCStripDigiTwig/RPhi/Strip");
	SoSeparator	*pulses = new SoSeparator; // strips
	if (mat) pulses->addChild (mat);
	if (ds) pulses->addChild (ds);
	
	sep->addChild(pulses);
	
	try 
	{
	    std::vector<SbVec3f> vtxPts;

	    for (std::vector<std::pair<CSCDetId, CSCStripDigi> >::const_iterator 
		     i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i)
	    {
		const CSCDetId& id = (*i).first;
//		if (CSCDetCmp(id, m_did))
		{
		    std::vector<int> adcCounts = (*i).second.getADCCounts ();
                    int signalThreshold =  (adcCounts [0] + adcCounts [1])/2 + thresholdOffset; 

		    const CSCLayer *layer_p = m_pDD->layer (id);
		    const CSCLayerGeometry *layerGeom = layer_p->geometry ();

                    if ( std::find_if(adcCounts.begin(),adcCounts.end(),bind2nd(std::greater<int>(),signalThreshold)) != adcCounts.end() )
                    {
			    // These are strips with signal

			    int strip_id = ((*i).second).getStrip();
			    MeasurementPoint mp(static_cast<float>(strip_id)-0.5, -0.5); // strip istrip, -y 
			    LocalPoint lp = layerGeom->topology()->localPosition(mp);
			    Surface::GlobalPoint pos = (m_pDD->idToDet (id))->surface ().toGlobal(lp);
			    vtxPts.push_back(SbVec3f(pos.x()/100.0, pos.y()/100.0, pos.z()/100.0));
			    mp = MeasurementPoint(static_cast<float>(strip_id)-0.5,  0.5); // strip istrip  +y 
                            lp = layerGeom->topology()->localPosition(mp);
			    pos = (m_pDD->idToDet (id))->surface ().toGlobal(lp);
			    vtxPts.push_back(SbVec3f(pos.x()/100.0, pos.y()/100.0, pos.z()/100.0));
                    }
		}
	   }
	   SoSeparator *newHits = new SoSeparator;
	   SoCoordinate3 *coords = new SoCoordinate3;
	   coords->point.setValues(0,vtxPts.size(),&vtxPts[0]);
	   SoLineSet *set = new SoLineSet;
	   std::vector<int> nv(vtxPts.size()/2,2);
	   set->numVertices.setValues(0,nv.size(),&nv[0]);
	   newHits->addChild(coords);
	   newHits->addChild(set);
	   pulses->addChild(newHits);
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
    }// end of if (m_digis.empty)
}


void
VisCSCStripDigiTwig::update (IgSpareRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    //
    rep->clear ();
    //
    if (! m_digis.empty () && m_pDD.isValid ()) 
    {
	SoSeparator *sep = new SoSeparator;    

	IgTwigConfigElement *element = IgTwigConfigElement::get(state());
	SoMaterial *mat = element->material("VisCSCStripDigiTwig/Spare");
	SoDrawStyle *ds = element->drawStyle("VisCSCStripDigiTwig/Spare");

	if (mat) sep->addChild (mat);
	if (sep) sep->addChild(ds);
	
	SoMaterial *matms = element->material("VisCSCStripDigiTwig/Spare/Matched/Strip");
	SoMaterial *matmf = element->material("VisCSCStripDigiTwig/Spare/Matched/Frame");

	SoMaterial *matus = element->material("VisCSCStripDigiTwig/Spare/UnMatched/Strip");
	SoMaterial *matuf = element->material("VisCSCStripDigiTwig/Spare/UnMatched/Frame");

	SoDrawStyle *dsw = element->drawStyle("VisCSCStripDigiTwig/Spare/Strip");

	try 
	{
	    const int thresholdOffset = 9; //ADC counts
	    bool iterations = false;
	    float OurPhi = 9999999.;
	    float OurPhiSlice = 9999999.;
	    float OurZ = 99999999.;
	    for (std::vector<std::pair<CSCDetId, CSCStripDigi> >::const_iterator i = 
		     m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	    {
		const CSCDetId& id = (*i).first;
		const CSCLayer* layer_p = m_pDD->layer (id);
		const CSCLayerGeometry* layerGeom = 0;
	      
		if (layer_p)
		{
		    layerGeom = layer_p->geometry();
		}
		else
		{
		    // to be filled...
		}

		std::vector<int> adcCounts = (*i).second.getADCCounts ();
                int signalThreshold = (adcCounts[0]+adcCounts[1])/2 + thresholdOffset;
                if ( std::find_if(adcCounts.begin(),adcCounts.end(),bind2nd(std::greater<int>(),signalThreshold)) != adcCounts.end() )
                {
		    Surface::GlobalPoint pos =  (m_pDD->idToDet (id))->surface ().position ();
		    float layer_x = pos.x () / 100.0;  // cm -> m
		    float layer_y = pos.y () / 100.0;  // cm -> m
		    float layer_z = pos.z () / 100.0;  // cm -> m
		    float layer_phi = pos.phi ();
		    float layer_r = sqrt(layer_x*layer_x + layer_y*layer_y);
		    if (!iterations)
		    {
			// Make captions, some init
			iterations=VisCaptions(layer_z, layer_phi, OurPhi, OurZ, sep);
		    }
		    //
		    const std::vector<float> trapezBounds = layerGeom->parameters();
		    OurPhiSlice = 2. * atan(trapezBounds[1]/(layer_r + trapezBounds[3])) * 0.8;
		    // Only adjacent chambers 
		    if (fabs(layer_phi - OurPhi) < 2. * OurPhiSlice)
		    {
			int strip_id = ((*i).second).getStrip();
			float xstrip = layerGeom->xOfStrip(strip_id);
			// 		  float stripLength = layerGeom->length()/100.;
			// Make frames
			SoLineSet *trapez = new SoLineSet;
			SoLineSet *rect_s = new SoLineSet;
			SoLineSet *rect_w = new SoLineSet;
			SoTranslation *trans = new SoTranslation; // trapez frames
			SoTranslation *trans_caption = new SoTranslation; // captions
			SoTranslation *trans_s = new SoTranslation; // strips
			SoTranslation *trans_w = new SoTranslation; // wire groups
			// caption
			SoText3 *TextCaption = new SoText3;
			char ch[10];
			sprintf( ch,"%1.1f", layer_phi);
			TextCaption->string = ch;
			// font
			SoFont *font_caption = new SoFont;
			font_caption->name.setValue(VisFont);
			font_caption->size.setValue(VisFontSize);
			// Strip as a line
			SoVertexProperty *vert = new SoVertexProperty;
			int nVrx = 0;
			float x = xstrip * (1. - trapezBounds[3]/100./layer_r)/100.;
			float y = -trapezBounds[3]/100.;
			float z = 0.; 
			vert->vertex.set1Value (nVrx++,SbVec3f (x, y, z) );
			//
			x = xstrip * (1. + trapezBounds[3]/100./layer_r)/100.;
			y = trapezBounds[3]/100.;
			z = 0.;
			vert->vertex.set1Value (nVrx++,SbVec3f (x, y, z) );
			vert->vertex.setNum (nVrx);
			//
			SoLineSet *StripVis = new SoLineSet;
			StripVis->vertexProperty.setValue (vert);
			StripVis->numVertices[2];
			// Strip projection - a point
			SoVertexProperty *stripDots = new SoVertexProperty;
			nVrx = 0;
			x =  xstrip/ 100.0;  // cm -> m
			y =  (trapezBounds[2]*1.1 -  id.layer()*2.*trapezBounds[2]/6.)/4.;
			z = 0. / 100.0;  // cm -> m
			if(3==id.station()){ 
			    y=-y;// ...
			    x=-x;// ...
			}
			stripDots->vertex.set1Value (nVrx++, SbVec3f (x, y, z));
			stripDots->vertex.setNum (nVrx);
			int Npoints=nVrx;
			// Colors
			SoMaterial *mats;
			SoMaterial *matf;
			if(OurPhi == layer_phi){
			    mats = matms;
			    matf = matmf;
			}
			else{
			    mats = matus;
			    matf = matuf;
			}
			// Build the "picture"
			SoSeparator *shape = new SoSeparator;// frame
			shape->addChild (trapez);
			//
			SoSeparator *caption = new SoSeparator;// caption
			caption->addChild (trans_caption);
			caption->addChild (font_caption);
			caption->addChild (TextCaption);  //
			//
			SoSeparator *proj_s = new SoSeparator;// projection strip frame
			proj_s->addChild (trans_s); 
			proj_s->addChild (rect_s);
			//
			SoSeparator *proj_w = new SoSeparator;// projection wire frame
			proj_w->addChild (trans_w); 
			proj_w->addChild (rect_w);
			//
			SoSeparator *dotsWG = new SoSeparator; // projection strips
			SoPointSet *points = new SoPointSet;
			points->vertexProperty.setValue (stripDots);
			points->numPoints.setValue (Npoints);
			dotsWG->addChild (mats); 
			dotsWG->addChild (dsw);
			dotsWG->addChild (trans_s);
			dotsWG->addChild (points);
			//
			SoSeparator *strip = new SoSeparator;// strip
			strip->addChild (mats); 
			strip->addChild (StripVis);
			//
			SoSeparator *layer = new SoSeparator;// layer
			layer->addChild (trans);
			layer->addChild (shape);
			layer->addChild (strip);
			// Visualization of all
			sep->addChild (matf); // defaults to frame materials
			sep->addChild (layer);
			sep->addChild (caption);
			sep->addChild (proj_s);
			sep->addChild (proj_w);
			sep->addChild (dotsWG);
		    }
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

void
VisCSCStripDigiTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if (! m_digis.empty () && m_pDD.isValid ()) 
    {
	IgTwigConfigElement *element = IgTwigConfigElement::get(state());
	SoMaterial *mat = element->material("VisCSCWireDigiTwig/RZ");
	SoDrawStyle *ds = element->drawStyle("VisCSCWireDigiTwig/RZ");

	SoSeparator *sep = new SoSeparator;    
	sep->addChild (mat);
	sep->addChild (ds);
    
	try 
	{
	    SoCoordinate3 *vertices = new SoCoordinate3;
	    int nVrx = 0;

	    for (std::vector<std::pair<CSCDetId, CSCStripDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); 
		 i != m_digis.end (); ++i) 
	    {
		Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
	    
		float x = pos.x () / 100.0;  // cm -> m
		float y = pos.y () / 100.0;  // cm -> m
		float z = pos.z () / 100.0;  // cm -> m
		
		vertices->point.set1Value (nVrx++, SbVec3f (x, y, z));
	    }
	    SoPointSet *points = new SoPointSet;
	    points->numPoints.setValue (m_digis.size ());
	    sep->addChild(vertices);
	    sep->addChild (points);
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
