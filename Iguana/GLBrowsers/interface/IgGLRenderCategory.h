#ifndef IGUANA_GL_BROWSERS_IG_GL_RENDER_CATEGORY_H
# define IGUANA_GL_BROWSERS_IG_GL_RENDER_CATEGORY_H

# include "Iguana/GLBrowsers/interface/IgCommonViewCategory.h"
# include "Iguana/Framework/interface/IgStateElement.h"

class QCheckBox;
class QComboBox;
class IgQtRangeControlUInt;

class IGUANA_GL_BROWSERS_API IgGLRenderCategory
    : public IgCommonViewCategory,
      public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgGLRenderCategory);    
    Q_OBJECT
public:
    IgGLRenderCategory (IgState *state);
    ~IgGLRenderCategory ();

    virtual void		browserChanged 	(Ig3DBaseBrowser *browser);    

private slots:
    //GL Render Action
    virtual void		setTransparencyType	(int type);
    virtual void		updateTransparencyType  (int type);
    virtual void		setRedrawPriority	(unsigned int priority);
    virtual void		setAntialiasingPasses	(unsigned int passes);
    virtual void		setAntialiasing		(bool option);
    virtual void		setAutoRedraw		(bool option);
    virtual void		setPassUpdate		(bool option);
    
private:
    void			addGLRenderTab 		(void);
    
    QComboBox			*m_transparencyType;
    IgQtRangeControlUInt	*m_redrawPriority;
    IgQtRangeControlUInt	*m_antialiasingPasses;
    QCheckBox			*m_antialiasing;
    QCheckBox			*m_autoRedraw;
    QCheckBox			*m_passUpdate;
};

#endif // IGUANA_GL_BROWSERS_IG_GL_RENDER_CATEGORY_H
