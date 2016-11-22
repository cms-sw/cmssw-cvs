#ifndef IGUANA_STUDIO_IG_PAGE_H
# define IGUANA_STUDIO_IG_PAGE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Studio/interface/config.h"
# include "Iguana/Studio/interface/IgQHSplitterSite.h"
# include <QObject>
# include <list>
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgDocument;
class IgView;
class IgQtSiteStack;
class QMdiArea;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_STUDIO_API IgPage : public QObject, public IgQHSplitterSite
{
    Q_OBJECT
public:
    IgPage (IgDocument *doc);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    QMdiArea		*workspace (void);
    
    virtual IgState *	state (void) const;
    void		registerView (IgView *view, 
				      QWidget *selectableWidget);
    void 		printViews (const QString &text);

public slots:
    void 		showMaximized (void);
    void		showFullScreen (void);

signals:
    void		autoPrint (const QString &text);
    
private:
    IgDocument		*m_document;
    QMdiArea		*m_workspace;    
    
    std::map<QWidget *, IgView*>	m_views;
    int 		m_numRegisteredWindows;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_STUDIO_IG_PAGE_H
