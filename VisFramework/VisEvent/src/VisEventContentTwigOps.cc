//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisEvent/interface/VisEventContentTwigOps.h"
#include "VisFramework/VisEvent/interface/VisEventContentTwig.h"
#include "Iguana/Studio/interface/IgTwigOps.h"
#include "Iguana/Studio/interface/IgQtObjectMenu.h"
#include "Iguana/Studio/interface/IgQtObjectMenuMessage.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include <classlib/utils/DebugAids.h>
#include <qapplication.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
VisEventContentTwigOps::menuAction (IgQtObjectMenuMessage message)
{
    static const IgQtObjectMenu::ItemDef defs [] = {
	{ -1, MENU_EXPAND, "Expand",
	  SLOT(actionExpand (int)), MENU_EXPAND }
    };
    static const int ndefs = sizeof (defs)/sizeof (defs[0]);
    
    m_menu = message.menu ();
    if (! (m_twig = dynamic_cast<VisEventContentTwig *> (message.object ())))
	m_menu->removeFromDefs (defs, ndefs);
    else
    {
	m_menu->insertFromDefs (this, defs, ndefs);
	menuUpdate ();
    }
}
 
void
VisEventContentTwigOps::menuUpdate (void)
{
    ASSERT (m_menu);
    // Update menu items to reflec the state of this twig.
    m_menu->changeItem (MENU_EXPAND, m_twig->cascade ()
			? "Update" : "Expand");
}

void
VisEventContentTwigOps::actionExpand (int /* id */)
{
    ASSERT (m_twig);
    QApplication::setOverrideCursor (Qt::waitCursor);
    m_twig->cascade (! m_twig->cascade ());
    IgRepSet::invalidate (m_twig, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
    QApplication::restoreOverrideCursor ();
}
