#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/Studio/interface/IgQtObjectMenuMessage.h"
#include "Iguana/Studio/interface/IgQtObjectMenu.h"
#include <classlib/utils/DebugAids.h>
#include <QMenu>

IG_DEFINE_STATE_ELEMENT (IgQtObjectMenuService, "Services/Qt/Object Menu");

IgQtObjectMenuService::IgQtObjectMenuService (IgState *state)
    : m_state (state),
      m_menu (new QMenu),
      m_proxy (0)
{
    ASSERT (m_state);
    m_state->put (s_key, this);
    m_menu->setCaption ("Object Operations");
    m_proxy = new IgQtObjectMenu (m_menu);
}

IgQtObjectMenuService::~IgQtObjectMenuService (void)
{
    ASSERT (m_state);
    ASSERT (m_menu);
    ASSERT (m_proxy);

    m_state->detach (s_key);
    delete m_proxy;
    delete m_menu;
}

IgQtObjectMenu *
IgQtObjectMenuService::menu (void) const
{
    ASSERT (m_state);
    ASSERT (m_menu);
    ASSERT (m_proxy);
    return m_proxy;
}

QMenu *
IgQtObjectMenuService::realMenu (void) const
{
    ASSERT (m_state);
    ASSERT (m_menu);
    ASSERT (m_proxy);
    return m_menu;
}

QMenu *
IgQtObjectMenuService::request (IgRepresentable *object)
{
    ASSERT (object);
    ASSERT (m_state);
    ASSERT (m_menu);
    ASSERT (m_proxy);

    // Update the master and real menus.
    broadcast (IgQtObjectMenuMessage (m_proxy, object));
    return m_menu;
}

void
IgQtObjectMenuService::postUpdate (IgRepresentable *object)
{
    // FIXME: Do this only if the menu is torn off.  I don't know of a
    // way to find out whether that's the case, so we always do it.
    ASSERT (object);
    ASSERT (m_state);
    ASSERT (m_menu);
    ASSERT (m_proxy);

    broadcast (IgQtObjectMenuMessage (m_proxy, object));
}
