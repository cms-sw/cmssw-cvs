//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/IgCommonViewPropertiesCategory.h"
#include "Iguana/GLBrowsers/interface/IgControlCategory.h"
#include "Iguana/GLBrowsers/interface/IgQtApplicationCategory.h"
#include "Iguana/GLBrowsers/interface/IgGLRenderCategory.h"
#include "Iguana/GLBrowsers/interface/IgExaminerViewerCategory.h"
#include "Iguana/GLBrowsers/interface/IgGL2PSPrintCategory.h"
#include "Iguana/GLBrowsers/interface/IgSceneManagerCategory.h"
#include "Iguana/Framework/interface/IgState.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgCommonViewPropertiesCategory, "/ControlCentre/Categories/View Properties");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgCommonViewPropertiesCategory::IgCommonViewPropertiesCategory (IgState *state)
{
    ASSERT (state);
    m_state =  state;
    IgCommonViewCategory* category = 0;
    
    category = IgExaminerViewerCategory::get (state);
    if (!category)
    {
	category = 
	    new IgExaminerViewerCategory (state);
    }
    m_categories.push_back (category);

    category = IgGL2PSPrintCategory::get (state);
    if (!category)
    {
	category = 
	    new IgGL2PSPrintCategory (state);
    }
    m_categories.push_back (category);
    
    category = IgGLRenderCategory::get (state);
    if (!category)
    {
	category = 
	    new IgGLRenderCategory (state);
    }
    m_categories.push_back (category);

    category = IgQtApplicationCategory::get (state);
    if (!category)
    {
	category = 
	    new IgQtApplicationCategory (state);
    }
    m_categories.push_back (category);

    category = IgSceneManagerCategory::get (state);
    if (!category)
    {
	category = 
	    new IgSceneManagerCategory (state);
    }
    m_categories.push_back (category);
    
    state->put (s_key, this);
}

IgCommonViewPropertiesCategory::~IgCommonViewPropertiesCategory ()
{
    while(!m_categories.empty ())
    {
      delete m_categories[0];
      m_categories.erase (m_categories.begin ());
    }
    state()->detach (s_key);
}

void
IgCommonViewPropertiesCategory::registerBrowser (IgState *state,
                                         Ig3DBaseBrowser *browser)
{
    for(unsigned int i = 0; i < m_categories.size (); i++)
      m_categories[i]->registerBrowser (state, browser);
}
