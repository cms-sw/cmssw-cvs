#ifndef VIS_EVENT_VIS_EVENT_CONTENT_TWIG_OPS_H
# define VIS_EVENT_VIS_EVENT_CONTENT_TWIG_OPS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisEventContentTwig;
class IgQtObjectMenu;
class IgQtObjectMenuMessage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisEventContentTwigOps : public QObject
{
    Q_OBJECT
public:
    static const int MENU_EXPAND        = 71;

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    void                menuAction (IgQtObjectMenuMessage message);
    void                menuUpdate (void);
    
public slots:
    void                actionExpand (int id);

private:
    VisEventContentTwig *m_twig;
    IgQtObjectMenu      *m_menu;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_EVENT_VIS_EVENT_CONTENT_TWIG_OPS_H
