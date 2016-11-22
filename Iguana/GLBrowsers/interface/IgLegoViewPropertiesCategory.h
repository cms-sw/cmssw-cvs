#ifndef IGUANA_GL_BROWSERS_IG_LEGO_VIEW_PROPERTIES_CATEGORY_H
# define IGUANA_GL_BROWSERS_IG_LEGO_VIEW_PROPERTIES_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/GLBrowsers/interface/IgControlCategory.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgLegoBrowser;
class IgQtAxisAlignment;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgLegoViewPropertiesCategory 
    : public IgControlCategory,
      public IgStateElement
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (IgLegoViewPropertiesCategory);
    
public:
    IgLegoViewPropertiesCategory (IgState *state,
				  IgLegoBrowser *source);
    ~IgLegoViewPropertiesCategory (void);

    virtual void	browserChanged  (IgLegoBrowser *source);
    virtual void	registerBrowser (IgState *state,
                                         IgLegoBrowser *source);

public slots:
    virtual void	alignAxis (void);

private:
    IgLegoBrowser	*m_source;

    IgQtAxisAlignment	*m_axisAlignment; // 

    // undefined semantics
    IgLegoViewPropertiesCategory (const IgLegoViewPropertiesCategory &);
    IgLegoViewPropertiesCategory &operator = (const IgLegoViewPropertiesCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_LEGO_VIEW_PROPERTIES_CATEGORY_H
