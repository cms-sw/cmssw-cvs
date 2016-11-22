//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT
#include "Iguana/Studio/interface/IgQtTextBrowser.h"
#include "Iguana/Studio/interface/IgQtAppWindowService.h"
#include "Iguana/Studio/interface/IgQtSite.h"
#include "Iguana/Models/interface/IgSimpleTextModel.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/Models/interface/IgTextModelEvent.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgSelectionService.h"
#include "Iguana/Framework/interface/IgSelectionMessage.h"

#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Callback.h>
#include <classlib/utils/Log.h>
#include <QApplication>
#include <QMainWindow>
#include <QDockWidget>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>

lat::logflag LFtextbrowser = { 0, "textbrowser", true, -1 };

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

const char *
IgQtTextBrowser::catalogLabel (void)
{ return "Text"; }

IgQtTextBrowser::IgQtTextBrowser (IgState *state, IgSite *site)
    : QTextBrowser (IgQtSite::hostFrom (site)),
      m_state (state),
      m_model (new IgSimpleTextModel)
{
    ASSERT (IgSelectionService::get (m_state));
    IgQtSite::host (site, this);
    
    IgSelectionService::get (m_state)
	->add (lat::CreateCallback (this, &IgQtTextBrowser::selectMessage));
    m_model->listen (IgTextModel::TextChanged,
		     lat::CreateCallback (this, &IgQtTextBrowser::textChanged));

    QMainWindow *mainWindow = dynamic_cast<QMainWindow *>(IgQtAppWindowService::get (m_state)->mainWindow ());
    QDockWidget *dw = new QDockWidget;    
    dw->setWidget (this);    
    mainWindow->addDockWidget (Qt::BottomDockWidgetArea, dw);
    
    dw->show ();
}

IgQtTextBrowser::~IgQtTextBrowser (void)
{
    IgSelectionService::get (m_state)
	->remove (lat::CreateCallback (this, &IgQtTextBrowser::selectMessage));
    m_model->unlisten (IgTextModel::TextChanged,
		       lat::CreateCallback (this, &IgQtTextBrowser::textChanged));
    delete m_model;
}

void
IgQtTextBrowser::browse (IgRepresentable *object)
{ m_model->set (dynamic_cast<IgTextRep *>
	        (IgRepSet::lookup (object, m_model, true))); }

void
IgQtTextBrowser::select (IgTextRep *rep)
{
    LOG (0, trace, LFtextbrowser, "updating to " << rep << "\n");
    setText (rep ? rep->text ().c_str () : "");
}

void
IgQtTextBrowser::selectMessage (IgSelectionMessage message)
{
    // FIXME: broadcast selection message?
    qApp->setOverrideCursor (Qt::waitCursor);
    m_model->set (! message.context () ? 0
		  : dynamic_cast<IgTextRep *>
		    (IgRepSet::lookup (message.context (), m_model, true)));
    qApp->restoreOverrideCursor ();
}

void
IgQtTextBrowser::textChanged (IgTextModelEvent event)
{ select (event.source ()->text ()); }
