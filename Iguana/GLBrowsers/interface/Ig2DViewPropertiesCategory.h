#ifndef IGUANA_GL_BROWSERS_IG_2D_VIEW_PROPERTIES_CATEGORY_H
# define IGUANA_GL_BROWSERS_IG_2D_VIEW_PROPERTIES_CATEGORY_H

# include "Iguana/GLBrowsers/interface/IgControlCategory.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <QObject>

class Ig3DBaseBrowser;
class Ig3DBaseRep;
class IgQtAxisAlignment;
class IgState;

class  Ig2DViewPropertiesCategory 
  : public IgControlCategory,
    public IgStateElement
{
  Q_OBJECT
  IG_DECLARE_STATE_ELEMENT (Ig2DViewPropertiesCategory);
    
public:
  Ig2DViewPropertiesCategory (IgState *state,
			      Ig3DBaseBrowser *source);
				
  ~Ig2DViewPropertiesCategory (void);
    
  void		browserChanged  (Ig3DBaseBrowser *browser);

  virtual void	update (void);
  virtual void	apply (void);
  virtual void	registerBrowser (IgState *state,
				 Ig3DBaseBrowser *source);    
public slots:
  virtual void	setCuttingPlane (void);

private:
  Ig3DBaseBrowser	*m_source;
  Ig3DBaseRep		*m_root;

  IgQtAxisAlignment	*m_axisAlignment;

  // undefined semantics
  Ig2DViewPropertiesCategory (const Ig2DViewPropertiesCategory &);
  Ig2DViewPropertiesCategory &operator= (const Ig2DViewPropertiesCategory &);
};

#endif // IG_3D_BASE_BROWSER_IG_2D_VIEW_PROPERTIES_CATEGORY_H
