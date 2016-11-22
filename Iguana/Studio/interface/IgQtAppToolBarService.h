#ifndef IGUANA_STUDIO_IG_QT_APP_TOOL_BAR_SERVICE_H
# define IGUANA_STUDIO_IG_QT_APP_TOOL_BAR_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <map>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;
class QToolBar;
class QToolButton;
class QIcon;
class QObject;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtAppToolBarService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppToolBarService);
public:
    IgQtAppToolBarService (IgState *state, QWidget *mainWindow);
    ~IgQtAppToolBarService (void);

    // FIXME: Provide methods to manage the tool bar.  Synchronise
    // with menu items and the app context service to enable/disable
    // items.
    QToolBar *toolBar (const int id, const std::string &label);
    QToolButton *toolBarButton (IgState *state,
				const int barId,
				const int buttonId,
				const QIcon &iconSet,
				const std::string &name,
				QObject	*target,
				const char *slot,
				const char *signal = "");
    
     QToolButton * getToolBarButton (const int barId,
				     const int buttonId);
				     
private:
    IgState		*m_state;
    QWidget		*m_mainWindow;

    // undefined semantics
    IgQtAppToolBarService (const IgQtAppToolBarService &);
    IgQtAppToolBarService &operator= (const IgQtAppToolBarService &);
    
    // QUESTION: should we use QWidget instead?
    typedef std::map<int, QToolBar *> 		ToolBarMap;
    typedef std::map<int, QToolButton *> 	ToolButtonMap;
    
    ToolBarMap		m_toolBarMap;
    ToolButtonMap	m_toolButtonMap;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_APP_TOOL_BAR_SERVICE_H
