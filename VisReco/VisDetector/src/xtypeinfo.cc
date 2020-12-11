//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisDetector/interface/xtypeinfo.h"
#include "VisReco/VisDetector/interface/VisDetIvTwig.h"
#include "VisReco/VisDetector/interface/VisDetTextureTwig.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgCommon3DReps.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/GLModels/interface/xtypeinfo.h"
#include "Iguana/Models/interface/IgSimpleTextRep.h"
#include "Iguana/Models/interface/xtypeinfo.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/// #logstream flag for this package.
lat::logflag LFdetvis = { 0, "detvis", true, -1 };

XTYPEINFO_DEF_1 (VisDetIvTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (VisDetTextureTwig, IgSimpleTwig, false);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisDetIvTwig *twig, Ig3DRep *rep, unsigned mask))
{
    LOG (0, trace, LFdetvis, "doUpdate(VisDetIvTwig["
	 << twig->name () << "],Ig3DRep," << mask << ")\n" << lat::indent);

    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);

    if (! twig->visible ()) 
    {
        return;
    }
    else 
    {
	twig->update (rep);
    }
    
    LOG (0, trace, LFdetvis, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisDetIvTwig *twig, IgTextRep *rep, unsigned mask))
{
    LOG (0, trace, LFdetvis, "doUpdate(VisDetIvTwig["
	 << twig->name () << "],IgTextRep," << mask << ")\n" << lat::indent);

    // Let base class implementation update children.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);

    // Re-render only if the object has changed.  Pass the render
    // request to the event consumer thread.
    if (mask & IgTwig::SELF_MASK) twig->update (rep);

    LOG (0, trace, LFdetvis, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisDetIvTwig *twig, IgRPhiRep *rep, unsigned mask))
{
    LOG (0, trace, LFdetvis, "doUpdate(VisDetIvTwig["
         << twig->name () << "],IgRPhiRep," << mask << ")\n" << lat::indent);

    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);

    if (! twig->visible ()) 
    {
        return;
    }
    else
    {
	twig->update (rep);
    }
    
    LOG (0, trace, LFdetvis, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisDetIvTwig *twig, IgRZRep *rep, unsigned mask))
{
    LOG (0, trace, LFdetvis, "doUpdate(VisDetIvTwig["
         << twig->name () << "],IgRZRep," << mask << ")\n" << lat::indent);

    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    
    if (! twig->visible ()) 
    {
        return;
    }
    else
    {
	twig->update (rep);
    }
    
    LOG (0, trace, LFdetvis, lat::undent);
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisDetTextureTwig *twig, Ig3DRep *rep, unsigned mask))
{
    LOG (0, trace, LFdetvis, "doUpdate(VisDetIvTwig["
	 << twig->name () << "],Ig3DRep," << mask << ")\n" << lat::indent);

    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);

    if (! twig->visible ()) 
    {
        return;
    }
    else 
    {
	twig->update (rep);
    }
    
    LOG (0, trace, LFdetvis, lat::undent);
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisDetTextureTwig *twig, IgRPhiRep *rep, unsigned mask))
{
    LOG (0, trace, LFdetvis, "doUpdate(VisDetTextureTwig["
         << twig->name () << "],IgRPhiRep," << mask << ")\n" << lat::indent);

    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);

    if (! twig->visible ()) 
    {
        return;
    }
    else
    {
	twig->update (rep);
    }
    
    LOG (0, trace, LFdetvis, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisDetTextureTwig *twig, IgRZRep *rep, unsigned mask))
{
    LOG (0, trace, LFdetvis, "doUpdate(VisDetTextureTwig["
         << twig->name () << "],IgRZRep," << mask << ")\n" << lat::indent);

    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    
    if (! twig->visible ()) 
    {
        return;
    }
    else
    {
	twig->update (rep);
    }
    
    LOG (0, trace, LFdetvis, lat::undent);
}


