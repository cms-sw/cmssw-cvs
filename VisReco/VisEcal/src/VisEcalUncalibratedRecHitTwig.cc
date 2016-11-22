//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisEcal/interface/VisEcalUncalibratedRecHitTwig.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "VisReco/VisEcal/interface/VisEcalSubdetector.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "FWCore/Framework/interface/Event.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoCrystalHit.h"
#include "Iguana/Inventor/interface/IgSoCircularHist.h"
#include "Iguana/Inventor/interface/IgSoRectColHist.h"
#include "Iguana/Inventor/interface/IgSoTower.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <qapplication.h>
#include <qstring.h>
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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/ECAL Event/Uncalibrated Rec Hits");
	
	if (! eventTwig)
	    eventTwig = parent;

	return new VisEcalUncalibratedRecHitTwig (state, eventTwig, "[N/A] Uncalibrated Ecal Rec Hit (" + name + ")", 
						  friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisEcalUncalibratedRecHitTwig::VisEcalUncalibratedRecHitTwig (IgState *state, IgTwig *parent,
							      const std::string &name /* = "" */,
							      const std::string &friendlyName /* = "" */,
							      const std::string &moduleLabel /* = "" */,
							      const std::string &instanceName /* = "" */,
							      const std::string &processName /* = "" */,
							      unsigned colour /* = 0xff009c00 */)
    : VisQueuedTwig (state, parent, name),
      m_name (name),
      m_friendlyName (friendlyName),
      m_moduleLabel (moduleLabel),
      m_instanceName (instanceName),
      m_processName (processName),
      m_binning (state, lat::CreateCallback (this, &VisEcalUncalibratedRecHitTwig::twigChanged)),
      m_scale (state, lat::CreateCallback (this, &VisEcalUncalibratedRecHitTwig::twigChanged)),
      m_cut (state, lat::CreateCallback (this, &VisEcalUncalibratedRecHitTwig::twigChanged)),
      m_escale (state, lat::CreateCallback (this, &VisEcalUncalibratedRecHitTwig::twigChanged)),
      m_annotation (state, lat::CreateCallback (this, &VisEcalUncalibratedRecHitTwig::twigChanged)),
      m_text ("no info"),
      m_rgba (colour)
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID recHitCollID (typeid (EcalUncalibratedRecHitCollection));
    tfService->registerTwig (recHitCollID.friendlyClassName (), &createThisTwig);
}

void
VisEcalUncalibratedRecHitTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisEcalUncalibratedRecHitTwig::onNewEvent (const edm::Event &event,
					   const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_recHits.clear ();
    
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    // Get uncalibrated ecal barrel rechits
    std::vector<edm::Handle<EcalUncalibratedRecHitCollection> > rechits;
    try
    {	
	QApplication::setOverrideCursor (Qt::waitCursor);
	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (state ()))
	    sbar->setMessage ("Retrieving an EcalUncalibratedRecHitCollection...");

	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, rechits);
	}

	if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (state ()))
	    sbar->setMessage ("EcalUncalibratedRecHitCollection done.");
	QApplication::restoreOverrideCursor ();
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisEcalUncalibratedRecHitTwig::onNewEvent ");
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
    
    if (! rechits.empty ())
    {
	for (std::vector<edm::Handle<EcalUncalibratedRecHitCollection> >::iterator i = rechits.begin (), iEnd = rechits.end (); i != iEnd; ++i) 
	{
	    const EcalUncalibratedRecHitCollection& c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    this->name (nameStr.toStdString ());
	    for (std::vector<EcalUncalibratedRecHit>::const_iterator ci = c.begin (), ciEnd = c.end (); ci != ciEnd; ++ci)
	    {
		m_recHits.push_back (*ci); 
	    }
	}
    }
        
    VisQueuedTwig::onBaseInvalidate ();
}

void
VisEcalUncalibratedRecHitTwig::update (IgTextRep *rep)
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
	     << "<TH>Eta</TH>"
	     << "<TH>Phi</TH>"
	     << "<TH>Amplitude</TH>"
	     << "<TH>Pedestal</TH>"
	     << "<TH>Jitter</TH>"
	     << "<TH>Chi^2</TH>"
	     << "</TR>";
	text << setiosflags (std::ios::showpoint | std::ios::fixed);
	text.setf (std::ios::right, std::ios::adjustfield);
    
	int nDigis = 0;
	if (! m_recHits.empty ()) 
	{
	    VisEventSetupService *esService = VisEventSetupService::get (state ());
	    ASSERT (esService);

	    try 
	    {
		for (std::vector<EcalUncalibratedRecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
		{
		    float amp = (*i).amplitude ();
		    float energy = amp * 12 / 1000; // Rough estimation
		    if (energy > m_cut.value ())
		    {
			EcalUncalibratedRecHit hit = (*i);
			const GlobalPoint& pos = esService->getCellPosition ((*i).id ());		
			float eta = pos.eta ();
			float phi = pos.phi ();

			text << "<TR align = right>"
			     << "<TD>" << std::setw (3) << nDigis++ << "</TD>"
			     << "<TD>" << std::setw (5) << std::setprecision (3) << eta << "</TD>"
			     << "<TD>" << std::setw (5) << std::setprecision (3) << phi << "</TD>"
			     << "<TD>" << std::setw (5) << std::setprecision (3) << amp << "</TD>"
			     << "<TD>" << std::setw (5) << std::setprecision (3) << hit.pedestal () << "</TD>"
			     << "<TD>" << std::setw (5) << std::setprecision (3) << hit.jitter () << "</TD>"
			     << "<TD>" << std::setw (5) << std::setprecision (3) << hit.chi2 () << "</TD>"
			     << "</TR>";
		    }
		}
	    }
	    catch (cms::Exception& e)
	    {
		e.append (" from VisEcalUncalibratedRecHitTwig::update(IgTextRep*) ");
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
    
	text << "</table>";
    }
    
    // Send it over.
    rep->setText (text.str ());
}
 
void
VisEcalUncalibratedRecHitTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
        
    if (! m_recHits.empty ())
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	SoSeparator *sep = new SoSeparator;
    
	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (m_rgba, rgbcomponents);
	// mat->diffuseColor.setValue (SbColor (rgbcomponents));
	mat->diffuseColor.setValue (1.0, 0.0, 153.0 / 255.0);
	sep->addChild (mat);

	try
	{
	    for (std::vector<EcalUncalibratedRecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
	    {
		float amp = (*i).amplitude ();
		float energy = amp * 12 / 1000; // Rough estimation
		if (energy > m_cut.value ()) 
		{
		    EcalUncalibratedRecHit hit = (*i);

		    if (! hit.id ().null ()) 
		    {
			const CaloCellGeometry::CornersVec& corners = esService->getCorners ((*i).id ());
			ASSERT (corners.size () == 8);
		    
			IgSoCrystalHit *crystalHit = new IgSoCrystalHit;
			crystalHit->energy.setValue (energy);	    
			crystalHit->scale.setValue (m_escale.value ());
			crystalHit->relativeWidth.setValue (1.0);
			crystalHit->drawCrystal.setValue (false);
			crystalHit->drawHit.setValue (true);
		    
			crystalHit->front1.setValue (corners [3].x () / 100.0, corners [3].y () / 100.0, corners [3].z () / 100.0);
			crystalHit->front2.setValue (corners [2].x () / 100.0, corners [2].y () / 100.0, corners [2].z () / 100.0);
			crystalHit->front3.setValue (corners [1].x () / 100.0, corners [1].y () / 100.0, corners [1].z () / 100.0);
			crystalHit->front4.setValue (corners [0].x () / 100.0, corners [0].y () / 100.0, corners [0].z () / 100.0);
		    
			crystalHit->back1.setValue  (corners [7].x () / 100.0, corners [7].y () / 100.0, corners [7].z () / 100.0);
			crystalHit->back2.setValue  (corners [6].x () / 100.0, corners [6].y () / 100.0, corners [6].z () / 100.0);
			crystalHit->back3.setValue  (corners [5].x () / 100.0, corners [5].y () / 100.0, corners [5].z () / 100.0);
			crystalHit->back4.setValue  (corners [4].x () / 100.0, corners [4].y () / 100.0, corners [4].z () / 100.0);
		    
			sep->addChild (crystalHit);
		    }
		}
	    }
        }
	catch (cms::Exception& e)
	{
	    e.append (" from VisEcalUncalibratedRecHitTwig::update(Ig3DRep*) ");
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
}
 
void
VisEcalUncalibratedRecHitTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;    

    try 
    {
	if (! m_recHits.empty ()) 
	{
	    VisEventSetupService *esService = VisEventSetupService::get (state ());
	    ASSERT (esService);

	    for (std::vector<EcalUncalibratedRecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
	    {
		float amp = (*i).amplitude ();
		float energy = amp * 12 / 1000; // Rough estimation
		if (fabs (energy) > m_cut.value ()) 
 		{
		    const GlobalPoint& pos = esService->getCellPosition ((*i).id ());		
		    EcalUncalibratedRecHit hit = (*i);
		    float eta = pos.eta ();
		    float phi = pos.phi ();
		    (phi < 0.0) ?  phi = 2 * M_PI + phi : phi; // correction in absence of one convention

		    IgSoTower *tower = new IgSoTower;
		    tower->position = SbVec2f (phi, eta); // eta, phi
		    tower->scaleFactor = m_scale.value ();
		    tower->etaWidth = 0.0174;
		    tower->phiWidth = 0.0174;
		    tower->energy = energy;
		    tower->emFraction = 1;
		    sep->addChild (tower);
		}
	    }
	} 
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisEcalUncalibratedRecHitTwig::update(IgLegoRep *) ");
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
VisEcalUncalibratedRecHitTwig::update (IgRPhiRep *rep)
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
    mat->ambientColor.setValue (1.0, 0.0, 153.0 / 255.0);
    mat->diffuseColor.setValue (1.0, 0.0, 153.0 / 255.0);
    mat->specularColor.setValue (1.0, 0.0, 153.0 / 255.0);
    mat->emissiveColor.setValue (1.0, 0.0, 153.0 / 255.0);
    posSep->addChild (mat);
    
    SoMaterial *negMat = new SoMaterial;
    negMat->ambientColor.setValue (51.0 / 255.0, 1.0, 102.0 / 255.0);
    negMat->diffuseColor.setValue (51.0 / 255.0, 1.0, 102.0 / 255.0);
    negMat->specularColor.setValue (51.0 / 255.0, 1.0, 102.0 / 255.0);
    negMat->emissiveColor.setValue (51.0 / 255.0, 1.0, 102.0 / 255.0);
    negSep->addChild (negMat);

    int nbrOfBins = 360;
    float radius = 1.29;

    int binNumber;

    std::vector<float> bufferPositive (nbrOfBins, 0.0);
    std::vector<float> bufferNegative (nbrOfBins, 0.0);
    try 
    {
	if (! m_recHits.empty ()) 
	{
	    VisEventSetupService *esService = VisEventSetupService::get (state ());
	    ASSERT (esService);

	    for (std::vector<EcalUncalibratedRecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
	    {
		float amp = (*i).amplitude ();
		float energy = amp * 12 / 1000; // Rough estimation
		if (energy > m_cut.value ())
		{
		    if (VisEcalSubdetector::subDetName ((EcalSubdetector)((*i).id ().subdetId ())) == "EcalBarrel")
		    {
			const GlobalPoint& pos = esService->getCellPosition ((*i).id ());		
			EcalUncalibratedRecHit hit = (*i);
			float phi = pos.phi ();
			
			(phi < 0) ? phi = 2 * M_PI + phi : phi;
		    
			binNumber = (int) floor (phi / (2 * M_PI / nbrOfBins));

			if (binNumber >= 0 && binNumber < nbrOfBins)
			{			  
			    if (m_binning.value () == "project")
			    {				
				if ((energy > 0.0) && (energy > bufferPositive [binNumber]))
				    bufferPositive [binNumber] = energy;
				else if ((energy < 0.0) && (energy < bufferNegative [binNumber]))
				    bufferNegative [binNumber] = energy;
			    }
			    else
 				(energy >= 0.0) ? bufferPositive [binNumber] += energy : bufferNegative [binNumber] += energy;
			}
		    }		    
		}
	    }
	    
	    std::vector<float> ::const_iterator first = bufferPositive.begin ();
	    std::vector<float> ::const_iterator last = bufferPositive.end ();
	    
	    if (*max_element (first, last) > m_cut.value ())
	    {
		IgSoCircularHist *recHits = new IgSoCircularHist;
		recHits->minRadius = radius;
		recHits->maxRadius = -1;
		recHits->scaleFactor = m_escale.value ();
		recHits->numberOfBins = nbrOfBins;
		recHits->energies.setValues (0, nbrOfBins, &bufferPositive [0]);
		recHits->logScale = false;
		recHits->showAnnotations = m_annotation.value ();
		recHits->layer = 5.5;
		posSep->addChild (recHits);
	    }
	    
	    std::vector<float> ::const_iterator nfirst = bufferNegative.begin ();
	    std::vector<float> ::const_iterator nlast = bufferNegative.end ();
	    
	    if (fabs(*max_element (nfirst, nlast)) > m_cut.value ())
	    {
		IgSoCircularHist *negRecHits = new IgSoCircularHist;
		negRecHits->minRadius = radius;
		negRecHits->maxRadius = -1;
		negRecHits->scaleFactor = m_escale.value ();
		negRecHits->numberOfBins = nbrOfBins;
		negRecHits->energies.setValues (0, nbrOfBins, &bufferNegative [0]);
		negRecHits->logScale = false;
		negRecHits->showAnnotations = m_annotation.value ();
		negRecHits->layer = 5.0;
		negSep->addChild (negRecHits);
	    }	    
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisEcalUncalibratedRecHitTwig::update (IgRPhiRep*) ");
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
VisEcalUncalibratedRecHitTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
	
    SoSeparator *sep = new SoSeparator;    
    SoMaterial *mat = new SoMaterial;
    mat->ambientColor.setValue (1.0, 0.0, 153.0 / 255.0);
    mat->diffuseColor.setValue (1.0, 0.0, 153.0 / 255.0);
    mat->specularColor.setValue (1.0, 0.0, 153.0 / 255.0);
    mat->emissiveColor.setValue (1.0, 0.0, 153.0 / 255.0);
    sep->addChild (mat);    

    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);
	
    unsigned binNumber = 0;
    const unsigned nBinsZ = 85*2;
    const unsigned nBinsR = 39*2;
    const unsigned nbrOfBins = 2 * (nBinsZ + nBinsR);
    const float radiusR = 1.29f;
    const float radiusZ = 3.17f;
    const float barrelMaxEta = 1.479f; // max eta of ECAL Barrel
    const float beamPipeTheta = 5.7f * M_PI / 180.f;
    const float endcapMaxTheta = 25.464f * M_PI / 180.f;
	
    std::vector<float> energies (nbrOfBins * 2, 0.0); // times 2 for the other energy (which is not used)
    try 
    {
	if (! m_recHits.empty ()) 
	{
	    for (std::vector<EcalUncalibratedRecHit>::const_iterator i = m_recHits.begin (), iEnd = m_recHits.end (); i != iEnd; ++i) 
	    {
		float amp = (*i).amplitude ();
		float energy = amp * 12 / 1000; // Rough estimation: count around 12/13 MeV per ADC count
		if (energy > m_cut.value ())
		{
		    EcalUncalibratedRecHit hit = (*i);		
		    binNumber = esService->getBinNumber ((*i).id (), nBinsR, nBinsZ, radiusR, radiusZ, barrelMaxEta, beamPipeTheta, endcapMaxTheta);

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
		IgSoRectColHist* hist = new IgSoRectColHist;
		hist->radiusR = radiusR;
		hist->radiusZ = radiusZ;
		hist->numR = nBinsR;
		hist->numZ = nBinsZ;
		hist->energies.setValues (0, energies.size(), &energies [0]);
		hist->logScale = false;
		hist->layer = -5.0;
		hist->scaleFactor = m_escale.value ();
		hist->barrelMaxEta = barrelMaxEta;
		hist->beamPipeTheta = beamPipeTheta;
		hist->endcapMaxTheta = endcapMaxTheta;
				
		std::vector<SbColor> colors (4);
		colors[0] = SbColor (1.0, 0.0, 153.0 / 255.0);
		colors[1] = SbColor (51.0 / 255.0, 1.0, 102.0 / 255.0);
		colors[2] = SbColor (1.0, 1.0, 153.0 / 255.0);
		colors[3] = SbColor (51.0 / 255.0, 0.0, 102.0 / 255.0);
		
		hist->faceColors.setValues (0, colors.size (), &colors[0]);
		sep->addChild (hist);
	    }
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisEcalUncalibratedRecHitTwig::update(IgRZRep*) ");
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
