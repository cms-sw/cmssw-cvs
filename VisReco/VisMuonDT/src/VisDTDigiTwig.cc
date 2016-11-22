#include "VisReco/VisMuonDT/interface/VisDTDigiTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/DTDigi/interface/DTDigi.h"
#include "DataFormats/DTDigi/interface/DTDigiCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTransform.h>
#include <QString>
#include <iostream>
#include <sstream>
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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/Muon DT Event/DT Digis");
	
	if (! eventTwig)
	    eventTwig = parent;

	return new VisDTDigiTwig (state, eventTwig, "[N/A] DT Digi (" + name + ")", 
				  friendlyName, modLabel, instanceName, processName);
    }
  /** Get rotation of a DetUnit */ 
  void
  createRotation (const GeomDetUnit *det, SbVec3f & axis, float & angle)
  {    
    // Extract the coeff of the rotation matrix
    // with a projection on the basis vectors
    TkRotation< float > rot = det->surface ().rotation ();
    Basic3DVector< float > nx (1., 0., 0.); // x basis vector
    Basic3DVector< float > ny (0., 1., 0.); // y basis vector
    Basic3DVector< float > nz (0., 0., 1.); // z basis vector
    Basic3DVector< float > px = rot * nx; // Rxx Ryx Rzx
    Basic3DVector< float > py = rot * ny; // Rxy Ryy Rzy
    Basic3DVector< float > pz = rot * nz; // Rxz Ryz Rzz
    /* the rotation matrix is
     *	Rxx Rxy Rxz
     *	Ryx Ryy Ryz
     *	Rzx Rzy Rzz
     */
    // SbMatrix in row major order.
    // The 4th dimension is set empiricly set to
    // the identity
    SbMatrix sbm (px.x (), py.x (), pz.x (), 0,
		  px.y (), py.y (), pz.y (), 0,
		  px.z (), py.z (), pz.z (), 0,
		  0, 0, 0, 1);
    SbRotation sbr (sbm);

    float x;
    float y;
    float z;

    sbr.getValue (axis, angle);
    axis.getValue (x, y, z);
  }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisDTDigiTwig::VisDTDigiTwig (IgState *state, IgTwig *parent,
			      const std::string &name /* = "" */,
			      const std::string &friendlyName /* = "" */,
			      const std::string &moduleLabel /* = "" */,
			      const std::string &instanceName /* = "" */,
			      const std::string &processName /* = "" */,
			      //			      int layer,
			      unsigned color,
			      int wheel,
			      int sector) 
  //    : VisQueuedTwig (state, parent, name + QString (" (Superlayer # %1)").arg (layer).latin1 ()),
    : VisQueuedTwig (state, parent, name),
      m_text ("no info"),
      m_friendlyName (friendlyName),
      m_moduleLabel (moduleLabel),
      m_instanceName (instanceName),
      m_processName (processName),
//       m_layer (layer),
      m_rgba (color),
      m_wheel (wheel),
      m_sector (sector),
      m_annotate (false)
{
  //    m_rgba = 0x66ff0000;
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID dtDigisID (typeid (DTDigiCollection));
    tfService->registerTwig (dtDigisID.friendlyClassName (), &createThisTwig);
}

void
VisDTDigiTwig::onNewEvent (const edm::Event &event,
			   const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_digis.clear ();
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<DTDigiCollection> > digiCollections;
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, digiCollections);
	}
// 	else
// 	{
// 	    event.getManyByType (digiCollections);
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

    try
    {
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
	std::vector<edm::Handle<DTDigiCollection> >::iterator i;
	std::vector<edm::Handle<DTDigiCollection> >::iterator iEnd;
	for (i = digiCollections.begin (), iEnd = digiCollections.end (); i != iEnd; ++i) 
	{
	    const DTDigiCollection& c = *(*i);
	    int size = 0;

	    // Loop over the DetUnits with digis
// 	    for (DTDigiCollection::const_iterator dgi = c.begin ();
// 		 dgi != c.end (); ++dgi) 
// 	    {
// 		m_digi.push_back (*dgi);
// 	    }
	    DTDigiCollection::DigiRangeIterator detUnitIt;
	    DTDigiCollection::DigiRangeIterator detUnitItEnd;
	    for (detUnitIt = c.begin (), detUnitItEnd = c.end (); detUnitIt != detUnitItEnd; ++detUnitIt, ++size)
	    {
		const DTLayerId& id = (*detUnitIt).first;

		const DTDigiCollection::Range &range = (*detUnitIt).second;

		// Loop over the digis of this DetUnit
		for (DTDigiCollection::const_iterator digiIt = range.first;
		     digiIt != range.second;
		     ++digiIt)
		{
		    std::pair< DTLayerId, DTDigi > p;
		    p.first = id;
		    p.second = (*digiIt);
		    m_digis.push_back (p);
		} // for digis in layer
	    } // for layers
	    
	    QString sizeStr = (QString ("%1").arg (size));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    this->name (nameStr.toStdString ());
	}
    }
        
    IgRepSet::invalidate (this->parent (), IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);   
}

void
VisDTDigiTwig::update (IgTextRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);

  // Prepare property description.
  std::ostringstream  text;
  if (m_friendlyName.empty () && m_moduleLabel.empty () && m_instanceName.empty () && m_processName.empty ())
  {
    text << "Please, select the branches of this Twig for more information.";	
  }
  else
  {
    text << "Total " << m_digis.size () << " DT digis from";
    text << " run: " << m_text << "<br>";

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Wheel</TH>"
	 << "<TH>Station</TH>"
	 << "<TH>Sector</TH>"
	 << "<TH>SL</TH>"
	 << "<TH>Layer</TH>"
	 << "<TH>Cell</TH>"
	 << "<TH>number</TH>"
	 << "<TH>countsTDC</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
    
    if ((! m_digis.empty ()) &&  m_pDD.isValid ())
    {
      int nDigis = 0;
      try 
      {
	for (std::vector<std::pair<DTLayerId, DTDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	{
	  const DTLayer *layer = m_pDD->layer ((*i).first);

	  bool draw = true;
	  const DTChamber *chamber = layer->chamber ();
	  const DTChamberId chamberId = chamber->id ();

	  ((m_sector == 0 || m_sector == chamberId.sector ()) && draw) ? draw = true : draw = false;
	  ((m_wheel == 10 || m_wheel == chamberId.wheel ()) && draw) ? draw = true : draw = false;
	
	  if (draw)
	  {
	    int wireNumber = (*i).second.wire ();
	    int layerIdDg = (*i).first.layer();
	    int slayerIdDg = (*i).first.superlayerId().superLayer();
	    int sectorIdDg = (*i).first.superlayerId().chamberId().sector();
	    int stationIdDg = (*i).first.superlayerId().chamberId().station();
	    int wheelIdDg = (*i).first.superlayerId().chamberId().wheel();
		  
	    text << "<TR align = right>"
		 << "<TD>" << std::setw (3) << nDigis++ << "</TD>"
		 << "<TD>" << wheelIdDg << "</TD>"
		 << "<TD>" << stationIdDg << "</TD>"
		 << "<TD>" << sectorIdDg << "</TD>"
		 << "<TD>" << slayerIdDg << "</TD>"
		 << "<TD>" << layerIdDg << "</TD>"
		 << "<TD>" << std::setw (6) << std::setprecision (3) << wireNumber << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).second.number () << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).second.countsTDC () << "</TD>"
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
  }
  
  rep->setText (text.str ());
}

void
VisDTDigiTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if ((! m_digis.empty ()) &&  m_pDD.isValid ())
    {
	SoSeparator *sep = new SoSeparator;    

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (m_rgba, rgbcomponents);
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);

// 	SoDrawStyle *drawStyle = new SoDrawStyle;
// 	drawStyle->pointSize = 3.0;
// 	drawStyle->style = SoDrawStyle::LINES;
// 	drawStyle->lineWidth.setValue (2.0);
// 	sep->addChild (drawStyle);

	try 
	{ 
	    for (std::vector<std::pair<DTLayerId, DTDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	    {
	      SoSeparator* separator = new SoSeparator;		
	      addWire (separator, *i);
	      sep->addChild (separator);
// 		    if (m_annotate)
// 		      {
// 			sep->addChild (addText ());
// 		      }
		
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
VisDTDigiTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;    
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
// 	for (std::vector<std::pair<DTLayerId, DTDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
// 	{
// 	    int et = (*i).second.countsTDC ();
// 	    if (et > 0) 
// 	    {
// 		// FIXME: add a sensible rep here.
// 	    }
// 	}
//     }
//     catch (cms::Exception& e)
//     {
// 	if (this->m_onCmsException)
// 	    this->m_onCmsException (&e);
//     }
//     catch (lat::Error &e) 
//     {
// 	if (this->m_onError)
// 	    this->m_onError (&e);
//     }
//     catch (std::exception &e) 
//     {
// 	if (this->m_onException)
// 	    this->m_onException (&e);
//     }
//     catch (...) 
//     {
// 	if (this->m_onUnhandledException)
// 	    this->m_onUnhandledException ();
//     }
    
    rep->node ()->addChild (sep);
}

void
VisDTDigiTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    rep->clear ();

    if ((! m_digis.empty ()) &&  m_pDD.isValid ())
    {
	SoSeparator *sep = new SoSeparator;    

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (m_rgba, rgbcomponents);
	mat->ambientColor.setValue (SbColor (rgbcomponents));
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	mat->specularColor.setValue (SbColor (rgbcomponents));
	mat->emissiveColor.setValue(SbColor (rgbcomponents));
	sep->addChild (mat);


// 	float rgbcomponents [4];
// 	float hsvcomponents [4];
// 	float wrgbcomponents [4];
//	IgSbColorMap::unpack (m_rgba, rgbcomponents);
// 	IgSbColorMap::rgb2hsv (rgbcomponents, hsvcomponents);
// 	hsvcomponents [2] *= 0.5;
// 	IgSbColorMap::hsv2rgb (hsvcomponents, wrgbcomponents);
// 	IgSbColorMap::pack (wrgbcomponents);
	
// 	SoSeparator *wsep = new SoSeparator;    
// 	SoMaterial *wmat = new SoMaterial;
// 	wmat->ambientColor.setValue (SbColor (wrgbcomponents));
// 	wmat->diffuseColor.setValue (SbColor (wrgbcomponents));
// 	wmat->specularColor.setValue (SbColor (wrgbcomponents));
// 	wmat->emissiveColor.setValue (SbColor (wrgbcomponents));
// 	wsep->addChild (wmat);

// 	SoDrawStyle *wdrawStyle = new SoDrawStyle;
// 	wdrawStyle->style = SoDrawStyle::LINES;
// 	wdrawStyle->lineWidth.setValue (1.0);
// 	wsep->addChild (wdrawStyle);
// 	sep->addChild (wsep);

// 	SoSeparator *fsep = new SoSeparator;    
// 	SoMaterial *fmat = new SoMaterial;
// 	fmat->ambientColor.setValue (SbColor (rgbcomponents));
// 	fmat->diffuseColor.setValue (SbColor (rgbcomponents));
// 	fmat->specularColor.setValue (SbColor (rgbcomponents));
// 	fmat->emissiveColor.setValue (SbColor (rgbcomponents));
// 	fsep->addChild (fmat);
// 	SoDrawStyle *fdrawStyle = new SoDrawStyle;
// 	fdrawStyle->style = SoDrawStyle::FILLED;
// 	fsep->addChild (fdrawStyle);
// 	sep->addChild (fsep);

	try 
	{
	    for (std::vector<std::pair<DTLayerId, DTDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	    {
		SoSeparator* separator = new SoSeparator;		
		addWire (separator, *i);
		sep->addChild (separator);
// 		wsep->addChild (separator);
// 		fsep->addChild (separator);
		if (m_annotate)
		{
		    sep->addChild (addText (*i));
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
VisDTDigiTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    if ((! m_digis.empty ()) &&  m_pDD.isValid ())
    {
	SoSeparator *sep = new SoSeparator;    

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (m_rgba, rgbcomponents);
	mat->ambientColor.setValue (SbColor (rgbcomponents));
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	mat->specularColor.setValue (SbColor (rgbcomponents));
	mat->emissiveColor.setValue(SbColor (rgbcomponents));
	sep->addChild (mat);

// 	float rgbcomponents [4];
// 	float hsvcomponents [4];
// 	float wrgbcomponents [4];
// 	IgSbColorMap::unpack (m_rgba, rgbcomponents);
// 	IgSbColorMap::rgb2hsv (rgbcomponents, hsvcomponents);
// 	hsvcomponents [2] *= 0.5;
// 	IgSbColorMap::hsv2rgb (hsvcomponents, wrgbcomponents);
// 	IgSbColorMap::pack (wrgbcomponents);

// 	SoSeparator *wsep = new SoSeparator;    
// 	SoMaterial *wmat = new SoMaterial;
// 	wmat->ambientColor.setValue (SbColor (wrgbcomponents));
// 	wmat->diffuseColor.setValue (SbColor (wrgbcomponents));
// 	wmat->specularColor.setValue (SbColor (wrgbcomponents));
// 	wmat->emissiveColor.setValue (SbColor (wrgbcomponents));
// 	wsep->addChild (wmat);

// 	SoDrawStyle *wdrawStyle = new SoDrawStyle;
// 	wdrawStyle->style = SoDrawStyle::LINES;
// 	wdrawStyle->lineWidth.setValue (1.0);
// 	wsep->addChild (wdrawStyle);
// 	sep->addChild (wsep);

// 	SoSeparator *fsep = new SoSeparator;    
// 	SoMaterial *fmat = new SoMaterial;
// 	fmat->ambientColor.setValue (SbColor (rgbcomponents));
// 	fmat->diffuseColor.setValue (SbColor (rgbcomponents));
// 	fmat->specularColor.setValue (SbColor (rgbcomponents));
// 	fmat->emissiveColor.setValue (SbColor (rgbcomponents));
// 	fsep->addChild (fmat);
// 	SoDrawStyle *fdrawStyle = new SoDrawStyle;
// 	fdrawStyle->style = SoDrawStyle::FILLED;
// 	fsep->addChild (fdrawStyle);
// 	sep->addChild (fsep);

	try 
	{ 
	    for (std::vector<std::pair<DTLayerId, DTDigi> >::const_iterator i = m_digis.begin (), iEnd = m_digis.end (); i != iEnd; ++i) 
	    {
		int sectorIdDg = (*i).first.superlayerId().chamberId().sector();
		if (sectorIdDg == 4 || sectorIdDg == 10 || sectorIdDg == 13 || sectorIdDg == 14)
		{		    
		    SoSeparator* separator = new SoSeparator;		
		    addWire (separator, *i);
		    sep->addChild (separator);
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
VisDTDigiTwig::annotate (bool flag)
{
    m_annotate = flag;
}


/** Transform and add the wire to the scene graph */
void
VisDTDigiTwig::addWire (SoSeparator *separator, const std::pair<DTLayerId, DTDigi> &digi)
{    
    Surface::GlobalPoint cpos (0.0, 0.0, 0.0);
    
    const DTLayer *layer = m_pDD->layer (digi.first);

    const DTChamber* chamber = layer->chamber();
    const DTChamberId chamberId = chamber->id();

    bool draw = true;
    ((m_sector == 0 || m_sector == chamberId.sector()) && draw) ? draw = true : draw = false;
    ((m_wheel == 10 || m_wheel == chamberId.wheel()) && draw) ? draw = true : draw = false;		
    if (draw)
    {
	const GeomDetUnit *det = m_pDD->idToDetUnit (digi.first);
	int wireNumber = digi.second.wire ();
	
	// Position of the wire in the Layer's reference frame
	float localXPos = layer->specificTopology ().wirePosition (wireNumber);
	LocalPoint localPos (localXPos, 0, 0);
	// Position of the wire in the CMS's ref.frame
	cpos = layer->surface ().toGlobal (localPos);
	
	float posx = cpos.x () / 100.0;  // cm -> m
	float posy = cpos.y () / 100.0;  // cm -> m
	float posz = cpos.z () / 100.0;  // cm -> m
	
	float angle;
	SbVec3f axis;
	createRotation (det, axis, angle);
	
	SoTransform *transform = new SoTransform;
	transform->translation.setValue (posx, posy, posz);    
	transform->rotation.setValue (axis, angle);
	
	SoCube *cube = new SoCube;
	cube->width = layer->specificTopology ().cellWidth () / 100.0;   // cm -> m
	cube->height = layer->specificTopology ().cellLenght () / 100.0; // cm -> m
	cube->depth =  layer->specificTopology ().cellHeight () / 100.0; // cm -> m
	
	separator->addChild (transform);
	separator->addChild (cube);
    }
}

SoSeparator*	 	
VisDTDigiTwig::addText (const std::pair<DTLayerId, DTDigi> &digi)
{
    SoSeparator *sep = new SoSeparator;    
    SoMaterial *mat = new SoMaterial;
    SoDrawStyle *style = new SoDrawStyle;
    style->style = SoDrawStyle::LINES;
    sep->addChild (mat);
    sep->addChild (style);

    //    Surface::GlobalPoint cpos (0.0, 0.0, 0.0);
    
    const DTLayer *layer = m_pDD->layer (digi.first);

    const DTChamber* chamber = layer->chamber();
    const DTChamberId chamberId = chamber->id();

    bool draw = true;
    ((m_sector == 0 || m_sector == chamberId.sector()) && draw) ? draw = true : draw = false;
    ((m_wheel == 10 || m_wheel == chamberId.wheel()) && draw) ? draw = true : draw = false;
    
    if (draw) 
    {
	SoSeparator* separator = new SoSeparator;
	addDescription (separator, digi);
	sep->addChild (separator);
    }
    
    return sep;
}


void
VisDTDigiTwig::addDescription (SoSeparator *separator, const std::pair<DTLayerId, DTDigi> &digi)
{    
    // position of the wafer's center
    //     float posx = det->surface ().position ().x () / 100.0;  // cm -> m
    //     float posy = det->surface ().position ().y () / 100.0;  // cm -> m
    //     float posz = det->surface ().position ().z () / 100.0;  // cm -> m
  
    //     float angle;
    //     SbVec3f axis;
    //    createRotation (det, axis, angle);
  
    //     SoTransform *transform = new SoTransform;
    //     transform->translation.setValue (posx, posy, posz);
    //     transform->rotation.setValue (axis, angle);

    Surface::GlobalPoint cpos (0.0, 0.0, 0.0);
  
    const DTLayer *layer = m_pDD->layer (digi.first);

    const GeomDetUnit *det = m_pDD->idToDetUnit (digi.first);
    int wireNumber = digi.second.wire ();
	
    // Position of the wire in the Layer's reference frame
    float localXPos = layer->specificTopology ().wirePosition (wireNumber);
    LocalPoint localPos (localXPos, 0, 0);
    // Position of the wire in the CMS's ref.frame
    cpos = layer->surface ().toGlobal (localPos);
	
    float posx = cpos.x () / 100.0;  // cm -> m
    float posy = cpos.y () / 100.0;  // cm -> m
    float posz = cpos.z () / 100.0;  // cm -> m
	
    float angle;
    SbVec3f axis;
    createRotation (det, axis, angle);
	
    SoTransform *transform = new SoTransform;
    transform->translation.setValue (posx, posy, posz);    
    transform->rotation.setValue (axis, angle);

    //  int wireNumber = digi.second.wire ();
  
    QString label = QString ("%1").arg (wireNumber);
    SoSeparator *labelSep = new SoSeparator;
    SoText2  *cLabel = new SoText2;
    cLabel->string = label.latin1 ();
    labelSep->addChild (cLabel);
  
    separator->addChild (transform);
    separator->addChild (labelSep);
}
