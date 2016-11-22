#ifndef IGUANA_STUDIO_IG_QT_APP_MENU_SERVICE_H
# define IGUANA_STUDIO_IG_QT_APP_MENU_SERVICE_H

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"

class QWidget;
class QMainWindow;
class QMenuBar;
class QMenu;
class QString;
class QObject;

class IGUANA_STUDIO_API IgQtAppMenuService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtAppMenuService);
public:
    static const int	MENU_GROUP_RANGE = 1000;
    static const int	MENU_SUBGROUP_RANGE = 50;

    static const int	MENU_ID_FILE 		= 0;
    static const int	MENU_ID_FILE_SAVE	= 1;
    static const int	MENU_ID_FILE_PRINT	= 2;
    static const int	MENU_ID_FILE_QUIT 	= (MENU_ID_FILE
						   + 10 * MENU_SUBGROUP_RANGE);
    static const int	MENU_ID_HELP 		= 100 * MENU_GROUP_RANGE;

    IgQtAppMenuService (IgState *state, QWidget *mainWindow);
    ~IgQtAppMenuService (void);

    // FIXME: Add methods to manage the main menu (and popups?).
    // Organise menus by ids given in insertion stuff.  Use the app
    // context service to manage the enabling of menu items on popup.
    // Insert separators if ids change by certain boundary (1000?).

    QMenuBar *			menuBar (void);
    QMenu *			subMenu (int id);
    QMenu *			subMenu (int id1, int id2);
    QMenu *			findByName (QMenu *menu, 
					    const char *itemName);
    
    static int			index (QMenu *menu, int id);
    static int 			isIdThere (QMenu *menu, int id);
    static void			insertItem (QMenu *parentMenu,
					    const QString &text,
					    QObject *target,
					    const char *slot,
					    int id,
					    IgState *state = 0);
private:
    IgState			*m_state;
    QMainWindow			*m_mainWindow;

    // undefined semantics
    IgQtAppMenuService (const IgQtAppMenuService &);
    IgQtAppMenuService &operator= (const IgQtAppMenuService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_QT_APP_MENU_SERVICE_H
