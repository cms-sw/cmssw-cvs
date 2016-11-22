//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/IgOIVExtension.h"
#include "Iguana/GLBrowsers/interface/Ig3DSystem.h"
#include "Iguana/GLBrowsers/interface/IgIVView.h"
#include "Iguana/Studio/interface/IgDocument.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Studio/interface/IgStudioDriver.h"
#include "Iguana/Studio/interface/IgQtSite.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgOIVExtension, "Services/Studio/Session Types/Vis Example--Open Inventor File Reader; IvReader");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOIVExtension::IgOIVExtension (IgState *state)
    : m_state (state),
      m_view (0)
{
    ASSERT (m_state);
    m_state->put (s_key, this);
}
 
IgOIVExtension::~IgOIVExtension ()
{ delete m_view; }
 
bool
IgOIVExtension::setup (IgStudioDriver *into)
{
    ASSERT (into);
    
    // Initialise SoQt if it hasn't been done yet.
    Ig3DSystem::init (m_state, IgQtSite::selfFrom (into->mainSite ()));
    
    m_view = new IgIVView (new IgPage (new IgDocument (into)));
    
    return true;
}
