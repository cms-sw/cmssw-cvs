//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisExceptionService.h"
#include "Iguana/Framework/interface/IgArgsElement.h"
#include "Iguana/Studio/interface/IgQtAppWindowService.h"
#include <QApplication>
#include <QMessageBox>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (VisExceptionService, "Services/Framework/GUI/Exception");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisExceptionService::VisExceptionService (IgState *state)
    : m_state (state),
      m_mainWindow (IgQtAppWindowService::get (state)->mainWindow ()),
      m_msg (state, lat::CreateCallback (this, &VisExceptionService::msgChanged))
{
    // Register ourselves into the state.
    ASSERT (m_state);
    m_state->put (s_key, this);
}

VisExceptionService::~VisExceptionService (void)
{
    ASSERT (m_state);
    m_state->detach (s_key);
}

void
VisExceptionService::msgChanged (void)
{}

void
VisExceptionService::exceptionCallback (std::exception *e)
{
    IgArgsElement *args = IgArgsElement::get (m_state);
    
    if (m_msg.value () == "GUI")
	QMessageBox::critical (m_mainWindow, "Standard Library Exception", 
			       QString ("<p>Standard library exception caught in %1:</p>"
					"<p>%2</p>")
			       .arg (QString::fromStdString (args->argv () [0]))
			       .arg (QString::fromStdString (e->what ())));
}

void
VisExceptionService::errorCallback (lat::Error *e)
{
    IgArgsElement *args = IgArgsElement::get (m_state);
    
    QApplication::restoreOverrideCursor ();
    
    if (m_msg.value () == "GUI")
	QMessageBox::critical (m_mainWindow, "lat::Error", 
			       QString ("<p>Error caught in %1:</p>"
					"<p>%2</p>")
			       .arg (QString::fromStdString (args->argv () [0]))
			       .arg (QString::fromStdString (e->explainSelf ())));
}

void
VisExceptionService::cmsExceptionCallback (cms::Exception *e)
{    
    IgArgsElement *args = IgArgsElement::get (m_state);
    
    QApplication::restoreOverrideCursor ();
    
    if (m_msg.value () == "GUI")
	QMessageBox::critical (m_mainWindow, "cms::Exception", 
			       QString ("<p>CMS exception caught in %1:</p>"
					"<p>%2</p>")
			       .arg (QString::fromStdString (args->argv () [0]))
			       .arg (QString::fromStdString (e->explainSelf ())));
}

void
VisExceptionService::unhandledExceptionCallback (void)
{
    IgArgsElement *args = IgArgsElement::get (m_state);
    
    QApplication::restoreOverrideCursor ();
    
    if (m_msg.value () == "GUI")
	QMessageBox::critical (m_mainWindow, "Unknown Exception", 
			       QString ("<p>Unknown exception caught in %1</p>")
			       .arg (QString::fromStdString (args->argv () [0])));
}
