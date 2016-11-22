//<<<<<< INCLUDES                                                       >>>>>>

#include "VisGeant4/VisG4Transients/interface/VisG4TrajectoriesTwig.h"
#include "VisGeant4/VisG4Transients/interface/VisSoG4Trajectories.h"
#include "VisGeant4/VisG4Transients/interface/xtypeinfo.h"
#include "Iguana/GLModels/interface/IgCommon3DReps.h"
#include "Iguana/GLModels/interface/Ig3DModel.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/xtypeinfo.h"
#include "Iguana/Models/interface/IgTextModel.h"
#include "Iguana/Models/interface/IgSimpleTextRep.h"
#include "Iguana/Models/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/xtypeinfo.h"
#include <G4RunManager.hh>
#include <G4Trajectory.hh>
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

// FIXME: Use G4StateManager / G4VStateDependent?

VisG4TrajectoriesTwig::VisG4TrajectoriesTwig (IgTwig *parent)
    : IgSimpleTwig (parent, "Trajectories", true, false, true)
{
    // G4StateManager::RegisterDependent (this);
}

#if 0
bool
VisG4TrajectoriesTwig::Notify (G4ApplicationState requested);
{
    G4StateManager	*sm = G4StateManager::GetStateManager ();
    G4ApplicationState	prev = sm->GetPreviousState ();

    if (prev == GeomClosed && requested == EventProc)
    {
	// Beginning of event, but there isn't one yet.  So make it
	// null.  (FIXME: Update all models now?)  (FIXME: Install
	// timer to udpate later when we get back to gui loop?)
	m_event = 0;
	// IgRepSet::invalidate (this, SELF_MASK);
    }
    else if (prev == EventProc && requested == GeomClosed)
    {
	// End of event.  Remember the current event so we can keep on
	// displaying it.  Update all models now.  (FIXME: Delay?)
	m_event = G4RunManager::GetRunManager ()->GetCurrentEvent ();
    }
    else
	// Beginning of run (Idle -> GeomClosed) or end of run
	// (GeomClosed -> Idle).  Note that ending the run kills all
	// the events created in it, so we'll lose the last (current)
	// event too.  Hence all this code is commented out, as it is
	// really no use to us -- we can't get the info after the run
	// is finished (post /run/beamOn).  Use /run/breakAtEndOfEvent
	// to get a break (FIXME: force directly).

    return true;
}
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
	       (VisG4TrajectoriesTwig *twig, Ig3DRep *rep, unsigned mask))
{
    // Let base class implementation handle the basics.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);

    VisSoG4Trajectories	*repnode = new VisSoG4Trajectories;
    repnode->on = twig->visible ();
    repnode->flush = VisSoG4Trajectories::ALL;
    rep->node ()->addChild (repnode);
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
	       (VisG4TrajectoriesTwig *, IgTextRep *rep, unsigned))
{
    // FIXME: This should be linked to some general notification
    // whenever trajectories list changes, and updated accordingly!
    std::ostringstream text;

    text << "<table width='100%'><tr><td>Name</td><td>"
	 << "Trajectories</td></tr>\n";

    if (G4RunManager *runManager = G4RunManager::GetRunManager ())
	if (const G4Event *event = runManager->GetCurrentEvent ())
	    if (G4TrajectoryContainer *trajectories
		= event->GetTrajectoryContainer ())
	    {
		for (int i = 0; i < trajectories->entries (); ++i)
		{
		    text << "<tr><td>&nbsp;&nbsp;Trajectory</td><td>#"
			 << i << "</td></tr>\n";

		    G4Trajectory *t = static_cast<G4Trajectory *>
				      ((*trajectories) [i]);

		    for (int j = 0; j < t->GetPointEntries (); ++j)
			text << "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;Point #"
			     << j << "</td><td>"
			     << (static_cast<G4TrajectoryPoint *>
				 (t->GetPoint (j))->GetPosition ())
			     << "</td></tr>\n";
		}
	    }
	    else
		text << "<tr><td>(No trajectories)</td><td>&nbsp;</td></tr>";
	else
	    text << "<tr><td>(No event)</td><td>&nbsp;</td></tr>";
    else
	text << "<tr><td>(No run manager)</td><td>&nbsp;</td></tr>";

    text << "</table>" << '\0';

    rep->setText (text.str ());
}
