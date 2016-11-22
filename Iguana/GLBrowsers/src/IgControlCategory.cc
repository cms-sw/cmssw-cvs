#include "Iguana/GLBrowsers/interface/IgControlCategory.h"
#include "Iguana/GLBrowsers/interface/IgControlCentre.h"
#include "Iguana/GLBrowsers/interface/IgStudioCentre.h"
#include "Iguana/Studio/interface/IgQtAppContextService.h"
#include "Iguana/Studio/interface/IgQtCallbacks.h"
#include "Iguana/Framework/interface/IgBrowser.h"
#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/Studio/interface/IgQtScrollView.h"
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QToolButton>
#include <QLayout>
#include <QLabel>
#include <QInputDialog>
#include <QPoint>
#include <QSplitter>
#include <QLayout>
#include <QFrame>
#include <QPixmap>
#include <assert.h>

IgControlCategory::IgControlCategory(IgState *state,
				     const QString& name, int flags /* = 0 */)
  : m_state           (state),
    m_panel	        (0),
    m_leftArea        (0),
    m_rightArea       (0),
    m_bodyArea        (0),
    m_rightScrollArea (0),
    m_topLayout       (0),
    m_bodyLayout      (0),
    m_bottomLayout    (0),
    m_tracker         (0),
    m_items           (0),
    m_newItem	        (0),
    m_deleteItem      (0),
    m_current         (-1),
    m_indentLevel     (0),
    m_flags 	        (flags),
    m_helpURL ("")
{
  assert(! (flags & ~(MultiItem | ManageItems | MonitorChanges | SplitterArea)));
  setName (name);
  clearCategory ();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgControlCategory::clearCategory (void)
{
  QString name = this->name ();
  IgControlCentre *centre = IgStudioCentre::get (m_state)->controlCentre ();
    
  if(m_panel && centre)
    centre->removeCategory (this);
      
  if (m_flags & SplitterArea)
    m_panel = new QSplitter ();
  else
  {
    QGroupBox *panel = new QGroupBox (name);
    panel->setFlat (true);
    panel->setTitle (QString::null);
    m_panel = panel;
  }
    
  m_panel->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_panel->setLayout(new QHBoxLayout);
    
  clearLeftArea ();
  clearBodyArea ();

  m_panel->setEnabled (true);
  m_panel->hide ();

  if (centre)
    centre->addCategory (this);
}

void
IgControlCategory::clearLeftArea (void)
{
  if (m_leftArea)
  {
    panelLayout ()->remove (m_leftArea);
    m_leftArea->reparent (0, QPoint(0,0));
    delete m_leftArea->layout ();
    delete m_leftArea;
  }
  m_leftArea = new QWidget (m_panel);
  m_leftArea->setLayout(new QVBoxLayout);
    
  if (!(m_flags & SplitterArea))
    panelLayout ()->addWidget (m_leftArea);
}

void
IgControlCategory::clearBodyArea (void)
{
  // Fill the group box with two areas: the top area (a layout), and
  // the body (a widget).  The children of body are disabled when
  // the category is non-editable, those in the top are not (the
  // latter are direct children of this widget).
  //
  // The body area has two layout children: the body (a grid) and
  // bottom (a vbox).  The former is filled by subclasses with the
  // items, non-grid-aligned items like our tracker goes in the
  // latter.  Subclasses can insert items before the tracker but
  // under the grid layout with QBoxLayout::insertWidget(0, ...).
  // (We don't put the tracker into the grid since grids don't
  // provide "insert" facility: it would be too inconvenient for
  // subclasses to add their items before the tracker if did so.)
  //
  // All subclass widgets should be created with the appropriate
  // xxxArea()/xxxLayout().  We arrange those methods to return
  // the right thing: topArea() == m_panel, bottomArea() == body.
  //m_panel->setFlat(true);

  int flags = m_flags;
  if(m_rightArea){
    if (flags & MultiItem)
    {
      disconnect (m_items,    SIGNAL (activated    (int)), 
		  this,       SLOT   (itemChanged  (int)));
      disconnect (this,       SIGNAL (empty        (bool)), 
		  m_rightScrollArea, SLOT   (setDisabled  (bool)));
      disconnect (m_items,      SIGNAL (textChanged (const QString &)),
		  this,         SLOT   (renameItem  (const QString &)));
      m_items->reparent (0, QPoint(0,0));
      delete m_items;
	
      if (flags & ManageItems)
      {
	disconnect (m_newItem,    SIGNAL (clicked ()), 
		    this,         SLOT   (createNewItem ()));
	disconnect (m_deleteItem, SIGNAL (clicked ()), 
		    this,         SLOT   (deleteItem ()));
	disconnect (this,         SIGNAL (empty        (bool)), 
		    m_deleteItem, SLOT   (setDisabled  (bool)));
          
	m_newItem->reparent (0, QPoint(0,0));
	m_deleteItem->reparent (0, QPoint(0,0));
	delete m_newItem;
	delete m_deleteItem;
      }
    }
    if (flags & MonitorChanges)
    {
      disconnect (m_tracker,SIGNAL(toggled(bool)), this,SLOT(track(bool)));
      m_tracker->reparent (0, QPoint(0,0));
      delete m_tracker;
    }
    disconnect (this, SIGNAL(editable(bool)), m_rightScrollArea, SLOT(setEnabled(bool)));
      
    m_rightArea ->reparent (0, QPoint(0,0));
    m_rightScrollArea->reparent (0, QPoint(0,0));
    m_bodyArea->reparent (0, QPoint(0,0));
    panelLayout ()->remove (m_rightArea);
    delete m_bottomLayout;
    delete m_bodyLayout;
    delete m_bodyArea;
    delete m_topLayout;
    delete m_rightScrollArea;
    delete m_rightArea;
  }
        
  m_rightArea = new QWidget (m_panel);
  if (!(m_flags & SplitterArea))
    panelLayout ()->addWidget (m_rightArea);
  m_rightArea->setLayout(new QVBoxLayout);
  m_rightScrollArea = new IgQtScrollView (m_rightArea, "m_rightScrollArea");
  m_rightScrollArea->setFrameStyle(QFrame::Box | QFrame::Plain);
  m_rightScrollArea->setLayout(new QGridLayout);
  QGridLayout *fullBodyLayout1 = 
    new QGridLayout (m_rightScrollArea->viewport (), 1, 1);
    
  m_bodyArea = new QWidget(m_rightScrollArea->viewport());
  m_rightScrollArea->setWidget(m_bodyArea);
  fullBodyLayout1->addWidget (m_bodyArea, 0, 0);
    
  m_topLayout = new QVBoxLayout (rightLayout ());
  rightLayout ()->addWidget (m_rightScrollArea);

  QVBoxLayout *fullBodyLayout = new QVBoxLayout (m_bodyArea, 5);
  m_bodyLayout = new QGridLayout (fullBodyLayout);
  m_bottomLayout = new QVBoxLayout (fullBodyLayout);
  if (flags & MultiItem)
  {
    QHBoxLayout *topRow = new QHBoxLayout (topLayout ());
    topRow->add (m_items = new QComboBox (topArea ()));
    m_items->setEditable(true);
    connect (m_items,    SIGNAL (activated    (int)), 
	     this,       SLOT   (itemChanged  (int)));
    connect (this,       SIGNAL (empty        (bool)), 
	     m_rightScrollArea, SLOT   (setDisabled  (bool)));
    connect (m_items,      SIGNAL (textChanged (const QString &)),
	     this,         SLOT   (renameItem  (const QString &)));

    if (flags & ManageItems)
    {
      // Add buttons to add or remove items
      m_newItem = new QToolButton (topArea ());
      m_newItem->setText ("+");
      m_newItem->setAutoRaise (true);
      m_newItem->setFixedSize (20, 20);

      m_deleteItem = new QToolButton (topArea ());
      m_deleteItem->setText ("x");
      m_deleteItem->setAutoRaise (true);
      m_deleteItem->setFixedSize (20, 20);

      topRow->add (m_newItem);
      topRow->add (m_deleteItem);
	    
      connect (m_newItem,    SIGNAL (clicked ()), 
	       this,         SLOT   (createNewItem ()));
      connect (m_deleteItem, SIGNAL (clicked ()), 
	       this,         SLOT   (deleteItem ()));
      connect (this,         SIGNAL (empty        (bool)), 
	       m_deleteItem, SLOT   (setDisabled  (bool)));
    }
  }

  if (flags & MonitorChanges)
  {
    QHBoxLayout *trackrow = new QHBoxLayout (bottomLayout ());
    trackrow->add (m_tracker = new QCheckBox ("Track changes", bottomArea ()));
    trackrow->addItem (new QSpacerItem (20, 20, QSizePolicy::Expanding));
    m_tracker->setChecked (true);
    connect (m_tracker,SIGNAL(toggled(bool)), this,SLOT(track(bool)));
  }
  connect (this, SIGNAL(editable(bool)), m_rightScrollArea, SLOT(setEnabled(bool)));
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
int
IgControlCategory::items (void) const
{ return m_items ? m_items->count () : 0; }

int
IgControlCategory::currentItem (void) const
{ return items () ? m_items->currentItem () : -1; }

const QString
IgControlCategory::currentText (void) const
{ return m_items ? m_items->currentText () : ""; }

void
IgControlCategory::setCurrentItem (int index)
{
  assert(m_items);
  assert((size_t) m_items->count () == m_editable.size ());
  assert(index == -1 || (index >= 0 && index < m_items->count ()));
  m_items->setCurrentItem (index);
}

void
IgControlCategory::addItem (QString label, bool editable /* = true */)
{
  assert(m_items);
  assert((size_t) m_items->count () == m_editable.size ());
  assert(! label.isEmpty ());
  m_items->insertItem (label);
  m_editable.push_back (editable);    
  if (m_items->count () == 1)
  {
    setCurrentItem (0);
    emit empty (false);
  }
}

void
IgControlCategory::removeItem (int index)
{
  assert(m_items);
  assert((size_t) m_items->count () == m_editable.size ());
  assert(index >= 0 && index < m_items->count ());
  int selectedIndex = currentItem ();

  disconnect (m_items,      SIGNAL (textChanged (const QString &)),
	      this,         SLOT   (renameItem  (const QString &)));
  m_items->removeItem (index);
  connect (m_items,      SIGNAL (textChanged (const QString &)),
	   this,         SLOT   (renameItem  (const QString &)));

  m_editable.erase (m_editable.begin () + index);
  if( index == selectedIndex )
  {      
    if (currentItem () == -1 || index >= m_items->count ())
    {
      if ( m_items->count () > 0 )
      {
	m_items->setCurrentItem (index-1);
	itemChanged (currentItem ());
      }
      else
      {
	emit empty (true);
	emit clean ();
      }
    }
    else
      itemChanged (currentItem ());
  }
}

void
IgControlCategory::createNewItem (void)
{
  // Base class must override: create a new item into the list
  assert(false);
}

void
IgControlCategory::deleteItem (void)
{
  // Base class must override; call removeItem and then delete it.
  assert(false);
}

void
IgControlCategory::renameItem(const QString &newName)
{ 
  bool	ok = false;
  QString	name (QInputDialog::getText (tr("Rename Item"), QString::null,
					     QLineEdit::Normal,
					     newName,
					     &ok, panel ()));
  if (! ok || name.isEmpty ())
    return;
    
  disconnect (m_items,      SIGNAL (textChanged (const QString &)),
	      this,         SLOT   (renameItem  (const QString &)));
  m_items->changeItem (name, currentItem ());
  connect (m_items,      SIGNAL (textChanged (const QString &)),
	   this,         SLOT   (renameItem  (const QString &)));
}

bool
IgControlCategory::isItemEditable (int index) const
{
  assert(index == -1 || (index >= 0 && (size_t) index < m_editable.size ()));
  return index == -1 ? false : m_editable [index];
}

void
IgControlCategory::removeableItem (bool option)
{ 
  if (m_deleteItem && currentItem() >= 0)
    m_deleteItem->setEnabled (option);
}

void
IgControlCategory::setItemEditable (int index, bool editable /* = true */)
{
  assert(index == -1 || (index >= 0 && (size_t) index < m_editable.size ()));
  m_editable [index] = editable;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgControlCategory::setDirty (bool value)
{
  if (! m_tracker || ! m_tracker->isChecked ())
    IgControlItem::setDirty (value);
}

void
IgControlCategory::track (bool value)
{
  if (value)
    IgControlItem::setDirty (false);
  emit tracking (value);
}

bool
IgControlCategory::isTracking (void) const
{ return m_tracker && m_tracker->isChecked (); }

void
IgControlCategory::setTracking (bool value)
{
  if (m_tracker)
    m_tracker->setChecked (value);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
QWidget *
IgControlCategory::panel (void)
{ return m_panel; }

QHBoxLayout *
IgControlCategory::panelLayout (void)
{ return dynamic_cast<QHBoxLayout*>(m_panel->layout ()); }

QWidget *
IgControlCategory::bodyArea (void)
{ return m_bodyArea; }

QGridLayout *
IgControlCategory::bodyLayout (void)
{ return m_bodyLayout; }

QWidget *
IgControlCategory::bottomArea (void)
{ return m_bodyArea; }

QVBoxLayout *
IgControlCategory::bottomLayout (void)
{ return m_bottomLayout; }

QWidget *
IgControlCategory::topArea (void)
{ return m_rightArea; }

QVBoxLayout *
IgControlCategory::topLayout (void)
{ return m_topLayout; }

QWidget *
IgControlCategory::leftArea (void)
{ return m_leftArea; }

QVBoxLayout *
IgControlCategory::leftLayout (void)
{ return dynamic_cast<QVBoxLayout*>(m_leftArea->layout ()); }

QWidget *
IgControlCategory::rightArea (void)
{ return m_rightArea; }

QVBoxLayout *
IgControlCategory::rightLayout (void)
{ return dynamic_cast<QVBoxLayout*>(m_rightArea->layout ()); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
QWidget *
IgControlCategory::makeIndentedLabel (QString label, QWidget *area)
{
  QWidget *hbox = new QWidget(area);
  QHBoxLayout *hlayout = new QHBoxLayout;
  hlayout->addItem(new QSpacerItem (20 * m_indentLevel, 0, QSizePolicy::Fixed));
  hlayout->addWidget(new QLabel(label, hbox));
  hbox->setLayout(hlayout);
  return hbox;
}

void
IgControlCategory::pushIndent (void)
{ assert(m_indentLevel >= 0); m_indentLevel++; } 

void
IgControlCategory::popIndent (void)
{ assert(m_indentLevel); m_indentLevel--; } 

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgControlCategory::itemChanged (int /* newItem */)
{
  assert( !m_items || (size_t) m_items->count () == m_editable.size ());
  detach ();
  attach ();
  update ();
  setEditable (! m_items || isItemEditable (currentItem ()));
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
IgControlCategory::attach (void)
{}

void
IgControlCategory::detach (void)
{}

void
IgControlCategory::update (void)
{}

void
IgControlCategory::apply (void)
{}

void
IgControlCategory::revert (void)
{ update (); }

void
IgControlCategory::setShown (bool show)
{
  if (show)
    this->show ();
  else
    this->hide ();
}

void
IgControlCategory::show (void)
{
  if(m_items && m_items->count () == 0)
  {
    emit empty (true);
    return;
  }
  itemChanged (0);
  m_panel->show ();
  m_rightArea->show ();
  m_rightScrollArea->show ();
  m_bodyArea->show ();
  track (isTracking ());
  emit clean ();
}

void
IgControlCategory::hide (void)
{
  m_panel->hide ();
  detach ();
}

void
IgControlCategory::focusIn (void)
{ }

void
IgControlCategory::focusOut (void)
{ IgStudioCentre::get (state ())->controlCentre ()->updateOnWindowActive (); }

void
enableCategory (void *data, bool focus)
{
  IgControlCategory *self=static_cast<IgControlCategory *>(data);
  IgControlCentre *centre = IgStudioCentre::get (self->state ())->controlCentre ();
  centre->enableCategory (self, focus);
}

void
IgControlCategory::registerMe (IgState *state)
{
  IgQtAppContextService *cs = IgQtAppContextService::get (state);
    
  cs->add (&enableCategory, this);
}

const QString&
IgControlCategory::getHelpURL (void) const
{ return m_helpURL; }

void
IgControlCategory::setHelpURL (const QString &url)
{ m_helpURL = url; }

const QPixmap&
IgControlCategory::getIcon (void) const
{ return m_icon; }

void
IgControlCategory::setIcon (const QPixmap &icon)
{ m_icon = icon; }

void
IgControlCategory::hideBodyScrollArea (void) const
{ m_rightScrollArea->hide (); }

void
IgControlCategory::showBodyScrollArea (void) const
{ m_rightScrollArea->show (); }

