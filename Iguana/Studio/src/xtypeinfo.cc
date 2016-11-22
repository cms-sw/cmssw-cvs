#define QT_NO_EMIT
#include "Iguana/Studio/interface/IgQtTextBrowser.h"
#include "Iguana/Studio/interface/IgQtTwigBrowser.h"
#include "Iguana/Studio/interface/IgQtTwigRep.h"
#include "Iguana/Studio/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/xtypeinfo.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include <classlib/utils/Log.h>

IGUANA_STUDIO_API lat::logflag LFqttwig = { 0, "qttwig", true, -1 };

XTYPEINFO_DEF_1 (IgQtTextBrowser, IgBrowser, false);
XTYPEINFO_DEF_1 (IgQtTwigRep, IgRep, false);
XTYPEINFO_DEF_2 (IgQtTwigBrowser, IgBrowser, false, IgModel, false);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
static IgTwig *
findRepresentedAncestor (IgTwig *twig, IgModel *model)
{
    for (twig = twig->parent (); twig; twig = twig->parent ())
	if (IgRepSet::lookup (twig, model))
	    return twig;

    return 0;
}

MMM_DEFUN_FUNC(IgRepContext *,IgBrowserMethods::,doRepresent,
	       (IgTwig *twig, IgQtTwigBrowser *model))
{
    LOG (0, trace, LFqttwig, "  doRepresent (IgTwig[" << twig->name ()
	 << "],IgQtTwigBrowser)\n" << lat::indent);

    // Make sure that if any ancestor of this twig is represented, we
    // put all intermediate ancestors in there as well: we may come in
    // here from another browser that has a fully expanded view, and
    // want to expand the full path.  However, don't expand all the
    // way to the top -- stop at the first visible ancestor.  If no
    // ancestor is visible, just put this twig at the top level.
    IgTwig		*ancestor = findRepresentedAncestor (twig, model);
    IgTwig		*parent = twig->parent ();
    IgQtTwigRep		*prep = !ancestor ? 0 : dynamic_cast<IgQtTwigRep *>
				(IgRepSet::lookup (parent, model, true));
    IgQtTwigRep		*prev = 0;
    unsigned		index;

    if (prep && (index = parent->indexOf (twig)) > 0)
	prev = dynamic_cast<IgQtTwigRep *>
	       (IgRepSet::lookup (parent->child (index-1), model));

    IgQtTwigRep *rep = IgQtTwigRep::create (model, prep, prev,
					    twig->name().c_str (),
					    twig->traverse (),
					    twig->selfVisible (),
					    twig->cascade ());

    // Always expand the twig one more level to be able to always show
    // the right kind of expandable handle.  (FIXME: too aggressive?)
    // We don't create reps for the children, just expand the twig.
    twig->expand ();
    rep->setExpanded(twig->children ());

    LOG (0, trace, LFqttwig, lat::undent);
    return new IgRepContext (twig, rep);
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doExpand,
	       (IgTwig *twig,IgQtTwigRep *rep))
{
    LOG (0, trace, LFqttwig, "  doExpand (IgTwig[" << twig->name ()
	 << "],IgQtTwigRep)\n" << lat::indent);

    twig->expand ();
    for (unsigned i = 0, n = twig->children (); i < n; ++i)
      IgRepSet::lookup (twig->child (i), rep->model (), true);

    LOG (0, trace, LFqttwig, lat::undent);
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doCommit,
	       (IgQtTwigRep *rep, IgTwig *twig, unsigned field))
{
    LOG (0, trace, LFqttwig, "  doCommit (IgQtTwigRep,IgTwig[" << twig->name ()
	 << "]," << field << ")\n" << lat::indent);
    switch (field)
    {
    case IgQtTwigRep::CASCADE_FIELD:
	// Fiddle the flag; we are the only ones who care about cascade
	twig->cascade (rep->isExpanded());
	break;

    case IgQtTwigRep::TRAVERSE_FIELD:
	// Set and propage to other reps
	twig->selfTraverse (rep->checked (field));
        IgRepSet::invalidate (rep->context (),
			      IgTwig::STRUCTURE_MASK
			      | IgTwig::FLAGS_MASK);
	break;

    case IgQtTwigRep::VISIBLE_FIELD:
	// Set and propage to other reps
	twig->selfVisible (rep->checked (field));
        IgRepSet::invalidate (rep->context (), IgTwig::FLAGS_MASK);
	break;

    default:
	ASSERT (false);
    }

    LOG (0, trace, LFqttwig, lat::undent);
}

MMM_DEFUN_FUNC(void,IgBrowserMethods::,doUpdate,
	       (IgTwig *twig, IgQtTwigRep *rep, unsigned mask))
{
    LOG (0, trace, LFqttwig, "  doUpdate (IgTwig[" << twig->name ()
	 << "],IgQtTwigRep," << mask << ")\n" << lat::indent);

    if (mask & (IgTwig::FLAGS_MASK | IgTwig::SELF_MASK))
    {
      rep->setCheck (IgQtTwigRep::VISIBLE_FIELD, twig->selfVisible(), true);
      rep->setCheck (IgQtTwigRep::TRAVERSE_FIELD, twig->selfTraverse(), true);
      rep->setOpen (twig->cascade ());
      // FIXME: add a folder icon
      if(twig->children() && rep->icon(0).isNull())
      {
	QIcon icon;
        icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/fileopen.xpm")), QIcon::Normal, QIcon::Off);
	rep->setIcon(0, icon);
      }      
    }

    if (mask & IgTwig::SELF_MASK)
	rep->setText (0, twig->name().c_str ());

    if (mask & IgTwig::STRUCTURE_MASK)
    {
      IgModel *model = rep->model ();
	
      if (rep->isExpanded())
      {
	    // Ensure expansion; update the children that already exist,
	    // otherwise create a representation.
	    twig->expand ();

	    for (unsigned i = 0, n = twig->children (); i < n; ++i)
	      IgRepSet::lookup (twig->child (i), model, true);
	}
    }

    LOG (0, trace, LFqttwig, lat::undent);
}
