//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisDetector/interface/VisDetIvTwig.h"
#include "VisFramework/VisConfigService/interface/VisConfigurationService.h"
#include "Iguana/GLBrowsers/interface/Ig3DShapeManager.h"
#include "Iguana/Models/interface/IgTextRep.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include <classlib/utils/DebugAids.h>
#include <sstream>
#include <iostream>
#include <iomanip>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>

const char* const IV_NONE =
"#Inventor V2.1 ascii\n"
"\n"
" Separator {\n"
"}\n";

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisDetIvTwig::VisDetIvTwig (IgState *state, IgTwig *parent,
			    std::string name, std::string fileName1,
			    std::string fileName2, std::string fileName3)
    : IgSimpleTwig (parent, name, true, false, true),
      m_name (name)
{    
    Ig3DShapeManager *shapeManager = Ig3DShapeManager::get (state);
    ASSERT (shapeManager);
    
    SoNode *none = emptyNode ();
    
    SoNode *det3d = shapeManager->lookup (fileName1);
    if (det3d) 
    {
	m_dets.push_back (det3d);
	m_dets.back ()->ref ();
    }
    else
    {
	m_dets.push_back (none);
	m_dets.back ()->ref ();
    }
    
    
    SoNode *detRPhi = shapeManager->lookup (fileName2);
    if (detRPhi) 
    {
	m_dets.push_back (detRPhi);
	m_dets.back ()->ref ();
    }
    else
    {
	m_dets.push_back (none);
	m_dets.back ()->ref ();
    }
    
    SoNode *detRZ = shapeManager->lookup (fileName3);
    if (detRZ) 
    { 
	m_dets.push_back (detRZ);
	m_dets.back ()->ref ();
    }
    else
    {
	m_dets.push_back (none);
	m_dets.back ()->ref ();
    }
    
    const edm::ParameterSet *pset = VisConfigurationService::pSet ();
    if (pset)
    {
        std::vector<std::string> twigs;
	twigs = pset->getUntrackedParameter<std::vector<std::string> > ("EnabledTwigs", twigs);
	if (std::find (twigs.begin (), twigs.end (), fullName ()) != twigs.end ())
	    selfVisible (true);
    }
}

VisDetIvTwig::~VisDetIvTwig (void)
{
    for (std::vector<SoNode *>::iterator i = m_dets.begin (), iEnd = m_dets.end (); i != iEnd; ++i) 
    {
	(*i)->unref ();
    }
    m_dets.clear ();
}

/** Update object property description. */
void
VisDetIvTwig::update (IgTextRep *rep) 
{
    // Prepare the property description.
    std::ostringstream text;
    text << "<table width='100%'><tr><td>" << m_name << "</td><td>"
	 << "</table>";
    
    // Send it over.
    rep->setText (text.str ());
}
 
/** Update object */
void
VisDetIvTwig::update (Ig3DRep *rep)
{
    // Send it over.
    if (m_dets.size () > 0)
    {
	rep->clear ();
	rep->node ()->addChild (m_dets [0]);
    }
}

void
VisDetIvTwig::update (IgRPhiRep *rep)
{
    if (m_dets.size () >= 2)
    {	
	rep->clear ();
	rep->node ()->addChild (m_dets [1]);
    }
}

void
VisDetIvTwig::update (IgRZRep *rep)
{
    if (m_dets.size () >= 3)
    {	
	rep->clear ();
	rep->node ()->addChild (m_dets [2]);
    }
}

SoNode*
VisDetIvTwig::emptyNode (void)
{
    SoInput in;
    in.putBack (IV_NONE);
    
    SoNode *result;
    SoDB::read (&in, result);
    
    return result;
}
