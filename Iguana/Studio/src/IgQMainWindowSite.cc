//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Studio/interface/IgQMainWindowSite.h"
#include <QMainWindow>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQMainWindowSite::IgQMainWindowSite (IgState *, IgSite *parent)
    : m_window (new QMainWindow)
{
  // m_window->setCentralWidget (new QMdiArea);
}

const char *
IgQMainWindowSite::catalogLabel (void)
{ return "QMainWindow"; }

QWidget *
IgQMainWindowSite::self (void)
{
    return m_window;
}

QWidget *
IgQMainWindowSite::host (void)
{
    return m_window;
}

void
IgQMainWindowSite::host (QWidget *child)
{
    return m_window->setCentralWidget (child);
}

