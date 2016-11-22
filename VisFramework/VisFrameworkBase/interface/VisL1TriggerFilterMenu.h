/****************************************************************************
** Form interface generated from reading ui file 'VisL1TriggerFilterMenu.ui'
**
** Created: Fri Oct 24 15:09:56 2008
**      by: The User Interface Compiler ($Id: VisL1TriggerFilterMenu.h,v 1.2 2009/04/06 13:27:02 yana Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef VISL1TRIGGERMENU_H
#define VISL1TRIGGERMENU_H

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QRadioButton;
class QLineEdit;
class QPushButton;
class Q3ListView;
class Q3ListViewItem;

class VisL1TriggerMenu : public QDialog
{
    Q_OBJECT

public:
    VisL1TriggerMenu( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~VisL1TriggerMenu();

    QRadioButton* l1EventFilter;
    QLineEdit* l1EventFilterExpression;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    Q3ListView* listView;
    QPushButton* pushButtonAnd;
    QPushButton* pushButtonOr;

public slots:
    virtual void selectAlgo(Q3ListViewItem*);
    virtual void enableFilter(bool);
    virtual void help();
    virtual void andAlgo();
    virtual void orAlgo();

protected:
    Q3GridLayout* VisL1TriggerMenuLayout;
    Q3HBoxLayout* layout8;
    Q3HBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;
    Q3HBoxLayout* layout16;
    Q3VBoxLayout* layout15;
    QSpacerItem* spacer2;
    QSpacerItem* spacer3;

protected slots:
    virtual void languageChange();

};

#endif // VISL1TRIGGERMENU_H
