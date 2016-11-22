//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisHcal/interface/VisHBHERecHitTwig.h"
#include "VisReco/VisHcal/interface/VisDrawTowerHelper.h"
#include "VisReco/VisHcal/interface/VisHcalSubdetector.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "FWCore/Framework/interface/Event.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoCircularHist.h"
#include "Iguana/Inventor/interface/IgSoRectColHist.h"
#include "Iguana/Inventor/interface/IgSoTower.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <QString>
#include <classlib/utils/DebugAids.h>
#include <algorithm>
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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/HCAL Event/Hcal Barrel and Endcap RecHits");
	
	if (! eventTwig) 
	    eventTwig = parent;
		
	return new VisHBHERecHitTwig (state, eventTwig, "[N/A] HB and HE RecHits (" + name + ")", 
				      friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisHBHERecHitTwig::VisHBHERecHitTwig (IgState *state, IgTwig *parent,
				      const std::string &name /* = "" */,
				      const std::string &friendlyName /* = "" */,
				      const std::string &moduleLabel /* = "" */,
				      const std::string &instanceName /* = "" */,
				      const std::string &processName /* = "" */,
				      unsigned colour /* = 0x99ff0000 */)
    : VisQueuedTwig (state, parent, name),
      m_name (name),
      m_friendlyName (friendlyName),
      m_moduleLabel (moduleLabel),
      m_instanceName (instanceName),
      m_processName (processName),
      m_binning (state, lat::CreateCallback (this, &VisHBHERecHitTwig::twigChanged)),
      m_annotation (state, lat::CreateCallback (this, &VisHBHERecHitTwig::twigChanged)),
      m_escale (state, lat::CreateCallback (this, &VisHBHERecHitTwig::twigChanged)),
      m_cut (state, lat::CreateCallback (this, &VisHBHERecHitTwig::twigChanged)),
      m_scale (state, lat::CreateCallback (this, &VisHBHERecHitTwig::twigChanged)),
      m_phiStart (state, lat::CreateCallback (this, &VisHBHERecHitTwig::twigChanged)),
      m_phiDelta (state, lat::CreateCallback (this, &VisHBHERecHitTwig::twigChanged)),
      m_rMax (state, lat::CreateCallback (this, &VisHBHERecHitTwig::twigChanged)),
      m_colorCode (state, lat::CreateCallback (this, &VisHBHERecHitTwig::twigChanged)),
      m_showHit (state, lat::CreateCallback (this, &VisHBHERecHitTwig::twigChanged)),
      m_showTower (state, lat::CreateCallback (this, &VisHBHERecHitTwig::twigChanged)),
      m_text ("no info"),
      m_recHits (2350),
      m_rgba (colour),
      m_ncolors (10),
      m_maxEnergy(1.)
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID recHitCollID (typeid (HBHERecHitCollection));
    tfService->registerTwig (recHitCollID.friendlyClassName (), &createThisTwig);
    generateColors ();
}

VisHBHERecHitTwig::~VisHBHERecHitTwig (void) 
{
    if (! m_colors.empty ())
    {
	for (std::vector<SoMaterial *>::iterator it = m_colors.begin (); it != m_colors.end (); ++it)
	{
	    (*it)->unref (); //let OIV collect these
	}
	m_colors.resize (0);
    }
}

void
VisHBHERecHitTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisHBHERecHitTwig::onNewEvent (const edm::Event &event,
			       const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_recHits.clear ();
    
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());
    
    std::vector<edm::Handle<HBHERecHitCollection> > barrelRecHits;
    try
    {
      if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
      {
	VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	event.getMany (visSel, barrelRecHits);
      }
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisHBHERecHitTwig::onNewEvent ");
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
    if (! barrelRecHits.empty ())
    {
	for (std::vector<edm::Handle<HBHERecHitCollection> >::iterator i = barrelRecHits.begin (), iEnd = barrelRecHits.end (); i != iEnd; ++i) 
	{
	    const HBHERecHitCollection& c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    this->name (nameStr.toStdString ());
	    for (std::vector<HBHERecHit>::const_iterator ibarrel = c.begin (), ibarrelEnd = c.end (); 
		 ibarrel != ibarrelEnd; ++ibarrel)
	    {
		m_recHits.push_back (*ibarrel);
		if ((*ibarrel).energy () > m_maxEnergy)
		  m_maxEnergy = (*ibarrel).energy ();
	    }
	}
    }

    IgRepSet::invalidate (this->parent (), IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);   

    if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (state ()))
        sbar->setMessage (QString::fromStdString (m_text));
}

void
VisHBHERecHitTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);

    // Prepare property description.
    std::ostringstream  text;
    text << this->name () << " from ";
    text << m_text << "<br>";
    
    if (m_friendlyName.empty () && m_moduleLabel.empty () && m_instanceName.empty () && m_processName.empty ())
    {
	text << "Please, select the branches of this Twig for more information.";	
    }
    else
    {
	text << "Displayed only above  " << m_cut.value () << " GeV:<br>";

	text << "<table width='100%' border=1>"
	     << "<TR align = center>"
	     << "<TH>Number</TH>"
	     << "<TH>Energy</TH>"
	     << "<TH>Time</TH>"
	     << "<TH>Subdetector</TH>"
	     << "<TH>ietaAbs</TH>"
	     << "<TH>ieta</TH>"
	     << "<TH>iphi [rad]</TH>"
	     << "<TH>Depth</TH>"
	     << "</TR>";
	text << setiosflags (std::ios::showpoint | std::ios::fixed);
	text.setf (std::ios::right, std::ios::adjustfield);
    
	int nDigis = 0;
	try 
	{
	    for (std::vector<HBHERecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
	    {
		float energy = (*i).energy ();
		if (energy > m_cut.value ())
		{
		    text << "<TR align = right>"
			 << "<TD>" << std::setw (3) << nDigis++ << "</TD>"
			 << "<TD>" << std::setw (5) << std::setprecision (3) << energy << "</TD>"
			 << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).time () << "</TD>"
			 << "<TD>" << std::setw (2) << VisHcalSubdetector::subDetName ((*i).id ().subdet ()) << "</TD>"
			 << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).id ().ietaAbs () << "</TD>"
			 << "<TD>" << std::setw (6) << std::setprecision (3) << (*i).id ().ieta () << "</TD>"
			 << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).id ().iphi () << "</TD>"
			 << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).id ().depth() << "</TD>"
			 << "</TR>";
		}	    
	    }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisHBHERecHitTwig::update(IgTextRep*) ");
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
	text << "</table>";
    }
    
    // Send it over.
    rep->setText (text.str ());
}

void
VisHBHERecHitTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    SoSeparator *sep = new SoSeparator;
    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (m_rgba, rgbcomponents);
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    sep->addChild (mat);

    VisDrawTowerHelper drawTowerHelper(sep);
    
    if (! m_recHits.empty ())
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	try 
	{
	  std::vector<HBHERecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end ();
	  
	  for (; i != iEnd; ++i) 
	  {
	    float energy = (*i).energy ();

	    if (energy > m_cut.value ()) 
	    {
	      const CaloCellGeometry::CornersVec& corners = esService->getCorners ((*i).detid ());
	      ASSERT (corners.size () == 8);

	      SbVec3f front1(corners [0].x () / 100.0, corners [0].y () / 100.0, corners [0].z () / 100.0);
	      SbVec3f front2(corners [1].x () / 100.0, corners [1].y () / 100.0, corners [1].z () / 100.0);
	      SbVec3f front3(corners [2].x () / 100.0, corners [2].y () / 100.0, corners [2].z () / 100.0);
	      SbVec3f front4(corners [3].x () / 100.0, corners [3].y () / 100.0, corners [3].z () / 100.0);
		    
	      SbVec3f back1(corners [4].x () / 100.0, corners [4].y () / 100.0, corners [4].z () / 100.0);
	      SbVec3f back2(corners [5].x () / 100.0, corners [5].y () / 100.0, corners [5].z () / 100.0);
	      SbVec3f back3(corners [6].x () / 100.0, corners [6].y () / 100.0, corners [6].z () / 100.0);
	      SbVec3f back4(corners [7].x () / 100.0, corners [7].y () / 100.0, corners [7].z () / 100.0);
		    
	      drawTowerHelper.addScaledBox(front1, front2, front3, front4,
					   back1, back2, back3, back4, energy/m_maxEnergy);
	    }
	  }
	}
	catch (cms::Exception& e)
	{
	    e.append (" from VisHBHERecHitTwig::update(Ig3DRep*) ");
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
    
    rep->node ()->addChild (sep);
}

void
VisHBHERecHitTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;    

    try 
    {
	if (! m_recHits.empty ())
	{
	    for (std::vector<HBHERecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
	    {
		float et = (*i).energy ();
		if (fabs (et) > m_cut.value ()) 
 		{
		    int iphi = (*i).id ().iphi ();
		    int ieta = (*i).id ().ieta ();
		    
		    float phi = (iphi - 1) * 0.087 + 0.087 / 2.0;
		    float eta = ieta * 0.087 + 0.087 / 2.0;
	    
		    IgSoTower *tower = new IgSoTower;
		    tower->position = SbVec2f (phi, eta); // eta, phi
		    tower->scaleFactor = m_scale.value ();
		    tower->energy = et;
		    tower->emFraction = 0.0; // EM/Total 0%
		    sep->addChild (tower);
 		}
	    }
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisHBHERecHitTwig::update(IgLegoRep *) ");
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

void
VisHBHERecHitTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;    
    SoSeparator *posSep = new SoSeparator;    
    SoSeparator *negSep = new SoSeparator;
    sep->addChild (posSep);
    sep->addChild (negSep);
    
    SoMaterial *mat = new SoMaterial;
    mat->ambientColor.setValue (0.0, 0.4, 1.0);
    mat->diffuseColor.setValue (0.0, 0.4, 1.0);
    mat->specularColor.setValue (0.0, 0.4, 1.0);
    mat->emissiveColor.setValue (0.0, 0.4, 1.0);
    mat->transparency.setValue (0.5);
    
    posSep->addChild (mat);
    
    SoMaterial *negMat = new SoMaterial;
    negMat->ambientColor.setValue (1.0, 0.0, 0.0);
    negMat->diffuseColor.setValue (1.0, 0.0, 0.0);
    negMat->specularColor.setValue (1.0, 0.0, 0.0);
    negMat->emissiveColor.setValue (1.0, 0.0, 0.0);
    negSep->addChild (negMat);

    int nbrOfBins = 72;
    int binNumber;

    std::vector<float> bufferPositive (nbrOfBins, 0.);
    std::vector<float> bufferNegative (nbrOfBins, 0.);
    try 
    {
	if (! m_recHits.empty ())
	{
	    for (std::vector<HBHERecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
	    {
		float energy = (*i).energy ();
		if (energy > m_cut.value ())
		{
		    if (VisHcalSubdetector::subDetName ((*i).id ().subdet ()) == "HcalBarrel")
		    {
			int iphi = (*i).id ().iphi ();
			binNumber = iphi - 1;

			if (binNumber >= 0 && binNumber < 72)
			{			  
			    if (m_binning.value () == "project")
			    {				
				if ((energy > 0.0) && (energy > bufferPositive [binNumber]))
				    bufferPositive [binNumber] = energy;
				else if ((energy < 0.0) && (energy < bufferNegative [binNumber]))
				    bufferNegative [binNumber] = energy;
			    }
			    else
			      (energy > 0.0) ?  bufferPositive [binNumber] += energy : bufferNegative [binNumber] += energy;
			}
		    }
		}
	    }

	    if (*max_element (bufferPositive.begin (), bufferPositive.end ()) > m_cut.value ())
	    {
		IgSoCircularHist *recHits = new IgSoCircularHist;
		recHits->minRadius = 1.9;
		recHits->maxRadius = m_rMax.value ();
		recHits->scaleFactor = m_escale.value ();
		recHits->numberOfBins = nbrOfBins;
		recHits->energies.setValues (0, nbrOfBins, &bufferPositive [0]);
		recHits->logScale = false;
		recHits->showAnnotations = m_annotation.value ();
		recHits->layer = 9;
		posSep->addChild (recHits);
	    }
	    
	    std::vector<float>::const_iterator nfirst = bufferNegative.begin ();
	    std::vector<float>::const_iterator nlast = bufferNegative.end ();

	    if (fabs(*max_element (nfirst, nlast)) > m_cut.value ())
	    {
		IgSoCircularHist *negRecHits = new IgSoCircularHist;
		negRecHits->minRadius = 1.9;
		negRecHits->maxRadius = -1;
		negRecHits->scaleFactor = m_escale.value ();
		negRecHits->numberOfBins = nbrOfBins;
		negRecHits->energies.setValues (0, nbrOfBins, &bufferNegative [0]);
		negRecHits->logScale = false;
		negRecHits->showAnnotations = m_annotation.value ();
		negRecHits->layer = 9;
		negSep->addChild (negRecHits);
	    }	    
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisHBHERecHitTwig::update (IgRPhiRep*) ");
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

void
VisHBHERecHitTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
	
    SoSeparator *sep = new SoSeparator;    

    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (m_rgba, rgbcomponents);
    mat->ambientColor.setValue (SbColor (rgbcomponents));
    mat->diffuseColor.setValue (SbColor (rgbcomponents));
    mat->specularColor.setValue (SbColor (rgbcomponents));
    mat->emissiveColor.setValue (SbColor (rgbcomponents));
    mat->transparency.setValue (0.5);
    sep->addChild (mat);

    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);

    unsigned binNumber = 0;
    const unsigned nBinsZ = 17*2;
    const unsigned nBinsR = 13*2;
    const unsigned nbrOfBins = 2 * (nBinsZ + nBinsR);
    const float radiusR = 1.86f;
    const float radiusZ = 3.78f;
    const float barrelMaxEta = 1.444f;            // max eta of HCAL Barrel
    const float beamPipeTheta = esService->etaToTheta (3.f);
    std::vector<float> energies (nbrOfBins * 2, 0.);  // times 2 for the other energy (which is not used)

    try 
    {
	if (! m_recHits.empty ())
	{
	    for (std::vector<HBHERecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
	    {
		float energy = (*i).energy ();
		if (energy > m_cut.value ())
		{
		    binNumber = esService->getBinNumber ((*i).detid (), nBinsR, nBinsZ, radiusR, radiusZ, barrelMaxEta, beamPipeTheta);

		    if (binNumber < nbrOfBins)
		    {
			if (m_binning.value () == "project")
			    (energies[binNumber] > energy) ? energies[binNumber] : energies[binNumber] = energy;
			else
			    energies[binNumber] += energy;
		    }
		    else
		    {
			std::cerr << "Bin number (" << binNumber << ") exceeds the number of bins (" <<  nbrOfBins << ")"<< std::endl;
		    }
		}
	    }
			
	    if (*max_element (energies.begin (), energies.end ()) > m_cut.value ())
	    {
		SoMaterial *mat = new SoMaterial;
		mat->ambientColor.setValue (0.0, 0.4, 1.0);
		mat->diffuseColor.setValue (0.0, 0.4, 1.0);
		mat->specularColor.setValue (0.0, 0.4, 1.0);
		mat->emissiveColor.setValue (0.0, 0.4, 1.0);

		SoDrawStyle *drawStyle = new SoDrawStyle;
		drawStyle->lineWidth.setValue (2.0);

		IgSoRectColHist* hist = new IgSoRectColHist;
		hist->radiusR = radiusR;
		hist->radiusZ = radiusZ;
		if (m_rMax.value () > 0) hist->maxDist = m_rMax.value ();		
		hist->numR = nBinsR;
		hist->numZ = nBinsZ;
		hist->energies.setValues (0, energies.size(), &energies [0]);
		hist->logScale = false;
		hist->layer = -5.0;
		hist->scaleFactor = m_escale.value ();
		hist->barrelMaxEta = barrelMaxEta;
		hist->beamPipeTheta = beamPipeTheta;
				
		std::vector<SbColor> colors (4);
		colors[0] = SbColor (0.0, 0.3, 1.0); // first energy's positive color 
		colors[1] = SbColor (1.0, 0.0, 1.0); // first energy's negative color
		colors[2] = SbColor (1.0, 0.4, 0.0); // second energy's positive color (not used)
		colors[3] = SbColor (1.0, 1.0, 0.0); // second energy's negative color (not used)
				
		hist->faceColors.setValues (0, colors.size (), &colors[0]);
		sep->addChild (mat);
		sep->addChild (drawStyle);
		sep->addChild (hist);
	    }
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisHBHERecHitTwig::update(IgRZRep*) ");
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

SoMaterial * 
VisHBHERecHitTwig::energyColor (const float energy)
{
    // determine color (energy)
    int color_ndx = static_cast<int>((energy - m_cut.value ()) * m_ncolors / (m_maxEnergy - m_cut.value ()));
    color_ndx = std::min (color_ndx, m_ncolors - 1);
    return m_colors [color_ndx];
}

void
VisHBHERecHitTwig::generateColors (void)
{
    float hue = 0.66667F; // reverse load to get low to high: hue 0 -> red, 2/3->blue
    float saturation= 1.0F; 
    float value = 1.0F;
    SbColor hsvColor;
    float delta_hue = hue / m_ncolors;
    if (m_colors.empty ())
    {
	m_colors.reserve (m_ncolors);
	for (int i = 0; i < m_ncolors; i++, hue -= delta_hue)
	{
	    hsvColor.setHSVValue (hue, saturation, value);
	    SoMaterial *material = new SoMaterial;
	    material->diffuseColor.setValue (hsvColor);
	    m_colors.push_back (material);
	    material->ref (); // make sure OIV doesn't garbage collect 
	}
    }
    else
    {
	for (std::vector<SoMaterial *>::iterator it = m_colors.begin (); it != m_colors.end (); it++, hue -= delta_hue)
	{
	    hsvColor.setHSVValue (hue, saturation, value);
	    (*it)->diffuseColor.setValue (hsvColor);
	}
    }
}


