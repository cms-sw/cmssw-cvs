#ifndef IGUANA_GL_BROWSERS_IG_SPARE_VIEW_PROPERTIES_CATEGORY_H
# define IGUANA_GL_BROWSERS_IG_SPARE_VIEW_PROPERTIES_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/GLBrowsers/interface/IgControlCategory.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <qobject.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgSpareBrowser;
class IgQtAxisAlignment;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/*!
 *
 * \class IgSpareViewPropertiesCategory
 * \brief Implements a generic IgSpareViewProperties Category modeled after IgRPhiViewPropertiesCategory
 *
 */
class IGUANA_GL_BROWSERS_API IgSpareViewPropertiesCategory 
    : public IgControlCategory,
      public IgStateElement
{
    Q_OBJECT
    IG_DECLARE_STATE_ELEMENT (IgSpareViewPropertiesCategory);
    
public:
    IgSpareViewPropertiesCategory (IgState *state,
				  IgSpareBrowser *source);
    ~IgSpareViewPropertiesCategory (void);

    virtual void	browserChanged  (IgSpareBrowser *source);
    virtual void	registerBrowser (IgState *state,
                                         IgSpareBrowser *source);

public slots:
    virtual void	alignAxis (void);

private:
    IgSpareBrowser	*m_source;

    IgQtAxisAlignment	*m_axisAlignment; // 

    // undefined semantics
    IgSpareViewPropertiesCategory (const IgSpareViewPropertiesCategory &);
    IgSpareViewPropertiesCategory &operator = (const IgSpareViewPropertiesCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_SPARE_VIEW_PROPERTIES_CATEGORY_H
