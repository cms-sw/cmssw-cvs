//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Studio/interface/IgQtAppToolBarService.h"
#include "Iguana/Framework/interface/IgState.h"
#include <QMainWindow>
#include <QToolBar>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQtAppToolBarService, "Services/Qt/Tool Bar");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtAppToolBarService::IgQtAppToolBarService (IgState *state,
					      QWidget *mainWindow)
    : m_state (state),
      m_mainWindow (mainWindow)
{
    ASSERT (state);
    ASSERT (mainWindow);
    state->put (s_key, this);
}

IgQtAppToolBarService::~IgQtAppToolBarService (void)
{
    ASSERT (m_state);
    ASSERT (m_mainWindow);
    m_state->detach (s_key);
}

QToolBar *
IgQtAppToolBarService::toolBar (const int id,
				const std::string &label)
{
    if (m_toolBarMap[id] == 0)
    {
	QToolBar *bar = new QToolBar (label.c_str (), m_mainWindow);
	bar->setCaption (label.c_str ());
	m_toolBarMap[id] = bar;	
	dynamic_cast<QMainWindow *>(m_mainWindow)->addToolBar (bar);	
    }
    return m_toolBarMap[id];    
}
