#ifndef IGUANA_GL_BROWSERS_IG_GL2PS_PRINT_CATEGORY_H
# define IGUANA_GL_BROWSERS_IG_GL2PS_PRINT_CATEGORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgCommonViewCategory.h"
# include "Iguana/Framework/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QCheckBox;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgGL2PSPrintCategory
    : public IgCommonViewCategory,
      public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgGL2PSPrintCategory);    
    Q_OBJECT
public:
    IgGL2PSPrintCategory (IgState *state);
    ~IgGL2PSPrintCategory ();

    virtual void		browserChanged 	(Ig3DBaseBrowser *browser);
  
private slots:
    virtual void		updateGL2PSOptions	(void);
    virtual void		setGL2PSOptions		(void);

private:
    void			addGL2PSOptionTab 	(void);

    QCheckBox  			*m_gl2psSimpleLineOffset;
    QCheckBox  			*m_gl2psBestRoot;
    QCheckBox  			*m_gl2psOcclusionCulling;
    QCheckBox 			*m_gl2psBackGround;   
    QCheckBox  			*m_gl2psLandScape;
    static bool			updated;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_GL2PS_PRINT_CATEGORY_H
