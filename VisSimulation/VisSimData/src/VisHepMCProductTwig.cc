//<<<<<< INCLUDES                                                       >>>>>>

#include "VisSimulation/VisSimData/interface/VisHepMCProductTwig.h"
#include "VisSimulation/VisSimData/interface/VisGenParticleTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "Iguana/Studio/interface/IgQtLock.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DBaseModel.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <qstring.h>
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
	return new VisHepMCProductTwig (state, parent, name, friendlyName, modLabel, instanceName, processName);
    }
}

namespace 
{
    std::string itoa (int i)
    {
	char temp[20];
        sprintf (temp, "%d", i);
	return ((std::string) temp);
    }
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisHepMCProductTwig::VisHepMCProductTwig (IgState *state, IgTwig *parent, const std::string &name /* = ""*/,
					  const std::string &friendlyName /* = ""*/,
					  const std::string &moduleLabel /* = ""*/,
					  const std::string &instanceName /* = ""*/,
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
    edm::TypeID hepMCID (typeid (edm::HepMCProduct));
    tfService->registerTwig (hepMCID.friendlyClassName (), &createThisTwig);
}

void
VisHepMCProductTwig::onNewEvent (const edm::Event &event,
				 const edm::EventSetup &eventSetup)
{
    // Get debugging dump.
    VisQueuedTwig::onNewEvent (event, eventSetup);

    IgQtLock ();

    if (children ()) destroy ();

    m_text = (QString ("Run # %1, event # %2")
	      .arg (event.id ().run ())
	      .arg (event.id ().event ()).latin1 ());

    std::vector<edm::Handle<edm::HepMCProduct> > evtHandles;
    
    try
    {
	if ((! m_friendlyName.empty ()) || (! m_moduleLabel.empty ()) || (! m_instanceName.empty ()) || (! m_processName.empty ()))
	{
	    VisEventSelector visSel (m_friendlyName, m_moduleLabel, m_instanceName, m_processName);
	    event.getMany (visSel, evtHandles);
	}
	else
	{
	    event.getManyByType (evtHandles);
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

    if (! evtHandles.empty ())
    {
	edm::ESHandle < ParticleDataTable > particle_data_table;  // my
	eventSetup.getData( particle_data_table );                // my
	
	std::vector<edm::Handle<edm::HepMCProduct> >::iterator i;
	for (i = evtHandles.begin (); i != evtHandles.end (); ++i) 
	{
	    if ((i)->isValid ())
	    {
		const HepMC::GenEvent *evt = (*i)->GetEvent ();

		for (HepMC::GenEvent::particle_const_iterator p = evt->particles_begin ();
		     p != evt->particles_end (); ++p) 
		{
		    m_pMap [(*p)->barcode ()] = (*p);
		    
		    std::vector<HepMC::GenParticle *> parents;
		    if ((*p)->production_vertex ()) 
		    {
			for (HepMC::GenVertex::particle_iterator mother 
				 = (*p)->production_vertex ()->
				 particles_begin (HepMC::parents);
			     mother != (*p)->production_vertex ()->
				 particles_end (HepMC::parents); 
			     ++mother) 
			{
			    std::cout << "\t";
			    (*mother)->print ();
			    parents.push_back (*mother);
			}
		    } // = (*p)->listParents ();
		    if (parents.empty ())
		    {
			const HepPDT::ParticleData*  pd =               
			    particle_data_table->particle((*p)->pdg_id ());  // my
			std::string name = pd->name();                     // my
			VisGenParticleTwig *ptwig = 
			    new VisGenParticleTwig (this, name, *(*p));  // my
		        //VisGenParticleTwig *ptwig = new VisGenParticleTwig (this, itoa ((*p)->pdg_id ()), *(*p));
			m_twigMap [(*p)->barcode ()] = ptwig;
		    }
		    
		    std::vector<HepMC::GenParticle *> children;
		    if ((*p)->end_vertex ())
		    {
			for (HepMC::GenVertex::particle_iterator child 
				 = (*p)->end_vertex ()->
				 particles_begin (HepMC::children);
			     child != (*p)->end_vertex ()->
				 particles_end (HepMC::children); 
			     ++child) 
			{
			    std::cout << "\t";
			    (*child)->print ();
			    parents.push_back (*child);
			}
		    }

		    if (! children.empty ())
		    {
			for (std::vector<HepMC::GenParticle *>::const_iterator ic = children.begin (), end = children.end (); ic != end; ++ic)
			{
			    const HepPDT::ParticleData*  pd =
				particle_data_table->particle((*ic)->pdg_id ());  // my
			    std::string name = pd->name();                      // my
			    VisGenParticleTwig *ptwig = new VisGenParticleTwig  // my
							(m_twigMap [(*p)->barcode ()], name, *(*ic));

			    // VisGenParticleTwig *ptwig = new VisGenParticleTwig (m_twigMap [(*p)->barcode ()], itoa ((*ic)->pdg_id ()), *(*ic));
			    m_twigMap [(*ic)->barcode ()] = ptwig;
			}
		    }
		}
	    }	    
	}
    }
    
    IgRepSet::invalidate (this->parent (), IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
}

void
VisHepMCProductTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
    IgQtLock ();

    // Prepare property description.
    std::ostringstream  text;

    text << "<table width='100%' border=1>"
	 << "<TR align = center>"
	 << "<TH>Number</TH>"
	 << "<TH>Bar code</TH>"
	 << "<TH>Name</TH>"
	 << "</TR>";
    text << setiosflags (std::ios::showpoint | std::ios::fixed);
    text.setf (std::ios::right, std::ios::adjustfield);

    int nPart = 0;
    
    for (std::map<int, HepMC::GenParticle *>::iterator it = m_pMap.begin ();
	 it != m_pMap.end (); it++)
    {
	std::string name = itoa ((*it).second->pdg_id ());

	text << "<TR align = right>"
	     << "<TD>" << std::setw (3) << nPart++ << "</TD>"
	     << "<TD>" << (*it).first << "</TD>"
	     << "<TD>" << name << "</TD>";
    }

    text << "</table>";

    // Send it over.
    rep->setText (text.str ());
}

void
VisHepMCProductTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);      
    IgQtLock ();
    rep->clear ();
}

void
VisHepMCProductTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

void
VisHepMCProductTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}

void
VisHepMCProductTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    VisQueuedTwig::update (rep);
}
