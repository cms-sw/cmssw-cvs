#ifndef IGUANA_GL_BROWSERS_IG_COMMON_VIEW_CATEGORY_H
# define IGUANA_GL_BROWSERS_IG_COMMON_VIEW_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/GLBrowsers/interface/IgControlCategory.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class Ig3DBaseBrowser;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgCommonViewCategory
    : public IgControlCategory
{
    Q_OBJECT
public:
    IgCommonViewCategory (IgState *state, const QString& name);

    virtual void		browserChanged 	(Ig3DBaseBrowser *browser);
    virtual void		registerBrowser (IgState *state,
                                                 Ig3DBaseBrowser *browser);    
public slots:
    virtual void        	show 			(void);
  
protected:
    Ig3DBaseBrowser	       	*m_source;
        
private:
    // undefined semantics
    IgCommonViewCategory (const IgCommonViewCategory &);
    IgCommonViewCategory &operator= (const IgCommonViewCategory &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_COMMON_VIEW_CATEGORY_H
