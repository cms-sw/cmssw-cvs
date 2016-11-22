#include "Iguana/Studio/interface/IgQtTwigRep.h"
#include "Iguana/Studio/interface/IgQtTwigBrowser.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include <QApplication>
#include <QPainter>
#include <QPixmap>

// FIXME: For some reason GCC 3.2 wants these instantiated
#ifdef __GNUC__
const unsigned int   IgQtTwigRep::TRAVERSE;
const unsigned int   IgQtTwigRep::VISIBLE;
#endif // __GNUC__

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtTwigRep *
IgQtTwigRep::create (IgQtTwigBrowser *in,
		     IgQtTwigRep *parent,
		     IgQtTwigRep *before,
		     QString name, bool traverse,
		     bool visible, bool opened)
{
    if (parent && before)
	return new IgQtTwigRep (parent, before, name, traverse, visible, opened);
    else if (parent)
	return new IgQtTwigRep (parent, name, traverse, visible, opened);
    else if (before)
	return new IgQtTwigRep (in, before, name, traverse, visible, opened);
    else
	return new IgQtTwigRep (in, name, traverse, visible, opened);
}

////////////////////////////////////////////////////////////////////////////////
IgQtTwigRep::IgQtTwigRep (IgQtTwigRep *parent, IgQtTwigRep *before,
			  QString name, bool traverse, bool visible, bool opened)
    : QTreeWidgetItem (parent, before),
      m_context (0),
      m_flags (0)
{
  setText(0, name);
  init (traverse, visible, opened);
}

IgQtTwigRep::IgQtTwigRep (IgQtTwigRep *parent,
			  QString name, bool traverse, bool visible, bool opened)
    : QTreeWidgetItem (parent),
      m_context (0),
      m_flags (0)
{
  setText(0, name);
  init (traverse, visible, opened);
}

IgQtTwigRep::IgQtTwigRep (IgQtTwigBrowser *in, IgQtTwigRep *before,
			  QString name, bool traverse, bool visible, bool opened)
    : QTreeWidgetItem (in, before),
      m_context (0),
      m_flags (0)
{
  setText(0, name);
  init (traverse, visible, opened);
}

IgQtTwigRep::IgQtTwigRep (IgQtTwigBrowser *in,
			  QString name, bool traverse, bool visible, bool opened)
    : QTreeWidgetItem (in),
      m_context (0),
      m_flags (0)
{
  setText(0, name);
  init (traverse, visible, opened);
}

void
IgQtTwigRep::init (bool traverse, bool visible, bool opened)
{
  if(! parent ())
  {  
    QFont itemFont = font(0);
    itemFont.setBold(true);
    setFont(0, itemFont);
    bold(true);
  }
  else
    setForeground(0, parent()->foreground(0));
  
    setCheck (TRAVERSE_FIELD, traverse, false);
    setCheck (VISIBLE_FIELD,  visible, false);
    setExpanded(opened);
    flag (EXPANDED, opened);
    
    // Delay until we get a context
    flag (AUTO_OPEN, opened);
}

IgQtTwigRep::~IgQtTwigRep (void)
{
    // The destruction may come upon me in two ways: the object is
    // going away and the context is deleting me, or someone else
    // killed me (e.g. Qt).  In the former case the caller will have
    // done the right thing of getting rid of references to me.
    // However if my context *was* deleted, I must not delete it
    // again; if so, it has already set its rep to null, which we use
    // as a clue.

    ASSERT (m_context);
    if (m_context->rep ())
    {
	ASSERT (m_context->rep () == this);
	m_context->erase (false);
	delete m_context;
    }
}

void
IgQtTwigRep::context (IgRepContext *context)
{
    ASSERT (! m_context);
    ASSERT (context);
    m_context = context;

    // OK, it is now safe to open up this one
    if (flag (AUTO_OPEN))
	setOpen (true);
}

//////////////////////////////////////////////////////////////////////
IgRepContext *
IgQtTwigRep::context (void) const
{ return m_context; }

IgModel *
IgQtTwigRep::model (void) const
{ return static_cast<IgQtTwigBrowser *> (treeWidget()); }

//////////////////////////////////////////////////////////////////////
void
IgQtTwigRep::check (int column, bool value /* = true */)
{
    // Update the list view and commit the change back to the object,
    // but only if we are changing anything.
    if (value != checked (column))
    {
	setCheck (column, value, true);
	IgBrowserMethods::commit (this, m_context->object (), column);
    }
}

void
IgQtTwigRep::setOpen (bool opened)
{
  if (opened && ! flag (EXPANDED))
  {
    IgBrowserMethods::expand (m_context->object (), this);
    setExpanded(opened);
    setCheck (TRAVERSE_FIELD, flag (TRAVERSE), true);
  }

  if (opened != flag (EXPANDED))
    flag (EXPANDED, opened);
    
  setExpanded(opened);
  flag(CASCADE_FIELD, opened);
  IgBrowserMethods::commit (this, m_context->object (), CASCADE_FIELD);
}

//////////////////////////////////////////////////////////////////////
bool
IgQtTwigRep::traversed (void) const
{
    IgQtTwigRep *p = static_cast<IgQtTwigRep *> (parent ());
    for (; p; p = static_cast<IgQtTwigRep *> (p->parent ()))
	if (! p->flag (TRAVERSE))
	    return false;

    return true;
}
    
void
IgQtTwigRep::setCheck (int column, bool value, bool /* known */)
{
  switch (column)
  {
  case TRAVERSE_FIELD:
    flag(TRAVERSE, value);
    setExpanded(value);
    // FIXME:
    setCheckState(column, value ? Qt::Checked : Qt::Unchecked);
    //flag(EXPANDED, value);
    flag(EXPANDED, isExpanded());
    break;

  case VISIBLE_FIELD:
    flag(VISIBLE, value);
    setCheckState(column, value ? Qt::Checked : Qt::Unchecked);
    break;

  default:
    ASSERT (false);
  }
  setCheckState(3, flag(EXPANDED) ? Qt::Checked : Qt::Unchecked);
  if(!traversed())
    setForeground(0, Qt::gray);
  else
    setForeground(0, Qt::black);
}

QColorGroup
IgQtTwigRep::columnColor (int column, const QColorGroup &cg) const
{
    QColorGroup cgnew (cg);
    if (column == 0 && !traversed ())
	cgnew.setColor (QColorGroup::Text, cg.mid ());
    return cgnew;
}
