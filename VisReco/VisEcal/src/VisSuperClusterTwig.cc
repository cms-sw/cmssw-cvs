//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisEcal/interface/VisSuperClusterTwig.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
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
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <qapplication.h>
#include <qstring.h>
#include <classlib/utils/DebugAids.h>
#include <sstream>
#include <iomanip>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisSuperClusterTwig::VisSuperClusterTwig (IgState *state, IgTwig *parent,
					  const std::string &name,
					  std::vector <EcalRecHit> &recHits)
    : VisQueuedTwig (state, parent, name),
      m_name (name),
      m_binning (state, lat::CreateCallback (this, &VisSuperClusterTwig::twigChanged)),
      m_scale (state, lat::CreateCallback (this, &VisSuperClusterTwig::twigChanged)),
      m_cut (state, lat::CreateCallback (this, &VisSuperClusterTwig::twigChanged)),
      m_escale (state, lat::CreateCallback (this, &VisSuperClusterTwig::twigChanged)),
      m_annotation (state, lat::CreateCallback (this, &VisSuperClusterTwig::twigChanged)),
      m_recHits (recHits)
{}

void
VisSuperClusterTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisSuperClusterTwig::onNewEvent (const edm::Event &event,
				 const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);
    VisQueuedTwig::onBaseInvalidate ();
}

void
VisSuperClusterTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    
    // Prepare property description.
    std::ostringstream  text;
    text << this->name () << " from ";
    
    text << "Displayed only above  " << m_cut.value () << " GeV:<br>";
    
    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Energy</TH>"
	 << "<TH>Time</TH>"
	 << "<TH>Eta</TH>"
	 << "<TH>Phi</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);
    
    int n = 0;
    try 
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);

	for (std::vector<EcalRecHit>::const_iterator it = m_recHits.begin (), itEnd = m_recHits.end (); it != itEnd; ++it) 
	{
	    const GlobalPoint& pos = esService->getCellPosition ((*it).detid ());
	    float eta = pos.eta ();
	    float phi = pos.phi ();

	    text << "<TR align = right>"
		 << "<TD>" << std::setw (3) << n++ << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << (*it).energy () << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << (*it).time () << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << eta << "</TD>"
		 << "<TD>" << std::setw (5) << std::setprecision (3) << phi << "</TD>"
		 << "</TR>";
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisSuperClusterTwig::update(IgTextRep*) ");
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
    
    // Send it over.
    rep->setText (text.str ());
}
 
void
VisSuperClusterTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;
    
    SoMaterial *mat = new SoMaterial;
    float rgbcomponents [4];
    IgSbColorMap::unpack (0xff009c00, rgbcomponents);
    mat->diffuseColor.setValue (rgbcomponents);
    sep->addChild (mat);
    
    try 
    {
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);
	
	for (std::vector<EcalRecHit>::const_iterator it = m_recHits.begin (), itEnd = m_recHits.end (); it != itEnd; ++it) 
	{
	    const CaloCellGeometry::CornersVec& corners = esService->getCorners ((*it).detid ());
	    ASSERT (corners.size () == 8);
		    
	    IgSoCrystalHit *crystalHit = new IgSoCrystalHit;
	    crystalHit->energy.setValue ((*it).energy ());		    
	    crystalHit->scale.setValue (m_escale.value ());
	    crystalHit->relativeWidth.setValue (1.0);
	    crystalHit->drawCrystal.setValue (true);
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
    catch (cms::Exception& e)
    {
	e.append (" from VisSuperClusterTwig::update(Ig3DRep*) ");
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
VisSuperClusterTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;    

    try 
    {
	std::vector<EcalRecHit>::iterator posCurrent;
	std::vector<EcalRecHit>::iterator posEnd;

	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);
		    
	for (posCurrent = m_recHits.begin (), posEnd = m_recHits.end (); posCurrent != posEnd; ++posCurrent)
	{
	    const GlobalPoint& pos = esService->getCellPosition ((*posCurrent).detid ());		
		
	    float eta = pos.eta ();
	    float phi = pos.phi ();
	    (phi < 0.0) ? phi = 2 * M_PI + phi : phi; // correction in absence of one convention
	    
	    IgSoTower *tower = new IgSoTower;
	    tower->position = SbVec2f (phi, eta); // eta, phi
	    tower->scaleFactor = m_scale.value ();
	    tower->etaWidth = 0.0174;
	    tower->phiWidth = 0.0174;
	    tower->energy = (*posCurrent).energy (); 
	    tower->emFraction = 1;
	    sep->addChild (tower);
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisSuperClusterTwig::update(IgLegoRep *) ");
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
VisSuperClusterTwig::update (IgRPhiRep *rep)
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
    int binNumber;

    std::vector<float> bufferPositive (nbrOfBins, 0.0);
    std::vector<float> bufferNegative (nbrOfBins, 0.0);
    try 
    {
	float radius = 1.29;
	    
	std::vector<EcalRecHit>::iterator posCurrent;
	std::vector<EcalRecHit>::iterator posEnd;

	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);
		    
	for (posCurrent = m_recHits.begin (), posEnd = m_recHits.end (); posCurrent != posEnd; ++posCurrent)
	{
	    const GlobalPoint& pos = esService->getCellPosition ((*posCurrent).detid ());		
	    float phi = pos.phi ();

	    (phi < 0) ? phi = 2 * M_PI + phi : phi;
	    
	    binNumber = (int) floor (phi / (2 * M_PI / nbrOfBins));
	    float energy = (*posCurrent).energy ();
	    
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

	std::vector<float>::const_iterator first = bufferPositive.begin ();
	std::vector<float>::const_iterator last = bufferPositive.end ();
	    
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
	    
	std::vector<float>::const_iterator nfirst = bufferNegative.begin ();
	std::vector<float>::const_iterator nlast = bufferNegative.end ();
	    
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
    catch (cms::Exception& e)
    {
	e.append (" from VisSuperClusterTwig::update (IgRPhiRep*) ");
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
VisSuperClusterTwig::update (IgRZRep *rep)
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

    unsigned binNumber = 0;
    const unsigned nBinsZ = 85*2; // bins along the Z axis (one side only, 85 = a quarter of bins in Z direction)
    const unsigned nBinsR = 39*2;
    const unsigned nbrOfBins = 2 * (nBinsZ + nBinsR);
    const float radiusR = 1.29f;
    const float radiusZ = 3.17f;
    const float barrelMaxEta = 1.479f;                  // max eta of ECAL Barrel
    const float beamPipeTheta = 5.7f * M_PI / 180.f;    // opening angle of the beam gap
    const float endcapMaxTheta = 25.464f * M_PI / 180.f;// max theta of ECAL endcaps
    std::vector<float> energies (nbrOfBins * 2, 0.0); // times 2 for the other energy (which is not used)
	
    try 
    {
	std::vector<EcalRecHit>::iterator posCurrent;
	std::vector<EcalRecHit>::iterator posEnd;
	
	VisEventSetupService *esService = VisEventSetupService::get (state ());
	ASSERT (esService);
				
	for (posCurrent = m_recHits.begin (), posEnd = m_recHits.end (); posCurrent != posEnd; ++posCurrent)
	{
	    float energy = (*posCurrent).energy ();
	    
	    binNumber = esService->getBinNumber ((*posCurrent).detid (), nBinsR, nBinsZ, radiusR, radiusZ, barrelMaxEta, beamPipeTheta, endcapMaxTheta);

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
	    colors[0] = SbColor (1.0, 0.0, 153.0 / 255.0); // first energy's positive color 
	    colors[1] = SbColor (1.0, 1.0, 153.0 / 255.0); // first energy's negative color
	    colors[2] = SbColor (51.0 / 255.0, 1.0, 102.0 / 255.0); // second energy's positive color (not used)
	    colors[3] = SbColor (51.0 / 255.0, 0.0, 102.0 / 255.0); // second energy's negative color (not used)
			
	    hist->faceColors.setValues (0, colors.size (), &colors[0]);
	    sep->addChild (hist);
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisSuperClusterTwig::update(IgRZRep*) ");
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
