//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisConfiguration.h"
#include "VisFramework/VisFrameworkBase/interface/VisConfigurable.h"
#include "VisFramework/VisFrameworkBase/interface/VisConfigEditor.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <classlib/utils/Error.h>
#include <classlib/utils/Callback.h>
#include <exception>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (VisConfiguration, "Services/Framework/Configuration");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisConfiguration::VisConfiguration (IgState *state)
    : m_state (state),
      m_editor (new VisConfigEditor (state))
{    
    ASSERT (state);
    state->put (s_key, this);

    ASSERT (IgQtAppMenuService::get (m_state));
    QMenuBar *menubar = IgQtAppMenuService::get (m_state)->menuBar ();    
    QMenu *menuConfig = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuConfig"));
    if (! menuConfig)
    {
	menuConfig = new QMenu (menubar);
	menuConfig->setObjectName (QString::fromUtf8 ("menuConfig"));
	menuConfig->setTearOffEnabled (true);

	QMenu *menuDebug = menubar->findChild <QMenu *> (QString::fromUtf8 ("menuDebug"));
	if (menuDebug != 0)
	    menubar->insertMenu (menuDebug->menuAction (), menuConfig);
	else
	    menubar->addMenu (menuConfig);
	menuConfig->setTitle (QApplication::translate ("IgMainWindow", "Configure", 0, QApplication::UnicodeUTF8));
    }

    QAction *actionConfig = new QAction (menuConfig);
    actionConfig->setObjectName (QString::fromUtf8 ("actionConfig"));
    actionConfig->setEnabled (true);
    menuConfig->addAction (actionConfig);
    QObject::connect (actionConfig, SIGNAL(triggered()), m_editor, SLOT(show ()));
    actionConfig->setText (QApplication::translate ("IgMainWindow", "Edit...", 0, QApplication::UnicodeUTF8));
}

VisConfiguration::~VisConfiguration (void) 
{    
    ASSERT (m_state);
    m_state->detach (s_key);
    clear ();
}

void 
VisConfiguration::dumpConfig (VisState& state) const 
{
    try
    {
	VisConfigMapConItr e = m_registry.end ();
	VisConfigMapConItr p = m_registry.begin ();
	for (; p != e; ++p) 
	{
	    ASSERT ((*p).second);
 	    LOG (0, trace, LFfwvis, "VisConfiguration[" << (*p).first
 		 << ", " << (*p).second->get () << "]::dump()\n");
	    state [(*p).first] = (*p).second->get (); 
	}
    }
    catch (cms::Exception& e)
    {
	std::cout << "**** VisConfiguration::dump ****\n cms::Exception " << e.explainSelf () << std::endl;
    }
    catch (lat::Error &e) 
    {
	std::cout << "**** VisConfiguration::dump ****\n lat::Error " << e.explainSelf () << std::endl;
    }
    catch (std::exception &e) 
    {
	std::cout << "**** VisConfiguration::dump ****\n Standard library exception caught: " << e.what () << std::endl;	
    }
    catch (...) 
    {
	std::cout << "**** VisConfiguration::dump ****\n Unknown Exception" << std::endl;
    }
}

void 
VisConfiguration::insert (const std::string &iname, VisConfigurable *ihim) 
{
    m_registry.insert (std::make_pair (iname, ihim));
}

void
VisConfiguration::erase (const VisConfigurable *ihim)
{
    VisConfigMapItr e = m_registry.end ();
    VisConfigMapItr p = find (ihim, m_registry.begin ());
    while (p != e) 
    { 
	VisConfigMapItr q = p; 
	++q; 
	m_registry.erase (p); 
	p = q;  // ospace does not return next...
	p = find (ihim, p);
    }
}

void
VisConfiguration::clear (void) 
{
    m_registry.clear ();
}

VisConfiguration::VisConfigMap::iterator 
VisConfiguration::find (const VisConfigurable *ihim, VisConfigMapItr p) 
{ 
    VisConfigMapItr e = m_registry.end ();
    while (p != e) 
    {
	if ((*p).second == ihim) break; ++p;
    }
    return p;
}

void
VisConfiguration::modify (const std::string & iname, const std::string & ivalue) 
{   
    std::string val (ivalue);
    set (iname, val);
}

void 
VisConfiguration::set (const std::string & iname, const std::string & ivalue) 
{
    SRANGE range = m_registry.equal_range (iname);
    bool notEmpty = range.first != range.second;
    if (notEmpty)
    {
	// set method may erase itself making the iterator invalid...
	VisConfigMapItr p = range.first; 
	while (p != range.second) 
	{
	    VisConfigMapItr q = p; 
	    ++p; 
	    VisConfigurable *bs = (*q).second;
	    (*bs).set (ivalue);
	}
    }
}
