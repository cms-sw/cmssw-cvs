/****************************************************************************
** Form implementation generated from reading ui file 'VisL1TriggerFilterMenu.ui'
**
** Created: Fri Oct 24 15:10:42 2008
**      by: The User Interface Compiler ($Id: VisL1TriggerFilterMenu.cc,v 1.2 2009/04/06 13:27:03 yana Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "VisFramework/VisFrameworkBase/interface/VisL1TriggerFilterMenu.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <q3header.h>
#include <q3listview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

/*
 *  Constructs a VisL1TriggerMenu as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
VisL1TriggerMenu::VisL1TriggerMenu( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "VisL1TriggerMenu" );
    setSizeGripEnabled( TRUE );
    VisL1TriggerMenuLayout = new Q3GridLayout( this, 1, 1, 11, 6, "VisL1TriggerMenuLayout"); 

    layout8 = new Q3HBoxLayout( 0, 0, 6, "layout8"); 

    l1EventFilter = new QRadioButton( this, "l1EventFilter" );
    layout8->addWidget( l1EventFilter );

    l1EventFilterExpression = new QLineEdit( this, "l1EventFilterExpression" );
    l1EventFilterExpression->setEnabled( FALSE );
    l1EventFilterExpression->setReadOnly( TRUE );
    layout8->addWidget( l1EventFilterExpression );

    VisL1TriggerMenuLayout->addLayout( layout8, 1, 0 );

    Layout1 = new Q3HBoxLayout( 0, 0, 6, "Layout1"); 

    buttonHelp = new QPushButton( this, "buttonHelp" );
    buttonHelp->setAutoDefault( TRUE );
    Layout1->addWidget( buttonHelp );
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );

    VisL1TriggerMenuLayout->addLayout( Layout1, 2, 0 );

    layout16 = new Q3HBoxLayout( 0, 0, 6, "layout16"); 

    listView = new Q3ListView( this, "listView" );
    listView->addColumn( tr( "L1 Algorithm" ) );
    listView->addColumn( tr( "Bit" ) );
    listView->setSelectionMode( Q3ListView::Multi );
    listView->setAllColumnsShowFocus( TRUE );
    layout16->addWidget( listView );

    layout15 = new Q3VBoxLayout( 0, 0, 6, "layout15"); 
    spacer2 = new QSpacerItem( 20, 71, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout15->addItem( spacer2 );

    pushButtonAnd = new QPushButton( this, "pushButtonAnd" );
    layout15->addWidget( pushButtonAnd );

    pushButtonOr = new QPushButton( this, "pushButtonOr" );
    layout15->addWidget( pushButtonOr );
    spacer3 = new QSpacerItem( 20, 61, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout15->addItem( spacer3 );
    layout16->addLayout( layout15 );

    VisL1TriggerMenuLayout->addLayout( layout16, 0, 0 );
    languageChange();
    resize( QSize(499, 329).expandedTo(minimumSizeHint()) );
    //    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( listView, SIGNAL( clicked(Q3ListViewItem*) ), this, SLOT( selectAlgo(Q3ListViewItem*) ) );
    connect( l1EventFilter, SIGNAL( toggled(bool) ), this, SLOT( enableFilter(bool) ) );
    connect( buttonHelp, SIGNAL( clicked() ), this, SLOT( help() ) );
    connect( l1EventFilter, SIGNAL( toggled(bool) ), l1EventFilterExpression, SLOT( setEnabled(bool) ) );
    connect( pushButtonAnd, SIGNAL( clicked() ), this, SLOT( andAlgo() ) );
    connect( pushButtonOr, SIGNAL( clicked() ), this, SLOT( orAlgo() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
VisL1TriggerMenu::~VisL1TriggerMenu()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void VisL1TriggerMenu::languageChange()
{
    setCaption( tr( "L1 Trigger Filter" ) );
    l1EventFilter->setText( QString::null );
    buttonHelp->setText( tr( "&Help" ) );
    buttonHelp->setAccel( QKeySequence( tr( "F1" ) ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
    listView->header()->setLabel( 0, tr( "L1 Algorithm" ) );
    listView->header()->setLabel( 1, tr( "Bit" ) );
    pushButtonAnd->setText( tr( "AND" ) );
    pushButtonOr->setText( tr( "OR" ) );
}

void VisL1TriggerMenu::selectAlgo(Q3ListViewItem*)
{
    qWarning( "VisL1TriggerMenu::selectAlgo(QListViewItem*): Not implemented yet" );
}

void VisL1TriggerMenu::enableFilter(bool)
{
    qWarning( "VisL1TriggerMenu::enableFilter(bool): Not implemented yet" );
}

void VisL1TriggerMenu::help()
{
    qWarning( "VisL1TriggerMenu::help(): Not implemented yet" );
}

void VisL1TriggerMenu::andAlgo()
{
    qWarning( "VisL1TriggerMenu::andAlgo(): Not implemented yet" );
}

void VisL1TriggerMenu::orAlgo()
{
    qWarning( "VisL1TriggerMenu::orAlgo(): Not implemented yet" );
}

