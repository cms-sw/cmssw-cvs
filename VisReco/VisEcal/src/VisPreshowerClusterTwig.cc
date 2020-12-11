//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisEcal/interface/VisPreshowerClusterTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerClusterFwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoRectColHist.h"
#include "Iguana/Inventor/interface/IgSoTower.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <QApplication>
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
	eventTwig = rootTwig->lookup ("/Objects/CMS Event and Detector/ECAL Event/Preshower Clusters");
	
	if (! eventTwig)
	    eventTwig = parent;

	return new VisPreshowerClusterTwig (state, eventTwig, "[N/A] Preshower Clusters (" + name + ")", 
					    friendlyName, modLabel, instanceName, processName);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisPreshowerClusterTwig::VisPreshowerClusterTwig (IgState *state, IgTwig *parent,
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
      m_binning (state, lat::CreateCallback (this, &VisPreshowerClusterTwig::twigChanged)),
      m_scale (state, lat::CreateCallback (this, &VisPreshowerClusterTwig::twigChanged)),
      m_cut (state, lat::CreateCallback (this, &VisPreshowerClusterTwig::twigChanged)),
      m_escale (state, lat::CreateCallback (this, &VisPreshowerClusterTwig::twigChanged)),
      m_rMax (state, lat::CreateCallback (this, &VisPreshowerClusterTwig::twigChanged)),
      m_rgba (colour)
{
    VisTwigFactroyService *tfService = VisTwigFactroyService::get (state);
    if (! tfService)
    {
	tfService = new VisTwigFactroyService (state);
    }
    edm::TypeID recHitCollID (typeid (reco::PreshowerClusterCollection));
    tfService->registerTwig (recHitCollID.friendlyClassName (), &createThisTwig);
}

void
VisPreshowerClusterTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisPreshowerClusterTwig::onNewEvent (const edm::Event &event,
			       const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    m_clusters.clear ();
    
    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<reco::PreshowerClusterCollection> > rechits;
    try
    {
	QApplication::setOverrideCursor (Qt::waitCursor);
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, rechits);
	}
	QApplication::restoreOverrideCursor ();
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisPreshowerClusterTwig::onNewEvent ");
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
	for (std::vector<edm::Handle<reco::PreshowerClusterCollection> >::iterator i = rechits.begin (), iEnd = rechits.end (); i != iEnd; ++i) 
	{
	    const reco::PreshowerClusterCollection& c = *(*i);
	    QString sizeStr = (QString ("%1").arg (c.size ()));
	    QString nameStr = QString::fromStdString (this->name ());
	    int ib = nameStr.find ("[");
	    int ie = nameStr.find ("]");
	    nameStr.replace (ib + 1, ie - 1, sizeStr);
	    this->name (nameStr.toStdString ());
	    for (std::vector<reco::PreshowerCluster>::const_iterator ci = c.begin (), ciEnd = c.end (); ci != ciEnd; ++ci)
	    {
		m_clusters.push_back (*ci);
	    }
	}
    }
    
    IgRepSet::invalidate (this->parent (), IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);   
}

void
VisPreshowerClusterTwig::update (IgTextRep *rep)
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
	     << "<TH>Number of RecHits</TH>"
	     << "<TH>Preshower Plane</TH>"
	     << "<TH>Et</TH>"
	     << "</TR>";
	text << setiosflags (std::ios::showpoint | std::ios::fixed);
	text.setf (std::ios::right, std::ios::adjustfield);
    
	if (! m_clusters.empty ()) 
	{
	    VisEventSetupService *esService = VisEventSetupService::get (state ());
	    ASSERT (esService);

	    try 
	    {
		for (std::vector<reco::PreshowerCluster>::const_iterator i = m_clusters.begin (), iEnd = m_clusters.end (); i != iEnd; ++i) 
		{
		    float energy = (*i).et ();
		    if (energy > m_cut.value ())
		    {
			text << "<TR align = right>"
			     << "<TD>" << std::setw (3) << (*i).nhits() << "</TD>"
			     << "<TD>" << std::setw (5) << std::setprecision (3) << (*i).plane () << "</TD>"
			     << "<TD>" << std::setw (5) << std::setprecision (3) << energy << "</TD>"
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
	}
    
	text << "</table>";
    }
    
    // Send it over.
    rep->setText (text.str ());
}
 
void
VisPreshowerClusterTwig::update (Ig3DRep *rep)
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
    SoDrawStyle *drawStyle = new SoDrawStyle;
    drawStyle->pointSize = 6;
    sep->addChild (drawStyle);
    
    if (! m_clusters.empty ())
    {
	try 
	{
	    SoVertexProperty *vertices = new SoVertexProperty;
	    int nVrtx = 0;
	    for (std::vector<reco::PreshowerCluster>::const_iterator i = m_clusters.begin (), iEnd = m_clusters.end (); i != iEnd; ++i) 
	    {
		float energy = (*i).et ();
		if (energy > m_cut.value ()) 
		{
		    vertices->vertex.set1Value (nVrtx++, SbVec3f ((*i).x(), (*i).y(), (*i).z()));
		}
	    }
	    vertices->vertex.setNum (nVrtx);
	    SoPointSet *points = new SoPointSet;
	    points->vertexProperty.setValue (vertices);
	    points->numPoints.setValue (nVrtx);
	    sep->addChild (points);
        }
	catch (cms::Exception& e)
	{
	    e.append (" from VisPreshowerClusterTwig::update(Ig3DRep*) ");
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
VisPreshowerClusterTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    rep->clear ();
    
    SoSeparator *sep = new SoSeparator;    

    try 
    {
	if (! m_clusters.empty ()) 
	{
	    for (std::vector<reco::PreshowerCluster>::const_iterator i = m_clusters.begin (), iEnd = m_clusters.end (); i != iEnd; ++i) 
	    {
		float energy = (*i).et ();
		if (fabs (energy) > m_cut.value ()) 
 		{
		    float eta = (*i).eta ();
		    float phi = (*i).phi ();
		    (phi < 0.0) ? phi = 2 * M_PI + phi : phi; // correction in absence of one convention

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
	e.append (" from VisPreshowerClusterTwig::update(IgLegoRep *) ");
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
VisPreshowerClusterTwig::update (IgRZRep *rep)
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
	if (! m_clusters.empty ()) 
	{
	    for (std::vector<reco::PreshowerCluster>::const_iterator i = m_clusters.begin (), iEnd = m_clusters.end (); i != iEnd; ++i) 
	    {
		float energy = (*i).energy ();
		if (energy > m_cut.value ())
		{
		    binNumber = esService->getBinNumber ((*i).eta (), (*i).phi (), nBinsR, nBinsZ, radiusR, radiusZ, barrelMaxEta, beamPipeTheta, endcapMaxTheta);
		    
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
		if (m_rMax.value () > 0) hist->maxDist = m_rMax.value ();		
		hist->numR = nBinsR;
		hist->numZ = nBinsZ;
		hist->energies.setValues (0, energies.size(), &energies [0]);
		hist->logScale = false;
		hist->layer = -6.0;
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
    }
    catch (cms::Exception& e)
    {
	e.append (" from VisPreshowerClusterTwig::update(IgRZRep*) ");
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
