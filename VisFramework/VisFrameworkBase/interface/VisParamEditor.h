#ifndef VIS_FRAMEWORK_BASE_VIS_PARAM_EDITOR_H
# define VIS_FRAMEWORK_BASE_VIS_PARAM_EDITOR_H

#include <qvariant.h>
#include <qwidget.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>
class Q3VBoxLayout; 
class Q3HBoxLayout; 
class Q3GridLayout; 
class QLabel;
class QLineEdit;
class QPushButton;
class IgState;

class VisParamEditor : public QWidget
{ 
    Q_OBJECT

public:
    VisParamEditor (IgState *state, QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0);
    ~VisParamEditor ();

    QLabel* peNameLbl;
    QLineEdit* peNameStr;
    QLabel* peValueLbl;
    QLineEdit* peValueStr;
    QPushButton* peHelpBtn;
    QPushButton* peApplyBtn;
    QPushButton* peOkBtn;
    QPushButton* peCancelBtn;

public slots:
    virtual void peHelpBtn_clicked();
    virtual void peApplyBtn_clicked();
    virtual void peOkBtn_clicked();
    virtual void peCancelBtn_clicked();
    virtual void peNameStr_textChanged(const QString &);
    virtual void peNameStr_returnPressed();
    virtual void peValueStr_returnPressed();
    virtual void peValueStr_textChanged(const QString &);

protected:
    Q3VBoxLayout* paramEditorLayout;
    Q3VBoxLayout* peLayout;
    Q3HBoxLayout* peBtnLayout;

private:
    IgState 	*m_state;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_PARAM_EDITOR_H
