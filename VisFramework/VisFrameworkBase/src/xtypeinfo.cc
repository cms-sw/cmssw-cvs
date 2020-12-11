#include "VisFramework/VisFrameworkBase/interface/VisQueuedTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisEventIdTwig.h"
#include "VisFramework/VisFrameworkBase/interface/VisFEDRawDataCollectionTwig.h"
#include "VisFramework/VisFrameworkBase/interface/xtypeinfo.h"
#include "VisFramework/VisFrameworkBase/interface/debug.h"
#include "Iguana/GLModels/interface/IgCommon3DReps.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/xtypeinfo.h"
#include "Iguana/GLModels/interface/IgLegoRep.h"
#include "Iguana/Models/interface/IgSimpleTextRep.h"
#include "Iguana/Models/interface/xtypeinfo.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/GLModels/interface/IgSpareRep.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"

/// #logstream flag for this package.
lat::logflag LFfwvis = { 0, "fwvis", true, -1 };

XTYPEINFO_DEF_1 (VisQueuedTwig, IgSimpleTwig, false);
XTYPEINFO_DEF_1 (VisEventIdTwig, VisQueuedTwig, false);
XTYPEINFO_DEF_1 (VisFEDRawDataCollectionTwig, VisQueuedTwig, false);


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisQueuedTwig *twig, Ig3DRep *rep, unsigned mask))
{
    LOG (0, trace, LFfwvis, "doUpdate(VisQueuedTwig["
	 << twig->name () << "],Ig3DRep," << mask << ")\n" << lat::indent);

    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);

    // Skip the update if possible.  We only render if the object is
    // visible, and either SELF_MASK is set (object was changed), or
    // if FLAGS_MASK is set and the rep has no content (either never
    // created or zapped).    
    if (twig->visible ()
	&& (((mask & IgTwig::FLAGS_MASK)
	     && rep->node ()->getNumChildren () == 0)
	    || (mask & IgTwig::SELF_MASK))) 
      twig->update (rep);
    
    LOG (0, trace, LFfwvis, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisQueuedTwig *twig, IgLegoRep *rep, unsigned mask))
{
    LOG (0, trace, LFfwvis, "doUpdate(VisQueuedTwig["
	 << twig->name () << "],IgLegoRep," << mask << ")\n" << lat::indent);

    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    
    // Skip the update if possible.  We only render if the object is
    // visible, and either SELF_MASK is set (object was changed), or
    // if FLAGS_MASK is set and the rep has no content (either never
    // created or zapped).    
    if (twig->visible ()
	&& (((mask & IgTwig::FLAGS_MASK)
	     && rep->node ()->getNumChildren () == 0)
	    || (mask & IgTwig::SELF_MASK)))
      twig->update (rep);
    
    LOG (0, trace, LFfwvis, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisQueuedTwig *twig, IgTextRep *rep, unsigned mask))
{
    LOG (0, trace, LFfwvis, "doUpdate(VisQueuedTwig["
	 << twig->name () << "],IgTextRep," << mask << ")\n" << lat::indent);

    // Let base class implementation update children.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);

    // Re-render only if the object has changed.  Pass the render
    // request to the event consumer thread.
    if (mask & IgTwig::SELF_MASK) twig->update (rep);
    
    LOG (0, trace, LFfwvis, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisQueuedTwig *twig, IgRPhiRep *rep, unsigned mask))
{
    LOG (0, trace, LFfwvis, "doUpdate(VisQueuedTwig["
         << twig->name () << "],IgRPhiRep," << mask << ")\n" << lat::indent);

    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    
    // Skip the update if possible.  We only render if the object is
    // visible, and either SELF_MASK is set (object was changed), or
    // if FLAGS_MASK is set and the rep has no content (either never
    // created or zapped).    
    if (twig->visible ()
        && (((mask & IgTwig::FLAGS_MASK)
             && rep->node ()->getNumChildren () == 0)
            || (mask & IgTwig::SELF_MASK)))
      twig->update (rep);
    
    LOG (0, trace, LFfwvis, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisQueuedTwig *twig, IgRZRep *rep, unsigned mask))
{
    LOG (0, trace, LFfwvis, "doUpdate(VisQueuedTwig["
         << twig->name () << "],IgRZRep," << mask << ")\n" << lat::indent);

    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    
    // Skip the update if possible.  We only render if the object is
    // visible, and either SELF_MASK is set (object was changed), or
    // if FLAGS_MASK is set and the rep has no content (either never
    // created or zapped).    
    if (twig->visible ()
        && (((mask & IgTwig::FLAGS_MASK)
             && rep->node ()->getNumChildren () == 0)
            || (mask & IgTwig::SELF_MASK)))
      twig->update (rep);

    LOG (0, trace, LFfwvis, lat::undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
               (VisQueuedTwig *twig, IgSpareRep *rep, unsigned mask))
{
    LOG (0, trace, LFfwvis, "doUpdate(VisQueuedTwig["
         << twig->name () << "],IgSpareRep," << mask << ")\n" << lat::indent);

    // Do most of the hard work in the base class implementation.
    mmm_doUpdate ((IgTwig *) twig, rep, mask);
    
    // Skip the update if possible.  We only render if the object is
    // visible, and either SELF_MASK is set (object was changed), or
    // if FLAGS_MASK is set and the rep has no content (either never
    // created or zapped).    
    if (twig->visible ()
        && (((mask & IgTwig::FLAGS_MASK)
             && rep->node ()->getNumChildren () == 0)
            || (mask & IgTwig::SELF_MASK)))
      twig->update (rep);
    
    LOG (0, trace, LFfwvis, lat::undent);
}
