#include "Iguana/Studio/interface/IgQtTwigBrowser.h"
#include "Iguana/Studio/interface/IgQtTwigRep.h"
#include "Iguana/Studio/interface/IgQtAppWindowService.h"
#include "Iguana/Studio/interface/IgQtSite.h"
#include "Iguana/Studio/interface/IgQtObjectMenuService.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgRepContext.h"
#include "Iguana/Framework/interface/IgSelectionService.h"
#include "Iguana/Framework/interface/IgSelectionMessage.h"
#include "Iguana/Framework/interface/IgBrowserMethods.h"
#include <QtGui>
#include <algorithm>

const char *
IgQtTwigBrowser::catalogLabel (void)
{ return "Twig"; }

IgQtTwigBrowser::IgQtTwigBrowser (IgState *state, 
				  IgSite *site)
    : QTreeWidget (IgQtSite::hostFrom (site)),
      m_state (state),
      m_discardRelease (false)
{
  ASSERT (IgSelectionService::get (m_state));
  ASSERT (IgQtObjectMenuService::get (m_state));
  IgQtSite::host (site, this);
  IgSelectionService::get (m_state)
    ->add (lat::CreateCallback (this, &IgQtTwigBrowser::selectMessage));

  header ()->setMovable(false);

  // FIXME: enable sorting, but default to off!
  //setSorting (-1, false);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  setRootIsDecorated(false);
  setSelectionMode(QAbstractItemView::SingleSelection);
  connect (this, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectionChange()));
  connect (this, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotExpand(QTreeWidgetItem *, int)));
  connect (this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(slotClick(QTreeWidgetItem *, int)));
  
  QMainWindow *mainWindow = dynamic_cast<QMainWindow *>(IgQtAppWindowService::get (m_state)->mainWindow ());
  QDockWidget *dw = new QDockWidget;    
  dw->setWidget (this);    
  mainWindow->addDockWidget (Qt::LeftDockWidgetArea, dw);

  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotContextMenu(const QPoint&)));
  
  QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
  __qtreewidgetitem->setText(3, QString::fromUtf8("Cascade"));
  __qtreewidgetitem->setText(2, QString::fromUtf8("Traverse"));
  __qtreewidgetitem->setText(1, QString::fromUtf8("Visibility"));
  __qtreewidgetitem->setText(0, QString::fromUtf8("Object"));
  setHeaderItem(__qtreewidgetitem);
  setObjectName(QString::fromUtf8("treeWidget"));
  setStyleSheet(QString::fromUtf8("QTreeView {\n"
				  "background-color: #cfdde6;\n"
				  "alternate-background-color: #cfdde6;\n"
				  "	margin: 0px;\n"
				  "   border: 0px;\n"
				  "}\n"
				  "\n"
				  " QTreeView {\n"
				  "   show-decoration-selected: 1;\n"
				  " }\n"
				  "\n"
				  " QTreeView::item {\n"
				  " }\n"
				  "\n"
				  " QTreeView::item:selected {\n"
				  "   border-top: 1px solid #567dbc;\n"
				  "   border-bottom: 1px solid #567dbc;\n"
				  "   border-left: 0px;\n"
				  "   border-right: 0px;\n"
				  " }\n"
				  "\n"
				  " QTreeView::item:selected:active{\n"
				  "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6ea1f1, stop: 1 #567dbc);\n"
				  " }\n"
				  "\n"
				  " QTreeView::item:selected:!active {\n"
				  "   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6b9be8, stop: 1 #577fbf);\n"
				  " }\n"
				  ""));
  setFrameShape(QFrame::NoFrame);
  setFrameShadow(QFrame::Plain);
  setLineWidth(0);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setAutoScroll(false);
  setProperty("showDropIndicator", QVariant(false));
  setDragEnabled(false);
  setAlternatingRowColors(false);
  setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  setIndentation(10);
  setColumnCount(2);
  header()->setVisible(false);
  header()->setCascadingSectionResizes(false);
  header()->setDefaultSectionSize(22);
  header()->setStretchLastSection(false);
  header()->resizeSection(0, 40);
  header()->setResizeMode(0, QHeaderView::Stretch);
  dw->show ();
}

void
IgQtTwigBrowser::slotContextMenu(const QPoint& pos) 
{
  if(!selectedItems().empty())
  {    
    QTreeWidgetItem *current = currentItem();
    contextMenu(current, pos);
  }
}

void
IgQtTwigBrowser::slotExpand(QTreeWidgetItem *item, int col)
{
  ASSERT (!item || dynamic_cast<IgQtTwigRep *> (item));
  if (! item)
    return;
  if(item->childCount() == 0)
  {
    IgRepresentable *object = static_cast<IgQtTwigRep *> (item)
			      ->context ()->object ();
    IgRep *rep = static_cast<IgQtTwigRep *> (item)
		 ->context ()->rep ();
    IgBrowserMethods::expand(object, rep);
    IgBrowserMethods::commit(rep, object, IgQtTwigRep::CASCADE_FIELD);
  }
}

void
IgQtTwigBrowser::slotClick(QTreeWidgetItem *item, int column)
{
  ASSERT (!item || dynamic_cast<IgQtTwigRep *> (item));
  if (! item)
    return;
  if (item && column > 0)
  {
    // Ask the rep to make the change (and commit the change back
    // to the object, and broadcast it to all other browsers).
    ASSERT (dynamic_cast<IgQtTwigRep *> (item));
    IgQtTwigRep *rep = static_cast<IgQtTwigRep *> (item);
    rep->check (column, ! rep->checked (column));
  }
}

IgQtTwigBrowser::~IgQtTwigBrowser (void)
{
    IgSelectionService::get (m_state)
	->remove (lat::CreateCallback (this, &IgQtTwigBrowser::selectMessage));
}

void
IgQtTwigBrowser::browse (IgRepresentable *object)
{
    // FIXME: This was stateless, no rep created, need to change that.
    // FIXME: broadcast selection message
    /* IgRepContext *context =*/ IgRepSet::lookup (object, this, true);
    // FIXME: model change event?
    // FIXME: select?
    // if (context)
    //     IgSelectionService::get (m_state)
    //		->broadcast (IgSelectionMessage (context));
}

void
IgQtTwigBrowser::select(IgQtTwigRep *rep)
{
  // FIXME: protect against looping from signals?
  if(! rep)
  {
    QList<QTreeWidgetItem *> selItems = selectedItems();
    for(int i = 0; i < selItems.size(); ++i)
    {
      selItems.at(i)->setSelected(false);
    }
  }
  else
  {
    if(! rep->isSelected())
      rep->setSelected(true);
    scrollToItem(rep);
  }
}

//////////////////////////////////////////////////////////////////////
void
IgQtTwigBrowser::selectMessage(IgSelectionMessage message)
{
  // FIXME: avoid re-emitting the signal?
  if(! message.context())
    select(0);
  else if(IgQtTwigRep *rep = dynamic_cast<IgQtTwigRep *>
	   (IgRepSet::lookup(message.context(), this, true)))
    // FIXME: update display
    select(rep);
}

//! Register changes in the selection list and broadcasts it on the bus.
void
IgQtTwigBrowser::onSelectionChange(void)
{
  // FIXME: broadcast a single message for multiple / single selection?
  QList<QTreeWidgetItem *> selList = selectedItems();
  for(int i = 0; i < selList.size(); ++i)
  {
    ASSERT (dynamic_cast<IgQtTwigRep *>(selList[i]));
    IgQtTwigRep *rep = static_cast<IgQtTwigRep *>(selList[i]);
    IgSelectionService::get (m_state)
      ->broadcast(IgSelectionMessage(rep->context()));
  }
}

//////////////////////////////////////////////////////////////////////
void
IgQtTwigBrowser::contextMenu(QTreeWidgetItem *item, const QPoint& pos)
{
  ASSERT (!item || dynamic_cast<IgQtTwigRep *> (item));
  if (! item)
    return;

  IgQtObjectMenuService *svc = IgQtObjectMenuService::get(m_state);
  IgRepresentable	*object = static_cast<IgQtTwigRep *>(item)
				  ->context ()->object ();

  // Pop up the menu and update it in case it was torn off.
  svc->request (object)->exec(mapToGlobal(pos));
  svc->postUpdate (object);
}
