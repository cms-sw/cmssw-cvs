#ifndef IGUANA_GL_BROWSERS_IG_SPARE_BROWSER_H
# define IGUANA_GL_BROWSERS_IG_SPARE_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/GLBrowsers/interface/Ig3DBaseBrowser.h"
//Added by qt3to4:
#include <QEvent>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgSpareModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/*! 
 *
 * \class IgSpareBrowser
 * \brief Implements a generic 3DBaseBrowser modeled after IgRPhiBrowser
 *
 *  This is intended to be used for specialist displays that need the utility
 *  of a IgRPhiBrowser, but which would interfere with its normal use.
 *
 */
class IGUANA_GL_BROWSERS_API IgSpareBrowser : public Ig3DBaseBrowser
{
    Q_OBJECT
public:
    IgSpareBrowser (IgState *state, IgSite *site, IgSpareModel *model = 0);

    static const char *		catalogLabel (void);
    bool                        isGridVisible (void);

public slots:
    virtual void                setGridVisibility (bool enable);
    virtual void 		viewPlaneX (void);
    virtual void 		viewPlaneY (void);
    virtual void 		viewPlaneZ (void);    
    virtual void 		toggleCameraType (void);
    virtual void		invertCamera (void);    

protected:
    static SbBool 		eventCallback (void *closure, QEvent *event);
    
    virtual void 		buildDecoration (QWidget *parent);    
    virtual QWidget 		*buildLeftTrim (QWidget *parent);
    virtual QWidget 		*buildBottomTrim (QWidget *parent);
    virtual QWidget 		*buildRightTrim (QWidget *parent);

private:
    virtual void                drawGrid (const bool enable);
    virtual void		init (void);
    bool			m_grid;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_SPARE_BROWSER_H
