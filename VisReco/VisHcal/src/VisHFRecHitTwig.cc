//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisHcal/interface/VisHFRecHitTwig.h"
#include "VisReco/VisHcal/interface/VisDrawTowerHelper.h"
#include "VisReco/VisHcal/interface/VisHcalSubdetector.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HFRecHit.h"
#include "FWCore/Framework/interface/Event.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/HCAL Event/Hcal Forward RecHits");
	
	if (! eventTwig) 
	    eventTwig = parent;
		
	return new VisHFRecHitTwig (state, eventTwig, "[N/A] HF RecHits (" + name + ")", 
				    friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisHFRecHitTwig::VisHFRecHitTwig (IgState *state, IgTwig *parent,
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
      m_binning (state, lat::CreateCallback (this, &VisHFRecHitTwig::twigChanged)),
      m_annotation (state, lat::CreateCallback (this, &VisHFRecHitTwig::twigChanged)),
      m_escale (state, lat::CreateCallback (this, &VisHFRecHitTwig::twigChanged)),
      m_cut (state, lat::CreateCallback (this, &VisHFRecHitTwig::twigChanged)),
      m_scale (state, lat::CreateCallback (this, &VisHFRecHitTwig::twigChanged)),
      m_phiStart (state, lat::CreateCallback (this, &VisHFRecHitTwig::twigChanged)),
      m_phiDelta (state, lat::CreateCallback (this, &VisHFRecHitTwig::twigChanged)),
      m_text ("no info"),
      m_recHits (880),
      m_rgba (colour),
      m_maxEnergy(1.)
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID recHitCollID (typeid (HFRecHitCollection));
    tfService->registerTwig (recHitCollID.friendlyClassName (), &createThisTwig);
}

void
VisHFRecHitTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisHFRecHitTwig::onNewEvent (const edm::Event &event,
			     const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_recHits.clear ();

    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());
    
    std::vector<edm::Handle<HFRecHitCollection> > recHits;
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, recHits);
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisHFRecHitTwig::onNewEvent ");
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
    if (! recHits.empty ())
    {
	for (std::vector<edm::Handle<HFRecHitCollection> >::iterator i = recHits.begin (), iEnd = recHits.end (); i != iEnd; ++i) 
	{
	    const HFRecHitCollection& c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    this->name (nameStr.toStdString ());
	    for (std::vector<HFRecHit>::const_iterator ci = c.begin (), ciEnd = c.end (); 
		 ci != ciEnd; ++ci)
	    {
		m_recHits.push_back (*ci);
		if ((*ci).energy () > m_maxEnergy)
		  m_maxEnergy = (*ci).energy ();
	    }
	}
    }
    
    VisQueuedTwig::onBaseInvalidate ();

    if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (state ()))
        sbar->setMessage (QString::fromStdString (m_text));
}

void
VisHFRecHitTwig::update (IgTextRep *rep)
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
	    for (std::vector<HFRecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
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
	    e.append (" from VisHFRecHitTwig::update(IgTextRep*) ");
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
VisHFRecHitTwig::update (Ig3DRep *rep)
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
	    for (std::vector<HFRecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
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
	    e.append (" from VisHFRecHitTwig::update(Ig3DRep*) ");
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
VisHFRecHitTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;    

    try 
    {
	if (! m_recHits.empty ())
	{
	    for (std::vector<HFRecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
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
		    sep->addChild (tower);
		}
	    }
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisHFRecHitTwig::update(IgLegoRep *) ");
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
VisHFRecHitTwig::update (IgRZRep *rep)
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
    sep->addChild (mat);

    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);
	
    unsigned binNumber = 0;
    const unsigned nBinsZ = 2;
    const unsigned nBinsR = 2*13;
    const unsigned nbrOfBins = 2 * (nBinsZ + nBinsR);
    const float radiusR = 1.29f;
    const float radiusZ = 11.f;
//     const float maxEta = 5.0;
//     const float minEta = 2.72;
	
    std::vector<float> energies (nbrOfBins * 2, 0.); // times 2 for the other energy (which is not used)

    try 
    {
	if (! m_recHits.empty ())
	{
	    for (std::vector<HFRecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
	    {
		float energy = (*i).energy ();
		if (energy > m_cut.value ()) 
		{
		    binNumber = esService->getBinNumber ((*i).detid (), nBinsR, nBinsZ, radiusR, radiusZ);		   
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
		mat->diffuseColor.setValue (0.0, 0.1, 1.0);
		mat->specularColor.setValue (0.0, 0.1, 1.0);
		mat->emissiveColor.setValue (0.0, 0.1, 1.0);

		SoDrawStyle *drawStyle = new SoDrawStyle;
		drawStyle->lineWidth.setValue (2.0);

		IgSoRectColHist* hist = new IgSoRectColHist;
		hist->radiusR = radiusR;
		hist->radiusZ = radiusZ;
		hist->numR = nBinsR;
		hist->numZ = nBinsZ;
		hist->energies.setValues (0, energies.size(), &energies [0]);
		hist->logScale = false;
		hist->layer = -5.0;
		hist->scaleFactor = m_escale.value ();
				
		std::vector<SbColor> colors (4);
		colors[0] = SbColor (0.0, 0.3, 1.0);
		colors[1] = SbColor (1.0, 0.0, 1.0);
		colors[2] = SbColor (1.0, 0.4, 0.0);
		colors[3] = SbColor (1.0, 1.0, 0.0);
				
		hist->faceColors.setValues (0, colors.size (), &colors[0]);
		sep->addChild (mat);
		sep->addChild (drawStyle);
		sep->addChild (hist);
	    }
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisHFRecHitTwig::update(IgRZRep*) ");
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
