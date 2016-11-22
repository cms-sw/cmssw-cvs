//<<<<<< INCLUDES                                                       >>>>>>

#include "VisSimulation/VisSimData/interface/VisGenParticleTwig.h"
#include "VisSimulation/VisSimData/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/Models/interface/xtypeinfo.h"
#include <HepMC/GenParticle.h>
#include <sstream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisGenParticleTwig::VisGenParticleTwig (IgTwig *parent,
					const std::string &name, HepMC::GenParticle &genParticle)
    : IgSimpleTwig (parent, name),
      m_genParticle (genParticle)
{
}

int
VisGenParticleTwig::id (void)
{ return m_genParticle.pdg_id (); }

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisGenParticleTwig *twig, IgTextRep *rep, unsigned))
{
    // Prepare property description.
    std::ostringstream  text;
     
    text << "Particle " << twig->name () << " with PDGid " << twig->id () << "<br>";
    rep->setText (text.str ());
}
