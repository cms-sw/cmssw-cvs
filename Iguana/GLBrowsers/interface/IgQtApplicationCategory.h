#ifndef IGUANA_GL_BROWSERS_IG_QTAPPLICATION_CATEGORY_H
# define IGUANA_GL_BROWSERS_IG_QTAPPLICATION_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgCommonViewCategory.h"
# include "Iguana/Framework/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QComboBox;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgQtApplicationCategory
    : public IgCommonViewCategory,
      public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgQtApplicationCategory);    
    Q_OBJECT
public:
    IgQtApplicationCategory (IgState *state);
    ~IgQtApplicationCategory ();

private slots:
    virtual void		setWindowStyle		(const QString & style);
    virtual void		updateWindowStyle  	(const QString & style);

private:
    void			addQApplicationTab 	(void);
    
    QComboBox			*m_windowStyle;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QTAPPLICATION_CATEGORY_H
