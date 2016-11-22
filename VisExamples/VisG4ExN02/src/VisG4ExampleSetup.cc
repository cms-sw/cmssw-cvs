//<<<<<< INCLUDES                                                       >>>>>>

#include "VisExamples/VisG4ExN02/src/VisG4ExampleSetup.h"
#include "VisExamples/VisG4ExN02/src/VisG4ExampleDetector.h"
#include "VisExamples/VisG4ExN02/src/VisG4ExamplePhysics.h"
#include "VisExamples/VisG4ExN02/src/VisG4ExampleGenerator.h"
#include "VisExamples/VisG4ExN02/src/VisG4ExampleRunAction.h"
#include "VisExamples/VisG4ExN02/src/VisG4ExampleEventAction.h"
#include "VisGeant4/VisG4Core/interface/VisG4UIManager.h"
#include "VisGeant4/VisG4Core/interface/VisG4VisManager.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4GeometryTwig.h"
#include "VisGeant4/VisG4Volumes/interface/VisG4TwigOps.h"
#include "VisGeant4/VisG4Transients/interface/VisG4TrajectoriesTwig.h"
#include "VisGeant4/VisG4Transients/interface/VisG4TransientsTwig.h"
#include "VisGeant4/VisG4Fields/interface/VisG4AnnotationsTwig.h"
#include "VisGeant4/VisG4Fields/interface/VisG4MagFieldTwig.h"
#include "Iguana/Studio/interface/IgTwigOps.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/Studio/interface/IgQtObjectMenuMessage.h"
#include "Iguana/Studio/interface/IgStudioDriver.h"
#include "Iguana/Studio/interface/IgDocument.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Studio/interface/IgMDIView.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include <G4RunManager.hh>
#include <QApplication>
#undef emit
#include <classlib/utils/DebugAids.h>
#include <cstdlib>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (VisG4ExampleSetup, "Services/Studio/Session Types/Vis Example--G4 N02");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisG4ExampleSetup::VisG4ExampleSetup (IgState *state)
    : m_state (state),
      m_studio (0),
      m_document (0)
{ ASSERT (m_state); m_state->put (s_key, this); }

VisG4ExampleSetup::~VisG4ExampleSetup (void)
{ ASSERT (m_state); m_state->detach (s_key); /* delete m_document; */ }

bool
VisG4ExampleSetup::setup (IgStudioDriver *into)
{
    ASSERT (into);
    ASSERT (! m_studio);
    m_studio = into;

    // Setup G4.
    QApplication::setOverrideCursor (Qt::waitCursor);

    // Create document.
    ASSERT (! m_document);
    m_document = new IgDocument (m_studio);
    m_document->setName ("Geant4 N02 Visualisation Demo");
    IgPage *page = new IgPage (m_document);
    new IgMDIView (page);

    // Add data.
    G4RunManager *rm = new G4RunManager;
    VisG4UIManager *uim = new VisG4UIManager (m_state);
    VisG4VisManager *vm = new VisG4VisManager (m_state);

    ASSERT (IgQtObjectMenuService::get (m_state));
    IgQtObjectMenuService::get (m_state)
	->add (lat::CreateCallback (new IgTwigOps, &IgTwigOps::menuAction));
    IgQtObjectMenuService::get (m_state)
	->add (lat::CreateCallback (new VisG4TwigOps, &VisG4TwigOps::menuAction));

    // Build and browse the twigs
    IgCompoundTwig *t;
    t = new VisG4GeometryTwig (vm, m_document->data ()->root ());
    t = new VisG4TransientsTwig (m_document->data ()->root ());
    new VisG4TrajectoriesTwig (t);
    t = new VisG4AnnotationsTwig (m_document->data ()->root ());
    new VisG4MagFieldTwig (t, m_state);

    IgRepSet::update (m_document->data ()->root (), IgTwig::STRUCTURE_MASK);

    // Initialise G4 run configuration.
    VisG4ExampleDetector *det = new VisG4ExampleDetector;
    rm->SetUserInitialization (det);
    rm->SetUserInitialization (new VisG4ExamplePhysics);
    rm->SetUserAction (new VisG4ExampleGenerator (det));
    rm->SetUserAction (new VisG4ExampleRunAction);
    rm->SetUserAction (new VisG4ExampleEventAction);
    rm->Initialize ();

    // Start the G4 command line loop (Qt event loop is in studio).
    uim->start ();

    QApplication::restoreOverrideCursor ();
    return true;
}
