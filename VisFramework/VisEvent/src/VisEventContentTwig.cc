//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT
#include "VisFramework/VisEvent/interface/VisEventContentTwig.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
#include "VisFramework/VisFrameworkBase/interface/VisTwigFactroyService.h"
#include "VisFramework/VisEvent/interface/xtypeinfo.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventSelector.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ConstProductRegistry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Studio/interface/IgQtAppStatusBarService.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/Models/interface/xtypeinfo.h"
#include <QApplication>
#include <iostream>
#include <sstream>
#include <iomanip>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisEventContentTwig::VisEventContentTwig (IgState 		*state, 
					  IgTwig 		*parent,
					  const std::string 	name /* = "" */,
					  bool              	traverse /* false */,
					  bool              	visible /* false */,
					  bool              	open /* false */)
    : IgCompoundTwig (parent),
      m_state (state),
      m_name (name),
      m_text ("No info")
{
    ASSERT (parent);
    ASSERT (state);
}

std::string
VisEventContentTwig::name (void) const
{ return m_name; }
 
void
VisEventContentTwig::name (const std::string &)
{ ASSERT (false); }

bool
VisEventContentTwig::expand (void)
{
    if (IgCompoundTwig::expand ()) 
    {
 	// if (children ()) destroy ();
 	// VisEventNavigator::build (observed (), this);
	
	return true;
    }

    return false;
}

bool
VisEventContentTwig::selfTraverse (void) const
{ return flag (TRAVERSE); }
  
void
VisEventContentTwig::selfTraverse (bool value)
{ flag (TRAVERSE, value); }

bool
VisEventContentTwig::selfVisible (void) const
{ return flag (VISIBLE); }
  
void
VisEventContentTwig::selfVisible (bool value)
{ flag (VISIBLE, value); }

bool
VisEventContentTwig::cascade (void) const
{ return flag (CASCADE); }

void
VisEventContentTwig::cascade (bool value)
{ flag (CASCADE, value); }

void
VisEventContentTwig::onPreEvent (const edm::EventID &eventID, 
				 const edm::Timestamp &timeStamp)
{
    const edm::ParameterSet *pset = VisConfigurationService::pSet ();
    if(! pset)
    {
        edm::ParameterSet p;
	VisConfigurationService *visServise = new VisConfigurationService (p);
	pset = visServise->pSet ();
	delete visServise;
    }
    // We construct the dynamic content here and 
    // do it only once - product registry gives us the product list 
    // which is the list of all 'potential' products 
    // which could ever be made in the job. 
    // Therefore event to event we could have less products 
    // then the total but never more.
    if (! children ())
    // FIXME: We need to check if the content changes from event to event
    // if (children ()) destroy ();
    {
	edm::Service<edm::ConstProductRegistry> reg;
    
	std::string friendlyName;
	std::string moduleLabel;
	std::string instanceName;
	std::string processName;

	IgSimpleTwig *unknownTwigs = 0;

	bool flag = pset->getUntrackedParameter<bool> ("UnknownDataCollections", true);
	if (flag)
	{
	    unknownTwigs = new IgSimpleTwig (this, "Unknown Collection Types (load more iglets which knows how to represent them)");
	    unknownTwigs->selfTraverse (false);
	    unknownTwigs->selfVisible (false);
	    unknownTwigs->cascade (false);
	}
	
	// Loop over provenance of products in registry.
	for (edm::ProductRegistry::ProductList::const_iterator it = reg->productList ().begin ();
	     it != reg->productList ().end (); ++it) 
	{
	    // See FWCore/Framework/interface/BranchDescription.h
	    // BranchDescription contains all the information for the product.
	    edm::BranchDescription desc = it->second;
	    friendlyName = desc.friendlyClassName ();
	    moduleLabel = desc.moduleLabel ();
	    instanceName = desc.productInstanceName ();
	    processName = desc.processName ();

	    std::string twigName = friendlyName
				   + std::string (":") + moduleLabel
				   + std::string (":") + instanceName 
				   + std::string (":") + processName;
// 				   + std::string (" + \"") + moduleLabel
// 				   + std::string ("\" + \"") + instanceName 
// 				   + std::string ("\" + \"") + processName + "\"";

	    VisEventSelector visSel (friendlyName, moduleLabel, instanceName, processName);

	    try 
	    {
		VisTwigFactroyService *tfService = VisTwigFactroyService::get (state ());
		if (tfService)
		{
		    tfService->createTwig (friendlyName, state (), this, friendlyName, moduleLabel, instanceName, processName);
		}
	    }
	    catch (...)
	    {
		if (flag)
		{
		    ASSERT (unknownTwigs);
		    IgSimpleTwig *twig = new IgSimpleTwig (unknownTwigs, twigName);
		    twig->selfTraverse (false);
		    twig->selfVisible (false);
		}
	    }
	}
    }
}

void
VisEventContentTwig::onNewEvent (const edm::Event &event,
				 const edm::EventSetup &eventSetup)
{
    LOG (1, trace, LFfwvis, "VisEventContentTwig[" << name ()
	 << "]::onNewEvent()\n");

    m_text = QString ("Run # %1, event # %2")
	     .arg (event.id ().run ())
	     .arg (event.id ().event ()).latin1 ();
    
    if (IgQtAppStatusBarService *sbar = IgQtAppStatusBarService::get (state ()))
        sbar->setMessage (QString::fromStdString (m_text));
    
    if (! this->cascade ())
    {
	this->cascade (true);
	if (! this->selfTraverse ())
	    this->selfTraverse (true);

	IgRepSet::invalidate (this, IgTwig::SELF_MASK | IgTwig::STRUCTURE_MASK);
    }
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisEventContentTwig *twig, IgTextRep *rep, unsigned))
{
    // Prepare property description.
    std::ostringstream  text;
     
    text << twig->name () << "<br>";
    rep->setText (text.str ());
}
