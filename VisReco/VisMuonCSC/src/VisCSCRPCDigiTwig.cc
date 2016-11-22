//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMuonCSC/interface/VisCSCRPCDigiTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCChamberDrawer.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/CSCDigi/interface/CSCRPCDigi.h"
#include "DataFormats/CSCDigi/interface/CSCRPCDigiCollection.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/GeometrySurface/interface/TrapezoidalPlaneBounds.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoCoordinate3.h>
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
	return new VisCSCRPCDigiTwig (state, parent, "RPC digis (" + name + ")", 
				      friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCSCRPCDigiTwig::VisCSCRPCDigiTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID caloJetsID (typeid (CSCRPCDigiCollection));
    tfService->registerTwig (caloJetsID.friendlyClassName (), &createThisTwig);
}

void
VisCSCRPCDigiTwig::onNewEvent (const edm::Event &event,
			       const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_digis.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<CSCRPCDigiCollection> > digiCollections;
    
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
	std::vector<edm::Handle<CSCRPCDigiCollection> >::iterator i;
	std::vector<edm::Handle<CSCRPCDigiCollection> >::iterator iEnd;
	for (i = digiCollections.begin (), iEnd = digiCollections.end (); i != iEnd; ++i) 
	{
	    const CSCRPCDigiCollection& c = *(*i);
    
	    // Loop over the DetUnits with digis
	    CSCRPCDigiCollection::DigiRangeIterator detUnitIt;
	    CSCRPCDigiCollection::DigiRangeIterator detUnitItEnd;
	    for (detUnitIt = c.begin (), detUnitItEnd = c.end (); detUnitIt != detUnitItEnd; ++detUnitIt)
	    {
		const CSCDetId &id = (*detUnitIt).first;
		const CSCRPCDigiCollection::Range &range = (*detUnitIt).second;

		// Loop over the digis of this DetUnit
		for (CSCRPCDigiCollection::const_iterator digiIt = range.first;
		     digiIt != range.second;
		     ++digiIt)
		{
		    std::pair< CSCDetId, CSCRPCDigi > p;
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
VisCSCRPCDigiTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "Total " << m_digis.size () << " RPC digis: " << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Position</TH>"
	 << "<TH>RPC</TH>"
	 << "<TH>Pad number</TH>"
	 << "<TH>Tbin number</TH>"
	 << "<TH>BX</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    if ((! m_digis.empty ()) && m_pDD.isValid ()) 
    {
    
	int nDigis = 0;
	try 
	{
	    for (std::vector<std::pair<CSCDetId, CSCRPCDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	    {
		Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << ++nDigis << "</TD>"
		     << "<TD>" << pos.x () << ", " << pos.y () << ", " << pos.z () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.getRpc () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.getPad () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.getTbin () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.getBXN () << "</TD></TR>";
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

void
VisCSCRPCDigiTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
    m_dets.clear ();

    if ((! m_digis.empty ()) && m_pDD.isValid ())
    {
	VisCSCChamberDrawer d;

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
	    
	    SoCoordinate3 *vertices = new SoCoordinate3;
	    int nVrtx = 0;
	    for (std::vector<std::pair<CSCDetId, CSCRPCDigi> >::const_iterator 
		     i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i)
	    {
		const CSCDetId& id = (*i).first;
		// This is the center of the (active) layer 
		Surface::GlobalPoint pos =  (m_pDD->idToDet (id))->surface ().position ();
		float x = pos.x () / 100.0;  // cm -> m
		float y = pos.y () / 100.0;  // cm -> m
		float z = pos.z () / 100.0;  // cm -> m
		vertices->point.set1Value (nVrtx++, SbVec3f (x, y, z));

		// This is a frame around the (active) layer
		if (m_dets.find(id) == m_dets.end() )
                {
                    m_dets.insert(id);	
		    // This is a frame around the (active) layer
//		    d.getOutline(sep, m_pDD, id);
                }		    
	    }
	    vertices->point.setNum (m_digis.size ());
	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (m_digis.size ());
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
VisCSCRPCDigiTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;    

    try 
    {
// 	int n = 0;
	
// 	for (std::vector<std::pair<CSCDetId, CSCRPCDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
// 	{
// 	    Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
// 	    double eta = pos.eta ();
// 	    double phi = pos.phi ();
// 	    if (phi < 0.0) phi = 2 * M_PI + phi; // correction in absence of one convention

	    // int strip = (*i).second.getRPC ();
// 	    std::vector<int> adcCounts = (*i).second.getADCCounts ();

// 	    for (std::vector<int>::const_iterator k = adcCounts.begin (); k != adcCounts.end (); ++k)
// 	    {
// 		IgSoTower *tower = new IgSoTower;
// 		tower->position = SbVec2f (phi, eta); // eta, phi
// 		tower->scaleFactor = 20.0;
// 		tower->energy = (*k);
// 		posSep->addChild (tower);
// 	    }
// 	    n++;
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
    
    rep->node ()->addChild (sep);
}

void
VisCSCRPCDigiTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
    
    if (! m_digis.empty () && m_pDD.isValid ()) 
    {
	SoSeparator *sep = new SoSeparator;    

	SoMaterial *mat = new SoMaterial;
	mat->diffuseColor.setValue (0.3, 0.8, 0.0);
	sep->addChild (mat);

// 	int nbrOfBins = 72;
// 	int binNumber;

// 	std::vector<float> bufferPositive (nbrOfBins);
	
	try 
	{
// 	    for (std::vector<std::pair<CSCDetId, CSCRPCDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
// 	    {
// 		Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
// 		double phi = pos.phi ();
// 		binNumber = (int) floor (phi / ( 2 * M_PI / nbrOfBins));
		
// // 		int strip = (*i).second.getRPC ();
// // 		std::vector<int> adcCounts = (*i).second.getADCCounts ();
		
// 		bufferPositive [binNumber] += 100.0;
// 	    }
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
VisCSCRPCDigiTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if (! m_digis.empty () && m_pDD.isValid ()) 
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

	    for (std::vector<std::pair<CSCDetId, CSCRPCDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	    {
		Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
// 		int strip = (*i).second.getRPC ();
// 		std::vector<int> adcCounts = (*i).second.getADCCounts ();
	    
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
