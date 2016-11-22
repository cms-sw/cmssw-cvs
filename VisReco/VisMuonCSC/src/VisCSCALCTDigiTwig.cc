#include "VisReco/VisMuonCSC/interface/VisCSCALCTDigiTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCChamberDrawer.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/CSCDigi/interface/CSCALCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h"
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
#include "Iguana/Inventor/interface/IgSoTower.h"
//
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <classlib/utils/DebugAids.h>
#include <qstring.h>
#include <iostream>
#include <iomanip>
#include <cmath>


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
	return new VisCSCALCTDigiTwig (state, parent, "ALCT digis (" + name + ")", 
				       friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCSCALCTDigiTwig::VisCSCALCTDigiTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID caloJetsID (typeid (CSCALCTDigiCollection));
    tfService->registerTwig (caloJetsID.friendlyClassName (), &createThisTwig);
}

void
VisCSCALCTDigiTwig::onNewEvent (const edm::Event &event,
				const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_digis.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<CSCALCTDigiCollection> > digiCollections;
    
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
	std::vector<edm::Handle<CSCALCTDigiCollection> >::iterator i;
	std::vector<edm::Handle<CSCALCTDigiCollection> >::iterator iEnd;
	for (i = digiCollections.begin (), iEnd = digiCollections.end (); i != iEnd; ++i) 
	{
	    const CSCALCTDigiCollection& c = *(*i);
    
	    // Loop over the DetUnits with digis
	    CSCALCTDigiCollection::DigiRangeIterator detUnitIt;
	    CSCALCTDigiCollection::DigiRangeIterator detUnitItEnd;
	    for (detUnitIt = c.begin (), detUnitItEnd = c.end (); detUnitIt != detUnitItEnd; ++detUnitIt)
	    {
		const CSCDetId &id = (*detUnitIt).first;
		const CSCALCTDigiCollection::Range &range = (*detUnitIt).second;

		// Loop over the digis of this DetUnit
		for (CSCALCTDigiCollection::const_iterator digiIt = range.first;
		     digiIt != range.second;
		     ++digiIt)
		{
		    std::pair< CSCDetId, CSCALCTDigi > p;
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
VisCSCALCTDigiTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "Total " << m_digis.size () << " CSC A LCT digis: " << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Position</TH>"
	 << "<TH>Validity</TH>"
	 << "<TH>Accelerator bit</TH>"
	 << "<TH>Collision pattern</TH>"
	 << "<TH>Key wire group</TH>"
	 << "<TH>BX</TH>"
	 << "<TH>Track number</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if ((! m_digis.empty ()) && m_pDD.isValid ()) 
    {
    
	int nDigis = 0;
	try 
	{
	    for (std::vector<std::pair<CSCDetId, CSCALCTDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); 
		 i != iEnd; ++i) 
	    {
		Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nDigis++ << "</TD>"
		     << "<TD>" << pos.x () << ", " << pos.y () << ", " << pos.z () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.isValid () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.getQuality () << "</TD>" 
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.getAccelerator () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.getCollisionB () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.getKeyWG () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.getBX () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.getTrknmb () << "</TD></TR>";

		std::cout << "CSCDetId " << (*i).first << std::endl;
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
    rep->setText (text.str ());
}

void
VisCSCALCTDigiTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    m_dets.clear ();

    if ((! m_digis.empty ()) && m_pDD.isValid ())
    {
	VisCSCChamberDrawer d;

	const float width  = 0.01; // strip visualization 
	const float depth  = 0.01; // strip visualization

	SbColor grey35;
        float transparency = 0.0f;
        grey35.setPackedValue(static_cast<uint32_t>(0x00595959), transparency);
        SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (grey35); // grey35
	mat->emissiveColor.setValue(grey35);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 5.0; // big point; useful for the centers of the layers
    
	SoSeparator *sep = new SoSeparator; // global
	sep->addChild (mat);
	sep->addChild (drawStyle);

	try 
	{
	    SoMaterial *matl = new SoMaterial;
	    matl->diffuseColor.setValue (0.0, 1.0, 0.0); // green
	    matl->emissiveColor.setValue(0.0, 1.0, 0.0);	  
	    matl->shininess = 1.;

	    SoDrawStyle *drawStylel = new SoDrawStyle;
	    drawStylel->pointSize = 1.;

	    SoSeparator	*pulses = new SoSeparator; // strips
	    pulses->addChild (matl);
	    pulses->addChild (drawStylel);
	    
	    for (std::vector<std::pair<CSCDetId, CSCALCTDigi> >::const_iterator 
		     i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i)
	    {
		const CSCDetId &id = (*i).first;

		if (m_dets.find(id) == m_dets.end() )
                {
                    m_dets.insert(id);	
		    // This is a frame around the (active) layer
		    d.getOutline(sep, m_pDD, id);
                }

		const CSCLayer *layer = m_pDD->chamber (id)->layer (3);
		const CSCLayerGeometry *layerGeom = layer->geometry ();
		int wireGroup_id = ((*i).second).getKeyWG ();
		LocalPoint centerWireGroup = layerGeom->localCenterOfWireGroup (wireGroup_id);
		Surface::GlobalPoint pos = layer->toGlobal (centerWireGroup);
		float x = pos.x () / 100.0;  // cm -> m
		float y = pos.y () / 100.0;  // cm -> m
		float z = pos.z () / 100.0;  // cm -> m

		SoTranslation *winc = new SoTranslation;
		winc->translation = SbVec3f (x, y, z);
		
		SbVec3f waxis (0., 0., 1.);	
		float wangle = -atan (x / y) - M_PI / 2;
		SbRotation wr (waxis, wangle);
		
		SoTransform *wxform = new SoTransform;
		wxform ->rotation = wr;
		
		SoCube *whit = new SoCube;
		whit->width  = width;
		whit->height = layerGeom->lengthOfWireGroup (wireGroup_id) / 100.;
		whit->depth  = depth;
		
		SoSeparator *wpulse = new SoSeparator;// wire group
		wpulse->addChild (winc);
		wpulse->addChild (wxform);
		wpulse->addChild (whit);
		pulses->addChild (wpulse);
		// Visualize wire groups 
		sep->addChild (pulses);// wire groups
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
VisCSCALCTDigiTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;    
    SoSeparator *posSep = new SoSeparator;    
    SoSeparator *negSep = new SoSeparator;

    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (0.3, 0.8, 0.0);
    posSep->addChild (mat);

    SoMaterial *negMat = new SoMaterial;
    negMat->diffuseColor.setValue (1.0, 0.0, 0.0);
    negSep->addChild (negMat);

    sep->addChild (posSep);
    sep->addChild (negSep);

    if ((! m_digis.empty ()) && m_pDD.isValid ()) 
    {
	try 
	{
	    int n = 0;
	
	    for (std::vector<std::pair<CSCDetId, CSCALCTDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); 
		 i != iEnd; ++i) 
	    {
		Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
		// double eta = pos.eta ();
		double phi = pos.phi ();
		if (phi < 0.0) phi = 2 * M_PI + phi; // correction in absence of one convention

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
VisCSCALCTDigiTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    if (! m_digis.empty ()) 
    {
	SoSeparator *sep = new SoSeparator;    

	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (0.3, 0.8, 0.0);
	sep->addChild (mat);

	try 
	{
	    for (std::vector<std::pair<CSCDetId, CSCALCTDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); 
		 i != iEnd; ++i) 
	    {
		//	Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
		//		double phi = pos.phi ();
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
VisCSCALCTDigiTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if (! m_digis.empty ()) 
    {
	SoSeparator *sep = new SoSeparator;    
	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (0.3, 0.8, 0.0);
	sep->addChild (mat);

	SoDrawStyle *drawStyle = new SoDrawStyle;
	drawStyle->pointSize = 3.0;
	sep->addChild (drawStyle);
    
	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrx = 0;

	    for (std::vector<std::pair<CSCDetId, CSCALCTDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); 
		 i != iEnd; ++i) 
	    {
		Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
	    
		float x = pos.x () / 100.0;  // cm -> m
		float y = pos.y () / 100.0;  // cm -> m
		float z = pos.z () / 100.0;  // cm -> m
		
		vertices->vertex.set1Value (nVrx++, SbVec3f (x, y, z));
	    }
	    vertices->vertex.setNum (m_digis.size ());

	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (m_digis.size ());
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
