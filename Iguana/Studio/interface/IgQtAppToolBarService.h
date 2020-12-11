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

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgQtAppToolBarService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppToolBarService);
public:
    IgQtAppToolBarService (IgState *state, QWidget *mainWindow);
    ~IgQtAppToolBarService (void);

    QToolBar *toolBar (const int id, const std::string &label);
				     
private:
    IgState		*m_state;
    QWidget		*m_mainWindow;

    // undefined semantics
    IgQtAppToolBarService (const IgQtAppToolBarService &);
    IgQtAppToolBarService &operator= (const IgQtAppToolBarService &);
    
    typedef std::map<int, QToolBar *> 		ToolBarMap;
    ToolBarMap		m_toolBarMap;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_APP_TOOL_BAR_SERVICE_H
