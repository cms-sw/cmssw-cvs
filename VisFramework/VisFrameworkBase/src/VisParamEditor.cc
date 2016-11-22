//<<<<<< INCLUDES                                                       >>>>>>

#include "VisFramework/VisFrameworkBase/interface/VisParamEditor.h"
#include "VisFramework/VisFrameworkBase/interface/VisConfiguration.h"
#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qmessagebox.h> 
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

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

/* 
 *  Constructs a VisParamEditor which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
VisParamEditor::VisParamEditor (IgState *state, QWidget *parent, const char *name, Qt::WFlags fl)
    : QWidget (parent, name, fl),
      m_state (state)
{
    if (!name)
	setName ("VisParamEditor");
    resize (506, 184); 
    setCaption (trUtf8 ("Edit Configurable Parameter"));
    paramEditorLayout = new Q3VBoxLayout (this, 11, 6, "paramEditorLayout"); 

    peLayout = new Q3VBoxLayout (0, 0, 6, "peLayout"); 

    peNameLbl = new QLabel (this, "peNameLbl");
    peNameLbl->setText (trUtf8 ("Name"));
    peLayout->addWidget (peNameLbl);

    peNameStr = new QLineEdit (this, "peNameStr");
    peLayout->addWidget (peNameStr);

    peValueLbl = new QLabel (this, "peValueLbl");
    peValueLbl->setText (trUtf8 ("Value"));
    peLayout->addWidget (peValueLbl);

    peValueStr = new QLineEdit (this, "peValueStr");
    peLayout->addWidget (peValueStr);

    peBtnLayout = new Q3HBoxLayout (0, 0, 6, "peBtnLayout"); 

    peHelpBtn = new QPushButton (this, "peHelpBtn");
    peHelpBtn->setText (trUtf8 ("Help"));
    peBtnLayout->addWidget (peHelpBtn);

    peApplyBtn = new QPushButton (this, "peApplyBtn");
    peApplyBtn->setEnabled (false);
    peApplyBtn->setText (trUtf8 ("Apply"));
    peBtnLayout->addWidget (peApplyBtn);

    peOkBtn = new QPushButton (this, "peOkBtn");
    peOkBtn->setText (trUtf8 ("OK"));
    peOkBtn->setOn (false);
    peOkBtn->setDefault (true);
    peBtnLayout->addWidget (peOkBtn);

    peCancelBtn = new QPushButton (this, "peCancelBtn");
    peCancelBtn->setText (trUtf8 ("Cancel"));
    peBtnLayout->addWidget (peCancelBtn);
    peLayout->addLayout (peBtnLayout);
    paramEditorLayout->addLayout (peLayout);

    // signals and slots connections
    connect (peHelpBtn, SIGNAL (clicked ()), this, SLOT (peHelpBtn_clicked ()));
    connect (peApplyBtn, SIGNAL (clicked ()), this, SLOT (peApplyBtn_clicked ()));
    connect (peOkBtn, SIGNAL (clicked ()), this, SLOT (peOkBtn_clicked ()));
    connect (peCancelBtn, SIGNAL (clicked ()), this, SLOT (peCancelBtn_clicked ()));
    connect (peNameStr, SIGNAL (textChanged (const QString&)), this, SLOT (peNameStr_textChanged (const QString&)));
    connect (peNameStr, SIGNAL (returnPressed ()), this, SLOT (peNameStr_returnPressed ()));
    connect (peValueStr, SIGNAL (returnPressed ()), this, SLOT (peValueStr_returnPressed ()));
    connect (peValueStr, SIGNAL (textChanged (const QString&)), this, SLOT (peValueStr_textChanged (const QString&)));
}

/*  
 *  Destroys the object and frees any allocated resources
 */
VisParamEditor::~VisParamEditor()
{
    // no need to delete child widgets, Qt does it all for us
}

/**
 * Give some help.
 */
void
VisParamEditor::peHelpBtn_clicked ()
{
    QMessageBox::about (this, "Configurable Parameter Editor",
			"Visualization configurable parameters editor\n"
			"Copyright 2007 CMS\n");
}

/**
 * Apply changes.
 */
void 
VisParamEditor::peApplyBtn_clicked ()
{
    if (peApplyBtn->isEnabled ())
    {	
	std::string name (peNameStr->text ().latin1 ());                                                                                  
	std::string value (peValueStr->text ().latin1 ());
	if (!name.empty ()) VisConfiguration::get (m_state)->modify (name, value);
	else QMessageBox::warning (this, "Warning: Empty name",
				   "Empty name cannot be set!\n"
				   "Please, change the name\n"
				   "to something meaningful.");
	peApplyBtn->setEnabled (false);
    }	
}

/**
 * Finish editing the parameter:
 * apply changes and close the dialog.
 */
void 
VisParamEditor::peOkBtn_clicked ()
{
    peApplyBtn_clicked ();
    this->close ();
}

/**
 * Cancel session with the parameter editor:
 * close the parameter editor window.
 */
void 
VisParamEditor::peCancelBtn_clicked ()
{
    this->close ();
}

/**
 * When text changes enable "Apply" button.
 */
void 
VisParamEditor::peNameStr_textChanged (const QString &)
{
    peApplyBtn->setEnabled (true);
}

/**
 * Enable "Apply" on "Return" pressed within text 
 * field and apply the changes.
 */
void 
VisParamEditor::peNameStr_returnPressed ()
{
    peApplyBtn->setEnabled (true);
    peApplyBtn_clicked ();
}

/**
 * Enable "Apply" on "Return" pressed within text 
 * field.
 */
void 
VisParamEditor::peValueStr_returnPressed ()
{
    peApplyBtn->setEnabled (true);
    peApplyBtn_clicked ();
}

/**
 * Enable "Apply" on changes of the text.
 */
void 
VisParamEditor::peValueStr_textChanged (const QString&)
{
    peApplyBtn->setEnabled (true);
}
