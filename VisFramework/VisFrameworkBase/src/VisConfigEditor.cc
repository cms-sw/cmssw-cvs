//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisConfigEditor.h"
#include "VisFramework/VisFrameworkBase/interface/VisConfiguration.h"
#include "VisFramework/VisFrameworkBase/interface/VisParamEditor.h"
#include <qlineedit.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <iostream>
#include <map>
#include <string>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisConfigEditor::VisConfigEditor (IgState *state, QWidget* parent,  const char* name, Qt::WFlags fl)
    : QWidget (parent, name, fl),
      m_state (state)
{
    if (!name)
	setName ("VisConfigEditor");
    resize (613, 534); 
    setCaption (trUtf8 ("Configurable Parameters"));
    configEditorLayout = new Q3VBoxLayout (this, 11, 6, "configEditorLayout"); 

    layout2 = new Q3VBoxLayout (0, 0, 6, "layout2"); 

    configListView = new Q3ListView (this, "configListView");
    configListView->addColumn (trUtf8 ("Configurable Parameter Editor"));
    configListView->addColumn (trUtf8 ("Value"));
    layout2->addWidget (configListView);

    layout1 = new Q3HBoxLayout (0, 0, 6, "layout1"); 

    helpBtn = new QPushButton (this, "helpBtn");
    helpBtn->setText (trUtf8 ("Help"));
    QToolTip::add (helpBtn, trUtf8 ("On-line Help", "Help on configurable parameters"));
    Q3WhatsThis::add (helpBtn, trUtf8 ("Help on configurable parameters.", "Help on configurable parameters."));
    layout1->addWidget (helpBtn);

    newBtn = new QPushButton (this, "newBtn");
    newBtn->setEnabled (false);
    newBtn->setText (trUtf8 ("New"));
    QToolTip::add (newBtn, trUtf8 ("Add New Parameter"));
    Q3WhatsThis::add (newBtn, trUtf8 ("Add new configurable parameter\n"
				     "and set its value"));
    layout1->addWidget (newBtn);

    editBtn = new QPushButton (this, "editBtn");
    editBtn->setEnabled (false);
    editBtn->setText (trUtf8 ("Edit"));
    QToolTip::add (editBtn, trUtf8 ("Modify Value"));
    Q3WhatsThis::add (editBtn, trUtf8 ("Modify configurable parameter value"));
    layout1->addWidget (editBtn);

    refreshBtn = new QPushButton (this, "refreshBtn");
    refreshBtn->setEnabled (true);
    refreshBtn->setText (trUtf8 ("Refresh"));
    QToolTip::add (refreshBtn, trUtf8 ("Refresh Parameter List"));
    Q3WhatsThis::add (refreshBtn, trUtf8 ("Re-read the parameters list and update it on the screen"));
    layout1->addWidget (refreshBtn);

    okBtn = new QPushButton (this, "okBtn");
    okBtn->setText (trUtf8 ("OK"));
    okBtn->setOn (false);
    okBtn->setDefault (true);
    QToolTip::add (okBtn, trUtf8 ("Finish Editor Session"));
    layout1->addWidget (okBtn);

    cancelBtn = new QPushButton (this, "cancelBtn");
    cancelBtn->setText (trUtf8 ("Cancel"));
    QToolTip::add (cancelBtn, trUtf8 ("Terminate Editor Session"));
    layout1->addWidget (cancelBtn);
    layout2->addLayout (layout1);
    configEditorLayout->addLayout (layout2);

    // signals and slots connections
    connect (helpBtn, SIGNAL (clicked ()), this, SLOT (helpBtn_clicked ()));
    connect (editBtn, SIGNAL (clicked ()), this, SLOT (editBtn_clicked ()));
    connect (okBtn, SIGNAL (clicked ()), this, SLOT (okBtn_clicked ()));
    connect (cancelBtn, SIGNAL (clicked ()), this, SLOT (cancelBtn_clicked ()));
    connect (configListView, SIGNAL (doubleClicked (Q3ListViewItem*)), this, SLOT (configListView_doubleClicked (Q3ListViewItem*)));
    connect (configListView, SIGNAL (selectionChanged ()), this, SLOT (configListView_selectionChanged ()));
    connect (configListView, SIGNAL (selectionChanged (Q3ListViewItem*)), this, SLOT (configListView_selectionChanged (Q3ListViewItem*)));
    connect (refreshBtn, SIGNAL (clicked ()), this, SLOT (refreshBtn_clicked ()));
    connect (newBtn, SIGNAL (clicked ()), this, SLOT (newBtn_clicked ()));
}

/*  
 *  Destroys the object and frees any allocated resources
 */
VisConfigEditor::~VisConfigEditor ()
{
    // no need to delete child widgets, Qt does it all for us
}

/**
 * Get some help.
 */
void 
VisConfigEditor::helpBtn_clicked ()
{
    QMessageBox::about (this, "Configuration Editor",
			"The configuration editor is an interactive tool\n"
			"for Visualization parameter modification.\n\n"
			"Changing a parameter couses re-draw of the objects which use it.\n"
			"Copyright 2007 CMS\n");
}

/**
 * Edit a configurable parameter.
 */
void 
VisConfigEditor::editBtn_clicked ()
{
    VisParamEditor* parEditor = new VisParamEditor (m_state, 0, "parEditor");
    parEditor->peNameStr->setText (configListView->selectedItem ()->text (0));
    parEditor->peValueStr->setText (configListView->selectedItem ()->text (1));
    connect (parEditor->peApplyBtn, SIGNAL (clicked ()), this, SLOT (refreshBtn_clicked ()));
    connect (parEditor->peOkBtn, SIGNAL (clicked ()), this, SLOT (refreshBtn_clicked ()));
    parEditor->peApplyBtn->setEnabled (false);
    parEditor->show ();
}

/**
 * Finish the session with the configuration 
 * editor: just close it for now.
 */
void 
VisConfigEditor::okBtn_clicked ()
{
    this->hide ();
}

/**
 * Cancel the session with the configuration 
 * editor: just close it for now.
 */
void 
VisConfigEditor::cancelBtn_clicked ()
{
    this->hide ();
}

/**
 * Start a parameter editor when a configurable item is
 * double-clicked.
 */
void 
VisConfigEditor::configListView_doubleClicked (Q3ListViewItem * /* item */)
{
    VisParamEditor *parEditor = new VisParamEditor (m_state, 0, "parEditor");
    parEditor->peNameStr->setText (configListView->selectedItem ()->text (0));
    parEditor->peValueStr->setText (configListView->selectedItem ()->text (1));
    connect (parEditor->peApplyBtn, SIGNAL (clicked ()), this, SLOT (refreshBtn_clicked ()));
    connect (parEditor->peOkBtn, SIGNAL (clicked ()), this, SLOT (refreshBtn_clicked ()));
    parEditor->peApplyBtn->setEnabled (false);
    parEditor->show ();
}

/**
 * Enable editing when selection changed.
 */
void 
VisConfigEditor::configListView_selectionChanged ()
{
    editBtn->setEnabled (true);
}

/**
 * Enable editing when a configurable item is selected.
 */
void 
VisConfigEditor::configListView_selectionChanged (Q3ListViewItem * /* item */)
{
    editBtn->setEnabled (true);
}

/**
 * Update VisConfiguration on refresh.
 */
void 
VisConfigEditor::refreshBtn_clicked ()
{
    VisConfiguration::VisState simpConf;
    VisConfiguration *configuration = VisConfiguration::get (m_state);
    ASSERT (configuration);
    configuration->dumpConfig (simpConf);

    configListView->clear ();
    editBtn->setEnabled (false);

    typedef std::map< std::string, std::string, std::less< std::string > >::const_iterator SimpConfItr;
    SimpConfItr param = simpConf.begin ();
    SimpConfItr paramEnd = simpConf.end ();
    for ( ; param != paramEnd; ++param) 
    {
	std::cout << (*param).first << " = " << (*param).second << std::endl;
	new Q3ListViewItem (configListView, QString (((*param).first).c_str ()),  QString (((*param).second).c_str ()));
    }    
}

/**
 * Start a parameter editor to create a new configurable.
 */
void 
VisConfigEditor::newBtn_clicked ()
{
    VisParamEditor *parEditor = new VisParamEditor (m_state, 0, "parEditor");
    connect (parEditor->peApplyBtn, SIGNAL (clicked ()), this, SLOT (refreshBtn_clicked ()));
    connect (parEditor->peOkBtn, SIGNAL (clicked ()), this, SLOT (refreshBtn_clicked ()));
    parEditor->peApplyBtn->setEnabled (false);
    parEditor->show ();
}

/**
 *
 */
void 
VisConfigEditor::show ()
{
    refreshBtn_clicked ();
    QWidget::show ();
}

void 
VisConfigEditor::saveConfig ()
{
    refreshBtn_clicked ();
}
