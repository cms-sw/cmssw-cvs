/****************************************************************************
** Form implementation generated from reading ui file 'VisFrameworkBase/src/VisProcessEventDialog.ui'
**
** Created: Thu Feb 8 11:05:29 2007
**      by: The User Interface Compiler ($Id: VisProcessEventDialog.cc,v 1.3 2010/07/02 13:59:09 yana Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "VisFramework/VisFrameworkBase/interface/VisProcessEventDialog.h"

#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>

/*
 *  Constructs a VisProcessEventDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
VisProcessEventDialog::VisProcessEventDialog( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "VisProcessEventDialog" );
    VisProcessEventDialogLayout = new Q3GridLayout( this, 1, 1, 11, 6, "VisProcessEventDialogLayout"); 

    topLayout = new Q3HBoxLayout( 0, 0, 6, "topLayout"); 

    labelsAndTextLayout = new Q3HBoxLayout( 0, 0, 6, "labelsAndTextLayout"); 

    labelsLayout = new Q3VBoxLayout( 0, 0, 6, "labelsLayout"); 

    runNumberLabel = new QLabel( this, "runNumberLabel" );
    labelsLayout->addWidget( runNumberLabel );

    eventNumberLabel = new QLabel( this, "eventNumberLabel" );
    labelsLayout->addWidget( eventNumberLabel );
    labelsAndTextLayout->addLayout( labelsLayout );

    textLayout = new Q3VBoxLayout( 0, 0, 6, "textLayout"); 

    runNumberText = new QLineEdit( this, "runNumberText" );
    textLayout->addWidget( runNumberText );

    eventNumberText = new QLineEdit( this, "eventNumberText" );
    textLayout->addWidget( eventNumberText );
    labelsAndTextLayout->addLayout( textLayout );
    topLayout->addLayout( labelsAndTextLayout );

    buttonsLayout = new Q3VBoxLayout( 0, 0, 6, "buttonsLayout"); 

    okButton = new QPushButton( this, "okButton" );
    //    okButton->setFocusPolicy( QPushButton::TabFocus );
    okButton->setDefault( TRUE );
    buttonsLayout->addWidget( okButton );

    cancelButton = new QPushButton( this, "cancelButton" );
    buttonsLayout->addWidget( cancelButton );
    topLayout->addLayout( buttonsLayout );

    VisProcessEventDialogLayout->addLayout( topLayout, 0, 0 );
    languageChange();
    resize( QSize(366, 88).expandedTo(minimumSizeHint()) );
    //    clearWState( WState_Polished );

    // signals and slots connections
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
VisProcessEventDialog::~VisProcessEventDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void VisProcessEventDialog::languageChange()
{
    setCaption( tr( "Goto Event" ) );
    runNumberLabel->setText( tr( "Run number" ) );
    eventNumberLabel->setText( tr( "Event number" ) );
    okButton->setText( tr( "Go" ) );
    QToolTip::add( okButton, tr( "Go to the event specified by the run number and the event number." ) );
    Q3WhatsThis::add( okButton, tr( "Go to the event specified by the event number and the run number." ) );
    cancelButton->setText( tr( "Cancel" ) );
    QToolTip::add( cancelButton, tr( "Cancel the request and close the dialog." ) );
    Q3WhatsThis::add( cancelButton, tr( "Cancel the request and close the dialog." ) );
}

