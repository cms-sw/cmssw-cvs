#ifndef IGUANA_STUDIO_IG_QT_OBJECT_MENU_SERVICE_H
# define IGUANA_STUDIO_IG_QT_OBJECT_MENU_SERVICE_H

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include "Iguana/Framework/interface/IgDispatcher.h"

class IgRepresentable;
class IgQtObjectMenuMessage;
class IgQtObjectMenu;
class QMenu;

/** A manager for an object-specific #QPopupMenu.

    This service manages a context-sensitive popup menu for browsers.
    There is one menu that is updated to reflect actions relevant to
    the object at hand.  Browsers can use this service to pop up a
    menu when and where they desire; other parts of the application
    can register with this service to add object-specific actions to
    such menus.

    @sa #IgQtObjectMenuMessage, #IgQtObjectMenu.  */

class IGUANA_STUDIO_API IgQtObjectMenuService
    : public IgStateElement, public IgDispatcher<IgQtObjectMenuMessage>
{
    IG_DECLARE_STATE_ELEMENT (IgQtObjectMenuService);
public:
    IgQtObjectMenuService (IgState *state);
    ~IgQtObjectMenuService (void);

    QMenu *			request (IgRepresentable *object);
    void			postUpdate (IgRepresentable *object);
    IgQtObjectMenu *		menu (void) const;
    QMenu *			realMenu (void) const;

private:
    IgState			*m_state;
    QMenu			*m_menu;
    IgQtObjectMenu		*m_proxy;
};

#endif // IGUANA_STUDIO_IG_QT_OBJECT_MENU_SERVICE_H
