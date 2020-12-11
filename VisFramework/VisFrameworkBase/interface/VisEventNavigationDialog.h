/****************************************************************************
** Form interface generated from reading ui file 'VisFramework/VisFrameworkBase/src/VisEventNavigationDialog.ui'
**
** Created: Fri Feb 9 11:21:43 2007
**      by: The User Interface Compiler ($Id: VisEventNavigationDialog.h,v 1.2 2009/04/06 13:27:02 yana Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef VISEVENTNAVIGATIONDIALOG_H
#define VISEVENTNAVIGATIONDIALOG_H

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
class QLineEdit;
class QPushButton;
class Q3ButtonGroup;
class QRadioButton;

class VisEventNavigationDialog : public QDialog
{
    Q_OBJECT

public:
    VisEventNavigationDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~VisEventNavigationDialog();

    QLineEdit* skipNumberLineEdit;
    QPushButton* okPushButton;
    QPushButton* closePushButton;
    Q3ButtonGroup* buttonGroup;
    QRadioButton* forwardRadioButton;
    QRadioButton* backRadioButton;

public slots:
    virtual void skipBackwards();
    virtual void skipForward();
    virtual void skip();

protected:
    Q3GridLayout* VisEventNavigationDialogLayout;
    Q3VBoxLayout* buttonLayout;
    Q3GridLayout* buttonGroupLayout;

protected slots:
    virtual void languageChange();

};

#endif // VISEVENTNAVIGATIONDIALOG_H
