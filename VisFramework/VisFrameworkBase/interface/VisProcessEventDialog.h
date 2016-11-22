/****************************************************************************
** Form interface generated from reading ui file 'VisProcessEventDialog.ui'
**
** Created: Thu Feb 8 11:00:29 2007
**      by: The User Interface Compiler ($Id: VisProcessEventDialog.h,v 1.2 2009/04/06 13:27:02 yana Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef VISPROCESSEVENTDIALOG_H
#define VISPROCESSEVENTDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QPushButton;

class VisProcessEventDialog : public QDialog
{
    Q_OBJECT

public:
    VisProcessEventDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~VisProcessEventDialog();

    QLabel* runNumberLabel;
    QLabel* eventNumberLabel;
    QLineEdit* runNumberText;
    QLineEdit* eventNumberText;
    QPushButton* okButton;
    QPushButton* cancelButton;

protected:
    Q3GridLayout* VisProcessEventDialogLayout;
    Q3HBoxLayout* topLayout;
    Q3HBoxLayout* labelsAndTextLayout;
    Q3VBoxLayout* labelsLayout;
    Q3VBoxLayout* textLayout;
    Q3VBoxLayout* buttonsLayout;

protected slots:
    virtual void languageChange();

};

#endif // VISPROCESSEVENTDIALOG_H
