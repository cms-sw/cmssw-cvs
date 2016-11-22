/****************************************************************************
** Form implementation generated from reading ui file 'VisFramework/VisFrameworkBase/src/VisEventNavigationDialog.ui'
**
** Created: Fri Feb 9 11:22:08 2007
**      by: The User Interface Compiler ($Id: VisEventNavigationDialog.cc,v 1.2 2009/04/06 13:27:03 yana Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "VisFramework/VisFrameworkBase/interface/VisEventNavigationDialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <q3buttongroup.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3VBoxLayout>

/*
 *  Constructs a VisEventNavigationDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
VisEventNavigationDialog::VisEventNavigationDialog( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "VisEventNavigationDialog" );
    VisEventNavigationDialogLayout = new Q3GridLayout( this, 1, 1, 11, 6, "VisEventNavigationDialogLayout"); 

    skipNumberLineEdit = new QLineEdit( this, "skipNumberLineEdit" );

    VisEventNavigationDialogLayout->addWidget( skipNumberLineEdit, 0, 1 );

    buttonLayout = new Q3VBoxLayout( 0, 0, 6, "buttonLayout"); 

    okPushButton = new QPushButton( this, "okPushButton" );
    buttonLayout->addWidget( okPushButton );

    closePushButton = new QPushButton( this, "closePushButton" );
    buttonLayout->addWidget( closePushButton );

    VisEventNavigationDialogLayout->addLayout( buttonLayout, 0, 2 );

    buttonGroup = new Q3ButtonGroup( this, "buttonGroup" );
    buttonGroup->setExclusive( TRUE );
    buttonGroup->setColumnLayout(0, Qt::Vertical );
    buttonGroup->layout()->setSpacing( 6 );
    buttonGroup->layout()->setMargin( 11 );
    buttonGroupLayout = new Q3GridLayout( buttonGroup->layout() );
    buttonGroupLayout->setAlignment( Qt::AlignTop );

    forwardRadioButton = new QRadioButton( buttonGroup, "forwardRadioButton" );
    forwardRadioButton->setChecked( TRUE );

    buttonGroupLayout->addWidget( forwardRadioButton, 0, 0 );

    backRadioButton = new QRadioButton( buttonGroup, "backRadioButton" );

    buttonGroupLayout->addWidget( backRadioButton, 1, 0 );

    VisEventNavigationDialogLayout->addWidget( buttonGroup, 0, 0 );
    languageChange();
    resize( QSize(369, 103).expandedTo(minimumSizeHint()) );
    //    clearWState( WState_Polished );

    // signals and slots connections
    connect( okPushButton, SIGNAL( clicked() ), this, SLOT( skip() ) );
    connect( closePushButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( skipNumberLineEdit, SIGNAL( returnPressed() ), this, SLOT( skip() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
VisEventNavigationDialog::~VisEventNavigationDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void VisEventNavigationDialog::languageChange()
{
    setCaption( tr( "Event Navigation" ) );
    okPushButton->setText( tr( "Go" ) );
    QToolTip::add( okPushButton, tr( "Skip events" ) );
    Q3WhatsThis::add( okPushButton, tr( "Skip specified number of events forward." ) );
    closePushButton->setText( tr( "Close" ) );
    QToolTip::add( closePushButton, tr( "Close the dialog" ) );
    Q3WhatsThis::add( closePushButton, tr( "Close the dialog." ) );
    buttonGroup->setTitle( tr( "Skip" ) );
    forwardRadioButton->setText( tr( "Forward" ) );
    backRadioButton->setText( tr( "Back" ) );
}

void VisEventNavigationDialog::skipBackwards()
{
    qWarning( "VisEventNavigationDialog::skipBackwards(): Not implemented yet" );
}

void VisEventNavigationDialog::skipForward()
{
    qWarning( "VisEventNavigationDialog::skipForward(): Not implemented yet" );
}

void VisEventNavigationDialog::skip()
{
    accept ();
}

