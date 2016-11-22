#ifndef IGUANA_GL_BROWSERS_IG_RZ_VIEW_PROPERTIES_CATEGORY_H
# define IGUANA_GL_BROWSERS_IG_RZ_VIEW_PROPERTIES_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/GLBrowsers/interface/IgControlCategory.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgRZBrowser;
class IgQtAxisAlignment;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgRZViewPropertiesCategory 
    : public IgControlCategory,
      public IgStateElement
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (IgRZViewPropertiesCategory);
    
public:
    IgRZViewPropertiesCategory (IgState *state,
				IgRZBrowser *source);
    ~IgRZViewPropertiesCategory (void);

    virtual void	browserChanged  (IgRZBrowser *source);
    virtual void	registerBrowser (IgState *state,
                                         IgRZBrowser *source);

public slots:
    virtual void	alignAxis (void);

private:
    IgRZBrowser	*m_source;

    IgQtAxisAlignment	*m_axisAlignment; // 

    // undefined semantics
    IgRZViewPropertiesCategory (const IgRZViewPropertiesCategory &);
    IgRZViewPropertiesCategory &operator = (const IgRZViewPropertiesCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_RZ_VIEW_PROPERTIES_CATEGORY_H
