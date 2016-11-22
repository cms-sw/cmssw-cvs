//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMuonCSC/interface/VisCSCWireDigiTwig.h"
#include "VisReco/VisMuonCSC/interface/VisCSCDigiTools.h"
#include "VisReco/VisMuonCSC/interface/VisCSCChamberDrawer.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigi.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigiCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CSCGeometry/interface/CSCLayer.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

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
#include <classlib/utils/DebugAids.h>
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
	return new VisCSCWireDigiTwig (state, parent, "Wire digis (" + name + ")", 
				       friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCSCWireDigiTwig::VisCSCWireDigiTwig (IgState *state, IgTwig *parent,
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
    edm::TypeID caloJetsID (typeid (CSCWireDigiCollection));
    tfService->registerTwig (caloJetsID.friendlyClassName (), &createThisTwig);
}

void
VisCSCWireDigiTwig::onNewEvent (const edm::Event &event,
				const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_digis.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<CSCWireDigiCollection> > digiCollections;

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
	std::vector<edm::Handle<CSCWireDigiCollection> >::iterator i;
	std::vector<edm::Handle<CSCWireDigiCollection> >::iterator iEnd;
	for (i = digiCollections.begin (), iEnd = digiCollections.end (); i != iEnd; ++i) 
	{
	    const CSCWireDigiCollection& c = *(*i);
	
	    // Loop over the DetUnits with digis
	    CSCWireDigiCollection::DigiRangeIterator detUnitIt;
	    CSCWireDigiCollection::DigiRangeIterator detUnitItEnd;
	    for (detUnitIt = c.begin (), detUnitItEnd = c.end (); detUnitIt != detUnitItEnd; ++detUnitIt)
	    {
		const CSCDetId &id = (*detUnitIt).first;
		const CSCWireDigiCollection::Range &range = (*detUnitIt).second;

		// Loop over the digis of this DetUnit
		for (CSCWireDigiCollection::const_iterator digiIt = range.first;
		     digiIt != range.second;
		     ++digiIt)
		{
		    std::pair< CSCDetId, CSCWireDigi > p;
		    p.first = id;
		    p.second = (*digiIt);
		    m_digis.push_back (p);
//		    std::cout << "Hit: Id/Digi: " << id <<", " << *digiIt << std::endl; //DEBUG
		} // for digis in layer
	    } // for layers
	}
    }
    
    VisQueuedTwig::onBaseInvalidate ();
}

void
VisCSCWireDigiTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << "Total " << m_digis.size () << " CSC wire digis: " << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Position</TH>"
	 << "<TH>Wire group</TH>"
	 << "<TH>Beam-crossing tag</TH>"
	 << "<TH>Time bin</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
    
    if ( ! m_digis.empty () ) 
    {
       
	int nDigis = 0;
	try 
	{
	    for (std::vector<std::pair<CSCDetId, CSCWireDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	    {
		Surface::GlobalPoint pos( NAN, NAN, NAN );
                if (m_pDD.isValid ()) pos  =  (m_pDD->idToDet ((*i).first))->surface ().position ();
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << ++nDigis << "</TD>"
		     << "<TD>" << pos.x () << ", " << pos.y () << ", " << pos.z () << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).second.getWireGroup () << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).second.getBeamCrossingTag () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).second.getTimeBin () << "</TD>"
		     << "</TR>";
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
VisCSCWireDigiTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    //
    rep->clear ();
    m_dets.clear ();
    //
    if (! m_digis.empty () && m_pDD.isValid ())
    {
	IgTwigConfigElement *element = IgTwigConfigElement::get(state());
	SoMaterial *mat = element->material("VisCSCWireDigiTwig/3D/Layer");
	SoDrawStyle *ds = element->drawStyle("VisCSCWireDigiTwig/3D/Layer");
	VisCSCChamberDrawer d (mat, ds);

	mat = element->material("VisCSCWireDigiTwig/*/Wire");
	ds = element->drawStyle("VisCSCWireDigiTwig/*/Wire");
	SoSeparator *sep = new SoSeparator;

	SoSeparator *pulses = new SoSeparator;// wire groups
	if (mat) pulses->addChild (mat);	    
	if (ds) pulses->addChild (ds);
	sep->addChild (pulses);// wire groups

	try 
	{ 
	    std::vector<SbVec3f> vtxPts;
	
	    //
	    for (std::vector<std::pair<CSCDetId, CSCWireDigi> >::const_iterator 
		     i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i)
	    {
		const CSCDetId& id = (*i).first;
		if (m_dets.find(id) == m_dets.end() )
        	{
                    m_dets.insert(id);	
		    // This is a frame around the (active) layer
		    d.getOutline(sep, m_pDD, id);

        	}
		//
		const CSCLayer *layer_p = m_pDD->layer (id);
		const CSCLayerGeometry *layerGeom = layer_p->geometry ();
		const int wireGroup_id = ((*i).second).getWireGroup ();
		std::pair< LocalPoint, LocalPoint > ends = layerGeom->wireTopology()->wireEnds( layerGeom->middleWireOfGroup(wireGroup_id) );
		Surface::GlobalPoint pos = (m_pDD->idToDet (id))->surface ().toGlobal(ends.first);
		vtxPts.push_back(SbVec3f(pos.x()/100.0, pos.y()/100.0, pos.z()/100.0));
		pos = (m_pDD->idToDet (id))->surface ().toGlobal(ends.second);
		vtxPts.push_back(SbVec3f(pos.x()/100.0, pos.y()/100.0, pos.z()/100.0));
		
	    }
	    SoSeparator *newHits = new SoSeparator;
	    SoLineSet *ls = new SoLineSet;
	    SoCoordinate3 *coords = new SoCoordinate3;
	    coords->point.setValues(0,vtxPts.size(),&vtxPts[0]);
	    std::vector<int> nv(vtxPts.size()/2,2);
	    ls->numVertices.setValues(0,nv.size(),&nv[0]);
	    newHits->addChild(coords);
	    newHits->addChild(ls);
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
    }
}

void
VisCSCWireDigiTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
    
//     SoSeparator *sep = new SoSeparator;    
//     SoSeparator *posSep = new SoSeparator;    
//     SoSeparator *negSep = new SoSeparator;

//     SoMaterial *mat = new SoMaterial;
//     mat->diffuseColor.setValue (0.0, 0.0, 1.0);
//     posSep->addChild (mat);

//     SoMaterial *negMat = new SoMaterial;
//     negMat->diffuseColor.setValue (1.0, 0.0, 0.0);
//     negSep->addChild (negMat);

//     sep->addChild (posSep);
//     sep->addChild (negSep);

//     try 
//     {
// 	for (std::vector<std::pair<CSCDetId, CSCWireDigi> >::const_iterator i = m_digis.begin (); i != m_digis.end (); ++i) 
// 	{
// 	    Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
// 	    double eta = pos.eta ();
// 	    double phi = pos.phi ();
// 	    if (phi < 0.0) phi = 2 * M_PI + phi; // correction in absence of one convention

// // 	    int wire = (*i).second.getWireGroup ();
// // 	    int tbin = (*i).second.getBeamCrossingTag ();
	    
// 	    IgSoTower *tower = new IgSoTower;
// 	    tower->position = SbVec2f (phi, eta); // eta, phi
// 	    tower->scaleFactor = 20.0;
// 	    posSep->addChild (tower); 
// 	}
//     }
// 	catch (cms::Exception& e)
// 	{
// 	    if (this->m_onCmsException)
// 		this->m_onCmsException (&e);
// 	}
// 	catch (lat::Error &e) 
// 	{
// 	    if (this->m_onError)
// 		this->m_onError (&e);
// 	}
// 	catch (std::exception &e) 
// 	{
// 	    if (this->m_onException)
// 		this->m_onException (&e);
// 	}
// 	catch (...) 
// 	{
// 	    if (this->m_onUnhandledException)
// 		this->m_onUnhandledException ();
// 	}
    
//     rep->node ()->addChild (sep);
}

void
VisCSCWireDigiTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    //
    rep->clear ();
    m_dets.clear ();
    //
    if (! m_digis.empty () && m_pDD.isValid ())
    {
	IgTwigConfigElement *element = IgTwigConfigElement::get(state());
	SoMaterial *mat = element->material("VisCSCWireDigiTwig/RPhi");
	SoDrawStyle *ds = element->drawStyle("VisCSCWireDigiTwig/RPhi");
	
	SoSeparator *sep = new SoSeparator;// global
	sep->addChild (mat);
	sep->addChild (ds);

	SoSeparator	*wires = new SoSeparator; // wires
	mat = element->material("VisCSCWireDigiTwig/RPhi/Wire");
	ds = element->drawStyle("VisCSCWireDigiTwig/RPhi/Wire");
	if (mat) wires->addChild (mat);
	if (ds)  wires->addChild (ds);
	sep->addChild(wires);
	
	mat = element->material("VisCSCWireDigiTwig/RPhi/Layer");
	ds = element->drawStyle("VisCSCWireDigiTwig/RPhi/Layer");
	VisCSCChamberDrawer d(mat,ds);	
	//
	try 
	{ 
	    //
	    std::vector<SbVec3f> vtxPts;

	    for (std::vector<std::pair<CSCDetId, CSCWireDigi> >::const_iterator 
		     i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i)
	    {
		const CSCDetId& id = (*i).first;

		// This is a frame around the (active) layer
		if (m_dets.find(id) == m_dets.end() )
        	{
                    m_dets.insert(id);	
		    // This is a frame around the (active) layer
		    d.getOutline(sep, m_pDD, id);

        	}

		const CSCLayer *layer_p = m_pDD->layer (id);
		const CSCLayerGeometry *layerGeom = layer_p->geometry ();
		const int wireGroup_id = ((*i).second).getWireGroup ();
		std::pair< LocalPoint, LocalPoint > ends = layerGeom->wireTopology()->wireEnds( layerGeom->middleWireOfGroup(wireGroup_id) );
		Surface::GlobalPoint pos = (m_pDD->idToDet (id))->surface ().toGlobal(ends.first);
		vtxPts.push_back(SbVec3f(pos.x()/100.0, pos.y()/100.0, pos.z()/100.0)); //convert cm to m
		pos = (m_pDD->idToDet (id))->surface ().toGlobal(ends.second);
		vtxPts.push_back(SbVec3f(pos.x()/100.0, pos.y()/100.0, pos.z()/100.0));
	    }
	    SoSeparator *newHits = new SoSeparator;
	    SoLineSet *ls = new SoLineSet;
	    SoCoordinate3 *coords = new SoCoordinate3;
	    coords->point.setValues(0,vtxPts.size(),&vtxPts[0]);
	    std::vector<int> nv(vtxPts.size()/2,2);
	    ls->numVertices.setValues(0,nv.size(),&nv[0]);
	    newHits->addChild(coords);
	    newHits->addChild(ls);
	    wires->addChild(newHits);	       
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
VisCSCWireDigiTwig::update (IgSpareRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    //
    rep->clear ();
    //
    if (! m_digis.empty () && m_pDD.isValid ()) 
    {
	IgTwigConfigElement *element = IgTwigConfigElement::get(state());
	SoMaterial *mat = element->material("VisCSCWireDigiTwig/Spare");
	SoDrawStyle *ds = element->drawStyle("VisCSCWireDigiTwig/Spare");

	SoSeparator *sep = new SoSeparator;    
	if (mat) sep->addChild (mat);
	if (sep) sep->addChild(ds);
	
	SoMaterial *matmw = element->material("VisCSCWireDigiTwig/Spare/Matched/Wire");
	SoMaterial *matmf = element->material("VisCSCWireDigiTwig/Spare/Matched/Frame");

	SoMaterial *matuw = element->material("VisCSCWireDigiTwig/Spare/UnMatched/Wire");
	SoMaterial *matuf = element->material("VisCSCWireDigiTwig/Spare/UnMatched/Frame");

	SoDrawStyle *dsw = element->drawStyle("VisCSCWireDigiTwig/Spare/Wire");
	try 
	{
	    bool iterations = false;
	    float OurPhi = 9999999.;
	    float OurPhiSlice = 9999999.;
	    float OurZ = 99999999.;
	    for (std::vector<std::pair<CSCDetId, CSCWireDigi> >::const_iterator i = 
		     m_digis.begin (); i != m_digis.end (); ++i) 
	    {
		const CSCDetId& id = (*i).first;
		const CSCLayer* layer_p = m_pDD->layer(id);
		const CSCLayerGeometry* layerGeom=0;
	      
		if(layer_p){
		    layerGeom = layer_p->geometry();
		}
		else{// to be filled...
		}
		//
		Surface::GlobalPoint pos =  (m_pDD->idToDet (id))->surface ().position ();
		float layer_x = pos.x () / 100.0;  // cm -> m
		float layer_y = pos.y () / 100.0;  // cm -> m
		float layer_z = pos.z () / 100.0;  // cm -> m
		float layer_phi = pos.phi ();
		float layer_r = sqrt(layer_x*layer_x + layer_y*layer_y);
		if(!iterations){
		    // Make captions, some init
		    iterations=VisCaptions(layer_z, layer_phi, OurPhi, OurZ, sep);
		}
		const std::vector<float> trapezBounds = layerGeom->parameters();
		OurPhiSlice = 2. * atan(trapezBounds[1]/(layer_r + trapezBounds[3])) * 0.8;
		// Only adjacent chambers 
		if (fabs(layer_phi - OurPhi) < 2. * OurPhiSlice){
		    int wireGroup_id = ((*i).second).getWireGroup();
		    LocalPoint centerWireGroup=layerGeom->localCenterOfWireGroup(wireGroup_id);
		    float wireLength = layerGeom->lengthOfWireGroup(wireGroup_id);
		    // Make frames
		    SoLineSet *trapez = new SoLineSet;
		    SoLineSet *rect_s = new SoLineSet;
		    SoLineSet *rect_w = new SoLineSet;
		    // Translations
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
		    // Wire group as a line
		    SoVertexProperty *vert = new SoVertexProperty;
		    int nVrx = 0;
		    float x = (centerWireGroup.x()-wireLength/2.)/100.;
		    float y = centerWireGroup.y()/100.;
		    float z = 0.; 
		    vert->vertex.set1Value (nVrx++,SbVec3f (x, y, z) );
		    //
		    x = (centerWireGroup.x()+wireLength/2.)/100.;
		    y = centerWireGroup.y()/100.;
		    z = 0.;
		    vert->vertex.set1Value (nVrx++,SbVec3f (x, y, z) );
		    //
		    vert->vertex.setNum (nVrx);
		    SoLineSet *WireGroupVis = new SoLineSet;
		    WireGroupVis->vertexProperty.setValue (vert);
		    WireGroupVis->numVertices[2];
		    // Wire group projection - a point
		    SoVertexProperty *wireDots = new SoVertexProperty;
		    nVrx = 0;
		    x =  centerWireGroup.y()/ 100.0;  // cm -> m
		    y =  (trapezBounds[2]*1.1 -  id.layer()*2.*trapezBounds[2]/6.)/4.;
		    z = 0. / 100.0;  // cm -> m
		    if(3==id.station()) y=-y;// ...
		    wireDots->vertex.set1Value (nVrx++, SbVec3f (x, y, z));
		    wireDots->vertex.setNum (nVrx);
		    int Npoints=nVrx;
		    // Colors
		    SoMaterial *matw;
		    SoMaterial *matf;
		    if(OurPhi == layer_phi){
			matw = matmw;
			matf = matmf;
		    }
		    else{
			matw = matuw;
			matf = matuf;
		    }
		    //  Build the "picture"
		    SoSeparator *shape = new SoSeparator;// frame
		    shape->addChild (trapez);
		    //
		    SoSeparator *caption = new SoSeparator;// caption
		    caption->addChild (trans_caption);
		    caption->addChild (font_caption);
		    caption->addChild (TextCaption);
		    //
		    SoSeparator *proj_s = new SoSeparator;// projection strip frame
		    proj_s->addChild (trans_s); 
		    proj_s->addChild (rect_s);
		    //
		    SoSeparator *proj_w = new SoSeparator;// projection wire frame
		    proj_w->addChild (trans_w); 
		    proj_w->addChild (rect_w);
		    //
		    SoSeparator *dotsWG = new SoSeparator; // projection wire groups
		    SoPointSet *points = new SoPointSet;
		    points->vertexProperty.setValue (wireDots);
		    points->numPoints.setValue (Npoints);
		    dotsWG->addChild (matw); 
		    dotsWG->addChild (dsw);
		    dotsWG->addChild (trans_w);
		    dotsWG->addChild (points);
		    //
		    SoSeparator *wiregroup = new SoSeparator;// wiregroup
		    wiregroup->addChild (matw);
		    wiregroup->addChild (WireGroupVis);
		    //
		    SoSeparator *layer = new SoSeparator;// layer
		    layer->addChild (trans);
		    layer->addChild (shape);
		    layer->addChild (wiregroup);
		    // Visualization of all
		    sep->addChild (matf); // default materials
		    sep->addChild (layer);
		    sep->addChild (caption);
		    sep->addChild (proj_s);
		    sep->addChild (proj_w);
		    sep->addChild (dotsWG);
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
	//
	// FIXME: Move to another rep type. 
        rep->node ()->addChild (sep);
    }
}

void
VisCSCWireDigiTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    
    if (! m_digis.empty () && m_pDD.isValid ()) 
    {
	IgTwigConfigElement *element = IgTwigConfigElement::get(state());
	SoMaterial *mat = element->material("VisCSCWireDigiTwig/RZ");
	SoDrawStyle *ds = element->drawStyle("VisCSCWireDigiTwig/RZ");

	SoSeparator *sep = new SoSeparator;// global
	if (mat) sep->addChild (mat);
	if (ds) sep->addChild (ds);
    
		
	try 
	{
	    SoCoordinate3 *vertices = new SoCoordinate3;
	    int nVrx = 0;

	    for (std::vector<std::pair<CSCDetId, CSCWireDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	    {
		Surface::GlobalPoint pos =  (m_pDD->idToDet ((*i).first))->surface ().position ();
// 		int wire = (*i).second.getWireGroup ();
// 		int tbin = (*i).second.getBeamCrossingTag ();
	    
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
