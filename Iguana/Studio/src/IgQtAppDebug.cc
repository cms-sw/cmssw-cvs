#include "Iguana/Studio/interface/IgQtAppDebug.h"
#include "Iguana/Studio/interface/IgQtAppDebugService.h"
#include "Iguana/Studio/interface/IgQtAppMenuService.h"
#include "Iguana/Framework/interface/IgPluginLoader.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgPluginManager.h"
#include "Iguana/Framework/interface/IgModule.h"
#include "Iguana/Framework/interface/IgModuleCache.h"
#include "Iguana/Framework/interface/IgModuleDescriptor.h"
#include <classlib/utils/ResourceInfo.h>
#include <classlib/utils/ProcessInfo.h>
#include <classlib/utils/SharedLibrary.h>
#include <classlib/utils/Signal.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/sysapi/Filename.h>
#include <QMenu>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QFile>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include <QPixmap>
#include <QHideEvent>
#include <QVBoxLayout>
#include <stdio.h>
#include <set>
#include <cstddef>
#include <cstdlib>

IgQtAppDebugInfo::IgQtAppDebugInfo (QWidget *parent /* = 0 */,
				    bool autoRefresh /* = true */)
    : QDialog (parent),
      m_autoRefresh (autoRefresh),
      m_data (0),
      m_timer (0)
{
    m_data		 = new QTreeWidget (this);
    QWidget	*buttons = new QWidget;
    QHBoxLayout	*hbox	 = new QHBoxLayout;
    QPushButton	*ok	 = new QPushButton ("Close", buttons);
    QPushButton	*refresh = new QPushButton ("Refresh", buttons);
    QPushButton *save    = new QPushButton ("Save List", buttons);
    hbox->addWidget(ok);
    hbox->addWidget(refresh);
    hbox->addWidget(save);
    buttons->setLayout(hbox);
    
    QVBoxLayout	*vbox	 = new QVBoxLayout (this);

    vbox->addWidget (m_data);
    vbox->addWidget (buttons);

    connect (ok, SIGNAL(clicked ()), this, SLOT(hide()));
    connect (refresh, SIGNAL(clicked ()), this, SLOT(update()));
    connect (save, SIGNAL(clicked ()), this, SLOT(exportList ()));
}

IgQtAppDebugInfo::~IgQtAppDebugInfo (void)
{
    // FIXME: Does qt do these automatically?
    delete m_timer;
}

void
IgQtAppDebugInfo::info (void)
{
    update ();
    show ();
    start ();
}

void
IgQtAppDebugInfo::exportList (void)
{
    if(m_data->topLevelItemCount() == 0)
        return;

    QFileDialog* fileDialog = new QFileDialog(this, "File Dialog");
    fileDialog->setMode(QFileDialog::AnyFile);
  
    QString fileName;
    if (fileDialog->exec () == QDialog::Accepted) 
    {
        fileName = fileDialog->selectedFile ();
    
	QFile file (fileName);
	if (!file.open (QIODevice::WriteOnly))
	    return;

	QTextStream textStream(&file);
	iterateTreeWidget(textStream);
	file.close();
    }
}

void
IgQtAppDebugInfo::iterateTreeWidget(QTextStream &out, QTreeWidgetItem *parent) 
{
  int count = 
    parent ? parent->childCount() : m_data->topLevelItemCount();
  
  out << " ";
  
  for(int i = 0; i < count; i++)
  {
    QTreeWidgetItem *item = 
      parent ? parent->child(i) : m_data->topLevelItem(i);
	
    out << item->text(0) << (item->icon(0).isNull() ? "" : "*") << "\t";

    iterateTreeWidget(out, item);
  }
  out << "\n";
}

void
IgQtAppDebugInfo::hideEvent (QHideEvent* event) 
{
    delete m_timer;
    m_timer = 0;
    QWidget::hideEvent (event);
}

void
IgQtAppDebugInfo::start (void)
{
    if (m_autoRefresh)
    {
	// Create the update timer
	if (! m_timer)
	{
	    m_timer = new QTimer (this);
	    connect (m_timer, SIGNAL(timeout()), this, SLOT(update()));
	}
	m_timer->start (5000);
    }
}

QTreeWidget *
IgQtAppDebugInfo::list (void)
{
    return m_data;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtAppDebugResourceInfo::IgQtAppDebugResourceInfo (QWidget *parent)
    : IgQtAppDebugInfo (parent)
{
  QStringList headers;
  headers << "Resource" << "Value" << "Units";
  list ()->setHeaderLabels(headers);

  setCaption ("Program Resource Consumption");
  resize (320, 580);
}

void
IgQtAppDebugResourceInfo::update (void)
{
    QTreeWidget *props = list ();
    ASSERT (props);

    lat::ResourceInfo::Status		stats (lat::ResourceInfo::info_self());
    lat::ResourceInfo::Status::iterator i = stats.begin ();
    char				buf [512];

    props->clear ();
    for ( ; i != stats.end (); ++i)
    {
      QStringList values;
      values << lat::ResourceInfo::name (i->first)
	     <<	lat::ResourceInfo::format (i->first, i->second, buf, sizeof (buf))
	     << lat::ResourceInfo::unit (i->first);
      props->addTopLevelItem(new QTreeWidgetItem (values));
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtAppDebugLibrariesInfo::IgQtAppDebugLibrariesInfo (QWidget *parent)
    : IgQtAppDebugInfo (parent, false)
{
  QStringList headers;
  headers << "Address" << "Component" << "File Name";
  list ()->setHeaderLabels(headers);

  setCaption ("Shared Libraries Currently Loaded");
  resize (600, 200);
}

static void
LibraryEnumerator (const lat::SharedLibrary::LibraryInfo &info, QTreeWidget *view)
{
    char		buf [40];  // ample for 128+ bits.
    lat::Filename	fname (info.m_filename);

    sprintf (buf, "0x%08lX", info.m_text_start);
    QStringList values;
    values << buf << (const char *) fname.nondirectory() << (const char *) fname;
    view->addTopLevelItem(new QTreeWidgetItem(values));
}

void
IgQtAppDebugLibrariesInfo::update (void)
{
    QTreeWidget *props = list ();
    ASSERT (props);

    props->clear ();
    lat::SharedLibrary::loaded (lat::CreateCallback (&LibraryEnumerator, props));
    props->sortByColumn(0, Qt::AscendingOrder);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtAppDumpStateInfo::IgQtAppDumpStateInfo (IgState *state,
					    QWidget *parent)
    : IgQtAppDebugInfo (parent, false),
      m_state (state->root ())
{
  QStringList headers;
  headers << "States";
  list ()->setHeaderLabels(headers);

  setCaption ("IGUANA State tree");
  resize (600, 200);
}

void
IgQtAppDumpStateInfo::update (void)
{
    QTreeWidget *props = list ();
    ASSERT (props);

    props->clear ();
    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList("State"));
    props->addTopLevelItem(item);
    populate (m_state, item);
}

void
IgQtAppDumpStateInfo::populate (IgState* state, QTreeWidgetItem *item)
{
    item->setExpanded(true);
    
    QTreeWidgetItem* element = new QTreeWidgetItem(QStringList("Elements"));
    item->addChild(element);
    bool hasElements = false;
    for(unsigned int i=0; i<state->elements(); ++i)
    {
        if(state->element(i))
	{
	  element->addChild(new QTreeWidgetItem(QStringList(state->element (i)->stateKey().label ().c_str())));
	  hasElements = true;
	}
    }
    if (!hasElements)
        delete element;
    else
        element->setExpanded(true);
	
    if (state->children())
      for (unsigned int i=0; i<state->children(); ++i)
      {
	QTreeWidgetItem* eItem = new QTreeWidgetItem(QStringList("State"));
	item->addChild(eItem);
	populate (state->child(i), eItem);
      }
    
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
IgQtAppDebugPluginsInfo::IgQtAppDebugPluginsInfo (IgState *state,
						  QWidget *parent)
    : IgQtAppDebugInfo (parent, false),
      m_state (state)
{
  QStringList headers;
  headers << "Type" << "Plug-in" << "Attached";
  list ()->setHeaderLabels(headers);

  setCaption ("Available Plugins");
  resize (600, 200);
}

template <class I>
void
FillPlugins (I begin, I end, QTreeWidgetItem *parent)
{
}

void
IgQtAppDebugPluginsInfo::update (void)
{
    static const char	*check [] = {
	"    16    13        2            1",
	"  c none",
	"x c #000000",
	"                ",
	"               x",
	"             xx ",
	"            x   ",
	"  xx      xx    ",
	" xxxx    xx     ",
	"  xxxx  xx      ",
	"   xxxxxx       ",
	"    xxxxx       ",
	"     xxx        ",
	"      xx        ",
	"                ",
	"                "
    };
    static QPixmap	*checkPixmap = new QPixmap (check);

    // FIXME: get state from constructor?
    QTreeWidget	*props = list ();

    ASSERT (m_state);
    ASSERT (props);

    props->clear ();
    
    typedef std::pair<std::string,std::string> Seen;
    IgPluginManager			*db = IgPluginManager::get ();
    IgPluginManager::DirectoryIterator	dir;
    IgModuleCache::Iterator		plugin;
    IgModuleDescriptor			*cache;
    std::set<Seen>			seen;
    unsigned				i;

    for (dir = db->beginDirectories (); dir != db->endDirectories (); ++dir)
	for (plugin = (*dir)->begin (); plugin != (*dir)->end (); ++plugin)
	    for (cache=(*plugin)->cacheRoot(), i=0; i < cache->children(); ++i)
	    {
		IgModuleDescriptor *d = cache->child (i);

		// Get category
		const char *catname = d->token (0).c_str ();
		QList<QTreeWidgetItem *>cats = props->findItems(catname, Qt::MatchExactly);
		QTreeWidgetItem *cat;
		if(cats.empty ())
		{
		  cat = new QTreeWidgetItem(props, QStringList(catname));
		  cat->setExpanded(true);
		}
		else
		  cat = cats.at(0);

		// Get last item
		QTreeWidgetItem *prev = cat ? cat->child(0) : 0;
		int i = 0;
		while(prev && prev->child(++i))
		    prev = prev->child(i);

		const char *name = d->token (1).c_str ();
		const char *lib = (*plugin)->libraryName ();
		if (! *name)
		    name = "(Built-in)";
		QStringList values;
		values << name << lib;
		
		if (seen.count (Seen (catname, name)))
		    continue;
		seen.insert (Seen (catname, name));

		prev = new QTreeWidgetItem(cat, values);

		if ((*plugin)->attached ())
		    prev->setIcon(2, *checkPixmap);
	    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool IgQtAppDebug::m_profiler =  false;

IgQtAppDebug::IgQtAppDebug (IgState *state, QWidget *parent /* = 0 */)
    : m_state (state),
      m_parent (parent)
{
    m_info [0] = m_info [1] = m_info [2] = m_info [3] = 0; 
}

IgQtAppDebug::~IgQtAppDebug (void)
{
    // FIXME: Does qt do these automatically?
    delete m_info [3];
    delete m_info [2];
    delete m_info [1];
    delete m_info [0];
}

void
IgQtAppDebug::resources (void)
{
    if (! m_info [0])
	m_info [0] = new IgQtAppDebugResourceInfo (m_parent);

    m_info [0]->info ();
}

void
IgQtAppDebug::libraries (void)
{
    if (! m_info [1])
	m_info [1] = new IgQtAppDebugLibrariesInfo (m_parent);

    m_info [1]->info ();
}

void
IgQtAppDebug::plugins (void)
{
    if (! m_info [2])
	m_info [2] = new IgQtAppDebugPluginsInfo (m_state, m_parent);

    m_info [2]->info ();
}

void
IgQtAppDebug::dumpState (void)
{
    if (! m_info [3])
	m_info [3] = new IgQtAppDumpStateInfo (m_state, m_parent);

    m_info [3]->info ();
}

void
IgQtAppDebug::debugme (void)
{
    // Install a single-shot timer to start the debugger once the
    // event queue has been drained and the menu popped down.
    QTimer::singleShot (0, this, SLOT(rundebug()));
}

void
IgQtAppDebug::rundebug (void)
{
    // FIXME: Better logic to figure out best debugger to use.

    std::string programName = lat::ProcessInfo::fullProgramName ();

    char buf [512]; // must reserve space for entire program name+pid

    sprintf (buf, "ddd %s %s %ld &",
#ifdef __SUNPRO_CC
	     "--dbx",
#else
	     "--gdb",
#endif
	     programName.c_str (),
	     (long) lat::ProcessInfo::pid ());
    
    system (buf);
}

void
IgQtAppDebug::showProfiler (void)
{
    if (!m_profiler)
    {
      //Now load the Oprofile plugin.
      IgExtensionDB::Iterator it = IgExtensionDB::get ()->locate ("Extensions/Profiler/OProfile");
      if (*it != 0)
      {
        m_profiler = true;
        QMenu *profile = IgQtAppMenuService::get(m_state)->subMenu(IgQtAppDebugService::MENU_ID_DEBUG);
        profile->disconnectItem (IgQtAppDebugService::MENU_ID_DEBUG_PROFILER, this, SLOT(showProfiler()));
          
	ASSERT (IgPluginLoader::get (m_state));
        IgPluginLoader::get (m_state)->load (m_state, "Extensions/Profiler/OProfile", true);
      }
      else
        QMessageBox::critical (0, "IGUANA", "You do not seem to have"
                               "\"Extensions/Profiler/OProfile\" plugin installed.");
   }
}
