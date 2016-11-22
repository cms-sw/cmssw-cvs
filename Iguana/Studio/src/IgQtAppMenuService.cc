#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Studio/interface/IgQtAppContextService.h"
#include <classlib/utils/Callback.h>

#define INCLUDE_MENUITEM_DEF
#include <QMainWindow>
#include <QMenuBar>
#include <QMenuItem>
#include <QApplication>
#include <string>
#include <assert.h>

IG_DEFINE_STATE_ELEMENT (IgQtAppMenuService, "Services/Qt/App Menu");

IgQtAppMenuService::IgQtAppMenuService (IgState *state, QWidget *mainWindow)
    : m_state (state),
      m_mainWindow (dynamic_cast<QMainWindow *> (mainWindow))
{
    assert (state);
    assert (mainWindow);
    assert (m_mainWindow);
    state->put (s_key, this);

    QMenuBar *menubar = m_mainWindow->menuBar ();
    QMenu *menuFile = new QMenu (menubar);
    menuFile->setObjectName (QString::fromUtf8 ("menuFile"));
    menuFile->setTearOffEnabled (true);
    menubar->addMenu (menuFile);

    QAction *actionQuit = new QAction (menuFile);
    actionQuit->setObjectName (QString::fromUtf8 ("actionQuit"));
    actionQuit->setEnabled (true);

    menuFile->addAction (actionQuit);

    QObject::connect (actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    
    actionQuit->setText (QApplication::translate ("IgMainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
    actionQuit->setShortcut (QApplication::translate ("IgMainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
    menuFile->setTitle (QApplication::translate ("IgMainWindow", "&File", 0, QApplication::UnicodeUTF8));
}

IgQtAppMenuService::~IgQtAppMenuService (void)
{
    assert (m_state);
    assert (m_mainWindow);

    m_state->detach (s_key);
    m_mainWindow->menuBar ()->clear ();
}

QMenuBar *
IgQtAppMenuService::menuBar (void)
{
    return m_mainWindow->menuBar ();
}

QMenu *
IgQtAppMenuService::subMenu (int id)
{
    QMenuItem *item = menuBar ()->findItem (id);
    if (item)
	return item->menu ();
    return 0;
}

QMenu *
IgQtAppMenuService::subMenu (int id1, int id2)
{
    QMenu *item1 = subMenu (id1);
    if (item1)
    {
	QMenuItem *item2 = item1->findItem (id2);
	if (item2)
	{
	    return item2->menu ();	    
	}
    }

    return 0;
}

QMenu *
IgQtAppMenuService::findByName (QMenu *menu, const char *itemName)
{
    assert (menu);
    
    for (unsigned int i = 0; i < menu->count (); i++)
    {
	QMenuItem *item = menu->findItem (menu->idAt (i));
	
	if (item->text () == itemName)
	    return item->menu ();	    
    }
    return 0;    
}

int
IgQtAppMenuService::index (QMenu *menu, int id)
{
    assert (menu);
    
    for (int i = 0; i < (int) menu->count (); ++i)
	if (menu->idAt (i) >= id)
	    return i;

    return menu->count ();
}

int
IgQtAppMenuService::isIdThere (QMenu *menu, int id)
{
    assert (menu);
    
    for (int i = 0; i < (int) menu->count (); ++i)
	if (menu->idAt (i) == id)
	    return i;
    return -1;    
}

typedef struct 
{
    QMenu 	*m_menu;
    int 	m_id;
    QObject 	*m_target;
    std::string m_slot;    
} MenuConnection;

void 
connectMenuItem (MenuConnection * data)
{
    QMenu *menu = data->m_menu;
    int id = data->m_id;
    QObject *target = data->m_target;
    const char *slot = data->m_slot.c_str ();    
    menu->connectItem (id, target, slot);    
    //std::cerr << "Connected:" << target << std::endl;
}

void 
disconnectMenuItem (MenuConnection *data)
{
    QMenu *menu = data->m_menu;
    int id = data->m_id;
    QObject *target = data->m_target;
    const char * slot = data->m_slot.c_str ();    
    menu->disconnectItem (id, target, slot);
    //std::cerr << "Disconnected:" << target << std::endl;
}

// FIXME: add accelerators support.
void
IgQtAppMenuService::insertItem (QMenu *parentMenu,
				const QString &text,
				QObject *target,
				const char *slot,
				int id,
				IgState *state)
{
    if (isIdThere (parentMenu, id) == -1)
    {
	parentMenu->insertItem (text,
				target,
				slot,
				0,
				id, 
				IgQtAppMenuService::index (parentMenu, id));    
    }
    
    if (state)
    {
	
	// if a state is passed, than the connection between the
	// target and the menuitem is context sensitive.
	IgQtAppContextService *cs = IgQtAppContextService::get (state);
	MenuConnection *data = new MenuConnection;
	assert (cs);
	assert (data);
	
	data->m_menu = parentMenu;
	data->m_id = id;
	data->m_target = target;
	data->m_slot = slot ? slot : "";
	
	cs->addOnFocusIn (lat::CreateCallback (&connectMenuItem,
					       data));	

 	cs->addOnFocusOut (lat::CreateCallback (&disconnectMenuItem,
						data));
        disconnectMenuItem (data);
    }
}

