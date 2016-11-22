#ifndef VIS_FRAMEWORK_BASE_VIS_CONFIG_EDITOR_H
# define VIS_FRAMEWORK_BASE_VIS_CONFIG_EDITOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <qvariant.h>
# include <qwidget.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>

class Q3VBoxLayout; 
class Q3HBoxLayout; 
class Q3GridLayout; 
class Q3ListView;
class Q3ListViewItem;
class QPushButton;
class IgState;

class VisConfigEditor : public QWidget
{
    Q_OBJECT
public:
    VisConfigEditor (IgState *state, QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0);
    ~VisConfigEditor (void);

    Q3ListView 	*configListView;
    QPushButton *helpBtn;
    QPushButton *newBtn;
    QPushButton *editBtn;
    QPushButton *refreshBtn;
    QPushButton *okBtn;
    QPushButton *cancelBtn;


public slots:
    virtual void helpBtn_clicked (void);
    virtual void editBtn_clicked (void);
    virtual void okBtn_clicked (void);
    virtual void cancelBtn_clicked (void);
    virtual void configListView_doubleClicked (Q3ListViewItem *);
    virtual void configListView_selectionChanged (void);
    virtual void configListView_selectionChanged (Q3ListViewItem *);
    virtual void refreshBtn_clicked (void);
    virtual void newBtn_clicked (void);
    virtual void show (void);
    virtual void saveConfig (void);

protected:
    Q3VBoxLayout *configEditorLayout;
    Q3VBoxLayout *layout2;
    Q3HBoxLayout *layout1;

private:
    IgState 	*m_state;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_FRAMEWORK_BASE_VIS_CONFIG_EDITOR_H
