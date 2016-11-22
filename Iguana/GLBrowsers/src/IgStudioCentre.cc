//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/IgStudioCentre.h"
#include "Iguana/GLBrowsers/interface/IgControlCentre.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include <QApplication>
#include <QMenuBar>
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Callback.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgStudioCentre, "Extensions/Qt GUI/Control Centre");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Instantiate control centre into a state (normally studio).  */
IgStudioCentre::IgStudioCentre (IgState *state)
    : m_state (state)
{
    ASSERT (m_state);
    m_state->put (s_key, this);
    
    m_centre = new IgControlCentre (0);

    IgQtAppActiveService::get (m_state)
	->add (lat::CreateCallback (this, &IgStudioCentre::appActive));

    ASSERT (IgQtAppMenuService::get (m_state));
    QMenuBar *menubar = IgQtAppMenuService::get (m_state)->menuBar ();
    QMenu *menuFile = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuFile"));
 
    QAction *actionConCen = new QAction (menuFile);
    actionConCen->setObjectName (QString::fromUtf8 ("actionConCen"));
    actionConCen->setEnabled (true);

    QAction *actionQuit = menuFile->findChild <QAction *> (QString::fromUtf8 ("actionQuit"));

    menuFile->insertAction (actionQuit, actionConCen);
    menuFile->insertAction (actionQuit, menuFile->addSeparator ());

    QObject::connect (actionConCen, SIGNAL(triggered()), m_centre, SLOT(show()));
    actionConCen->setText (QApplication::translate ("IgMainWindow", "&Control centre", 0, QApplication::UnicodeUTF8));
}

IgControlCentre*
IgStudioCentre::controlCentre (void)
{ return m_centre; }

void
IgStudioCentre::appActive (IgQtAppActiveMessage message)
{
    if(message.active ())
      m_centre->updateOnWindowActive ();
}
