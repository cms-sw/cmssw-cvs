//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/IgCommonViewCategory.h"
#include "Iguana/Studio/interface/IgQtAppContextService.h"
#include "Iguana/GLBrowsers/interface/Ig3DBaseBrowser.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgCommonViewCategory::IgCommonViewCategory (IgState *state, const QString& name)
    : IgControlCategory (state, QString("View Properties/")+name),
      m_source		(0)
{}

void 
IgCommonViewCategory::browserChanged (Ig3DBaseBrowser *source)
{
    if (source)
	m_source = source;
}

//////////////////////////////////////////////////////////////////////////
void
IgCommonViewCategory::show (void)
{
    IgControlCategory::show();
    hideBodyScrollArea ();
}

//////////////////////////////////////////////////////////////////////
void
IgCommonViewCategory::registerBrowser (IgState *state, Ig3DBaseBrowser *browser)
{
    IgQtAppContextService *cs = IgQtAppContextService::get (state);
    ASSERT (cs);    
    cs->addOnFocusIn (lat::CreateCallback (this,
					   &IgCommonViewCategory::browserChanged,
					   browser));    
    IgControlCategory::registerMe (state);
}
