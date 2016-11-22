//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT

#include "VisReco/VisRecoJets/interface/VisCaloTowerTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventProcessorService.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/TypeID.h"
#include "Iguana/Inventor/interface/IgSoTower.h"
#include "Iguana/Inventor/interface/IgSoCircularHist.h"
#include "Iguana/Inventor/interface/IgSoCalHit.h"
#include "Iguana/Inventor/interface/IgSoRectColHist.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSelection.h>
#include <qstring.h>
#include <qwhatsthis.h>
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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/CaloTowers");
	
	if (! eventTwig)
	    eventTwig = parent;

	return new VisCaloTowerTwig (state, eventTwig, "[N/A] CaloTower (" + name + ")", 
				     friendlyName, modLabel, instanceName, processName);
    }

    void
    selectTower (void *userData, SoPath *pickPath) 
    {
	if (pickPath->getTail ()->isOfType (IgSoTower::getClassTypeId ()))
	{
	    SoNode *node;
	    node  = pickPath->getNode (pickPath->getLength () - 1);
	    IgSoTower *pickedTower = dynamic_cast<IgSoTower *> (node);
	    // FIXME: Calculate point!
	    QWhatsThis::showText (QPoint (), QString ("Et = %1 GeV, Em %2 %")
				  .arg (pickedTower->energy.getValue (), 0, 'f', 2)
				  .arg (pickedTower->emFraction.getValue () * 100, 0, 'f', 2));    
	}
    }
}

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisCaloTowerTwig::VisCaloTowerTwig (IgState *state, IgTwig *parent,
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
      m_processName (processName),
      m_binning (state, lat::CreateCallback (this, &VisCaloTowerTwig::twigChanged)),
      m_annotation (state, lat::CreateCallback (this, &VisCaloTowerTwig::twigChanged)),
      m_scale (state, lat::CreateCallback (this, &VisCaloTowerTwig::twigChanged)),
      m_cut (state, lat::CreateCallback (this, &VisCaloTowerTwig::twigChanged)),
      m_escale (state, lat::CreateCallback (this, &VisCaloTowerTwig::twigChanged)),
      m_phiStart (state, lat::CreateCallback (this, &VisCaloTowerTwig::twigChanged)),
      m_phiDelta (state, lat::CreateCallback (this, &VisCaloTowerTwig::twigChanged))
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID caloTowersID (typeid (CaloTowerCollection));
    tfService->registerTwig (caloTowersID.friendlyClassName (), &createThisTwig);
}

void
VisCaloTowerTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisCaloTowerTwig::onNewEvent (const edm::Event &event,
			      const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_text = QString ("Run # %1, event # %2")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ()).latin1 ();

    std::vector<edm::Handle<CaloTowerCollection> > towers;
    try 
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, towers);
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisCaloTowerTwig::onNewEvent ");
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
    
    m_seedTowers.clear ();

    if (! towers.empty ())
    {
	std::vector<edm::Handle<CaloTowerCollection> >::iterator i;
	for (i = towers.begin (); i != towers.end (); i++) 
	{
	    const CaloTowerCollection& c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    
	    this->name (nameStr.toStdString ());
	    
	    for (CaloTowerCollection::const_iterator j = c.begin (), end = c.end (); j != end; ++j) 
	    {
		m_seedTowers.push_back (*j);
	    }
	}
    }
    
    IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisCaloTowerTwig::update (IgTextRep *rep)
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
	     << "<TH>E<SUB>T (GeV)</TH>"
	     << "<TH>E<SUB>T em (GeV)</TH>"
	     << "<TH>E<SUB>T had (GeV)</TH>"
	     << "<TH>E<SUB>T outer (GeV)</TH>"
	     << "<TH>E (GeV)</TH>"
	     << "<TH>E<SUB>em (GeV)</TH>"
	     << "<TH>E<SUB>had (GeV)</TH>"
	     << "<TH>E<SUB>outer (GeV)</TH>"
	     << "<TH>eta</TH>"
	     << "<TH>phi [rad]</TH>"
	     << "</TR>";
	text << setiosflags (std::ios::showpoint | std::ios::fixed);
	text.setf (std::ios::right, std::ios::adjustfield);
        
	int nTowers = 0;
	for (std::vector<CaloTower>::const_iterator i = m_seedTowers.begin (); i != m_seedTowers.end (); ++i) 
	{
	    double eta = (*i).eta ();
	    double phi = (*i).phi ();
	    double et = (*i).et ();

	    if (et > m_cut.value ()) 
	    {   
		text << "<TR align = right>"
		     << "<TD>" << std::setw (3) << nTowers++ << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << et << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*i).emEt () << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*i).hadEt () << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*i).outerEt () << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*i).energy () << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*i).emEnergy () << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*i).hadEnergy () << "</TD>"
		     << "<TD>" << std::setw (2) << std::setprecision (3) << (*i).outerEnergy () << "</TD>"
		     << "<TD>" << std::setw (6) << std::setprecision (3) << eta << "</TD>"
		     << "<TD>" << std::setw (5) << std::setprecision (3) << phi << "</TD>"
		     << "</TR>";
	    }
	}
    
	text << "</table><br>";
    }

    // Send it over.
    rep->setText (text.str ());
}

void
VisCaloTowerTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);
    
    SoSelection *towerSep = new SoSelection;
    towerSep->policy = SoSelection::SINGLE;
    towerSep->addSelectionCallback (selectTower, (void*) false);
    
    // Loop over all Seeds
    for (std::vector<CaloTower>::const_iterator i = m_seedTowers.begin (), end = m_seedTowers.end (); i != end; ++i)
    {
	float eta = (*i).eta ();
	float phi = (*i).phi ();
	if (phi < 0) phi += 2 * M_PI;
	float et = (*i).et ();
	
	if (et > m_cut.value ())
	{
	    IgSoTower *tower = new IgSoTower;
	    tower->position = SbVec2f (phi, eta); // eta, phi
	    tower->scaleFactor = m_scale.value ();
	    tower->energy = et;
	    tower->emFraction = (*i).emEt () / et;
	    
	    if (fabs (eta) > 1.74)
	    {
		tower->etaWidth = 0.174f;
		tower->phiWidth = esService->phi4eta (fabs (eta));
	    }
	    
	    towerSep->addChild (tower);
	}
    }
    
    rep->node ()->addChild (towerSep);
}

/** Update object */
void
VisCaloTowerTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();

    SoSeparator *sep = new SoSeparator;
    
    VisEventSetupService *esService = VisEventSetupService::get (state ());
    ASSERT (esService);
    
    try 
    {
	for (std::vector<CaloTower>::const_iterator it = m_seedTowers.begin (), end = m_seedTowers.end (); 
	     it != end; ++it)
	{
	    float et = (*it).et ();
	    if (et > m_cut.value ()) 
	    {
		size_t size = (*it).constituentsSize ();
	    
		for (size_t i = 0; i <= size; ++i)
		{
		    DetId det = (*it).constituent (i);
		    if (! det.null ())
		    {
			if (esService->present (det))
			{
			    if (! esService->emptyCorners (det))
			    {
				const CaloCellGeometry::CornersVec& corners = esService->getCorners (det);
				ASSERT (corners.size () == 8);
				std::vector<SbVec3f> pts(8);

				pts [0] = SbVec3f (corners [0].x () / 100.0, corners [0].y () / 100.0, corners [0].z () / 100.0);
				pts [1] = SbVec3f (corners [1].x () / 100.0, corners [1].y () / 100.0, corners [1].z () / 100.0);
				pts [2] = SbVec3f (corners [2].x () / 100.0, corners [2].y () / 100.0, corners [2].z () / 100.0);
				pts [3] = SbVec3f (corners [3].x () / 100.0, corners [3].y () / 100.0, corners [3].z () / 100.0);
		    
				pts [4] = SbVec3f (corners [4].x () / 100.0, corners [4].y () / 100.0, corners [4].z () / 100.0);
				pts [5] = SbVec3f (corners [5].x () / 100.0, corners [5].y () / 100.0, corners [5].z () / 100.0);
				pts [6] = SbVec3f (corners [6].x () / 100.0, corners [6].y () / 100.0, corners [6].z () / 100.0);
				pts [7] = SbVec3f (corners [7].x () / 100.0, corners [7].y () / 100.0, corners [7].z () / 100.0);

				IgSoCalHit *hit = new IgSoCalHit;
				hit->vertices.setValues (0, 8, &pts[0]);
				hit->energy.setValue (et);
				hit->drawShape = false;
				hit->scaleFactor = m_escale.value ();
				hit->emFraction = (*it).emEt () / et;
				hit->orderedRGBA.set1Value (0, SbColor (0.5,0.5,0.5).getPackedValue ());
				hit->orderedRGBA.set1Value (1, SbColor (0.5,1.0,0.5).getPackedValue ());
				hit->orderedRGBA.set1Value (2, SbColor (0.5,0.5,1.0).getPackedValue ());
		    
				sep->addChild (hit);
			    }
			}
		    }		    
		}		
	    }
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisCaloTowerTwig::update(Ig3DRep*) ");
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
VisCaloTowerTwig::update (IgRPhiRep *rep) 
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;    
    SoSeparator *emSep = new SoSeparator;    
    SoSeparator *hadSep = new SoSeparator;
    sep->addChild (emSep);
    sep->addChild (hadSep);
    
    SoMaterial *mat = new SoMaterial;
    mat->ambientColor.setValue (0.5, 1.0, 0.5);
    mat->diffuseColor.setValue (0.5, 1.0, 0.5);
    mat->specularColor.setValue (0.5, 1.0, 0.5);
    mat->emissiveColor.setValue (0.5, 1.0, 0.5);
    emSep->addChild (mat);
    
    SoMaterial *hadMat = new SoMaterial;
    hadMat->ambientColor.setValue (0.5, 0.5, 1.0);
    hadMat->diffuseColor.setValue (0.5, 0.5, 1.0);
    hadMat->specularColor.setValue (0.5, 0.5, 1.0);
    hadMat->emissiveColor.setValue (0.5, 0.5, 1.0);
    hadSep->addChild (hadMat);

    int nbrOfBins = 72;
    int binNumber;

    std::vector<float> bufferEm (nbrOfBins);
    std::vector<float> bufferHad (nbrOfBins);
    try 
    {
	if (! m_seedTowers.empty ())
	{
	    for (std::vector<CaloTower>::const_iterator it = m_seedTowers.begin (), end = m_seedTowers.end (); 
		 it != end; ++it)
	    {
		float et = (*it).et ();
		if (et > m_cut.value ()) 
		{
		    float eta = (*it).eta ();
		    float em = (*it).emEt ();
		    if (fabs (eta) < 1.444f)
		    {
			int iphi = (*it).id ().iphi ();
			binNumber = iphi - 1;

			if (binNumber >= 0 && binNumber < 72)
			{
			    if (m_binning.value () == "project")
			    {				
				if (et > bufferHad [binNumber])
				    bufferHad [binNumber] = et;
				if (em > bufferEm [binNumber])
				    bufferEm [binNumber] = em;
			    }
			    else
			    {
				bufferHad [binNumber] += et;
				bufferEm [binNumber] += em;
			    }
			}
		    }		    
		}		
	    }
	    if (*max_element (bufferHad.begin (), bufferHad.end ()) > 0.0)
	    {
		IgSoCircularHist *tower = new IgSoCircularHist;
		tower->minRadius = 1.9;
		tower->maxRadius = -1;
		tower->scaleFactor = m_escale.value ();
		tower->numberOfBins = nbrOfBins;
		tower->energies.setValues (0, nbrOfBins, &bufferHad [0]);
		tower->logScale = false;
		tower->showAnnotations = m_annotation.value ();
		tower->layer = 5.6;
		hadSep->addChild (tower);
	    }
	    if (*max_element (bufferEm.begin (), bufferEm.end ()) > 0.0)
	    {
		IgSoCircularHist *emtower = new IgSoCircularHist;
		emtower->minRadius = 1.9;
		emtower->maxRadius = -1;
		emtower->scaleFactor = m_escale.value ();
		emtower->numberOfBins = nbrOfBins;
		emtower->energies.setValues (0, nbrOfBins, &bufferEm [0]);
		emtower->logScale = false;
		emtower->showAnnotations = m_annotation.value ();
		emtower->layer = 5.8;
		emSep->addChild (emtower);
	    }
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisCaloTowerTwig::update(IgRPhiRep*) ");
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
VisCaloTowerTwig::update (IgRZRep *rep) 
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
	
    SoSeparator *sep = new SoSeparator;    
    SoSeparator *hsep = new SoSeparator;    
    SoSeparator *emsep = new SoSeparator;    
    sep->addChild (hsep);
    sep->addChild (emsep);

    SoMaterial *hmat = new SoMaterial;
    hmat->ambientColor.setValue (SbColor (0.5,0.5,1.0));
    hmat->diffuseColor.setValue (SbColor (0.5,0.5,1.0));
    hmat->specularColor.setValue (SbColor (0.5,0.5,1.0));
    hmat->emissiveColor.setValue (SbColor (0.5,0.5,1.0));
    hsep->addChild (hmat);

    SoMaterial *mat = new SoMaterial;
    mat->ambientColor.setValue (SbColor (0.5,1.0,0.5));
    mat->diffuseColor.setValue (SbColor (0.5,1.0,0.5));
    mat->specularColor.setValue (SbColor (0.5,1.0,0.5));
    mat->emissiveColor.setValue (SbColor (0.5,1.0,0.5));
    emsep->addChild (mat);

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
    std::vector<float> energies (nbrOfBins * 2);  // times 2 for the other energy (which is not used)
    std::vector<float> emEnergies (nbrOfBins * 2);  // times 2 for the other energy (which is not used)

    try 
    {
	if (! m_seedTowers.empty ())
	{
	    for (std::vector<CaloTower>::const_iterator it = m_seedTowers.begin (), end = m_seedTowers.end (); 
		 it != end; ++it)
	    {
		float et = (*it).et ();
		if (et > m_cut.value ())
		{
		    float em = (*it).emEt ();
		    binNumber = esService->getBinNumber ((*it).eta (), (*it).phi (), nBinsR, nBinsZ, radiusR, radiusZ, barrelMaxEta, beamPipeTheta);

		    if (binNumber < nbrOfBins)
		    {
			if (m_binning.value () == "project")
			{			    
			    (energies[binNumber] > et) ? energies[binNumber] : energies[binNumber] = et;
			    (emEnergies[binNumber]  > em) ? emEnergies[binNumber] : emEnergies[binNumber] = em;
			}			
			else
			{			    
			    energies[binNumber] += et;
			    emEnergies[binNumber] += em;			    
			}			
		    }
		}
	    }
	    if (*max_element (energies.begin (), energies.end ()) > 0.0)
	    {
		IgSoRectColHist* hist = new IgSoRectColHist;
		hist->radiusR = radiusR;
		hist->radiusZ = radiusZ;
		hist->numR = nBinsR;
		hist->numZ = nBinsZ;
		hist->energies.setValues (0, energies.size(), &energies [0]);
		hist->logScale = false;
		hist->layer = -4.0;
		hist->scaleFactor = m_escale.value ();
		hist->barrelMaxEta = barrelMaxEta;
		hist->beamPipeTheta = beamPipeTheta;

		std::vector<SbColor> colors (4);
		colors[0] = SbColor (0.5,0.5,1.0); // first energy's positive color 
		colors[1] = SbColor (0.5,0.5,1.0); // first energy's negative color
		colors[2] = SbColor (0.5,0.5,1.0); // second energy's positive color (not used)
		colors[3] = SbColor (0.5,0.5,1.0); // second energy's negative color (not used)
				
		hist->faceColors.setValues (0, colors.size (), &colors[0]);
		hsep->addChild (hist);
	    }	    
	    if (*max_element (emEnergies.begin (), emEnergies.end ()) > 0.0)
	    {
		IgSoRectColHist* emhist = new IgSoRectColHist;
		emhist->radiusR = radiusR;
		emhist->radiusZ = radiusZ;
		emhist->numR = nBinsR;
		emhist->numZ = nBinsZ;
		emhist->energies.setValues (0, emEnergies.size(), &emEnergies [0]);
		emhist->logScale = false;
		emhist->layer = -5.0;
		emhist->scaleFactor = m_escale.value ();
		emhist->barrelMaxEta = barrelMaxEta;
		emhist->beamPipeTheta = beamPipeTheta;

		std::vector<SbColor> colors (4);
		colors[0] = SbColor (0.5,1.0,0.5); // first energy's positive color 
		colors[1] = SbColor (0.5,1.0,0.5); // first energy's negative color
		colors[2] = SbColor (0.5,1.0,0.5); // second energy's positive color (not used)
		colors[3] = SbColor (0.5,1.0,0.5); // second energy's negative color (not used)
				
		emhist->faceColors.setValues (0, colors.size (), &colors[0]);
		emsep->addChild (emhist);
	    }	    
	}
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisCaloTowerTwig::update(IgRZRep*) ");
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

