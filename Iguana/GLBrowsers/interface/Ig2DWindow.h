#ifndef IGUANA_GL_BROWSERS_IG_2D_WINDOW_H
# define IGUANA_GL_BROWSERS_IG_2D_WINDOW_H

# include "Iguana/GLBrowsers/interface/Ig3DBaseWindow.h"

class IgPage;
class Ig2DViewPropertiesCategory;
class Ig2DModel;

class Ig2DWindow : public Ig3DBaseWindow
{
  Q_OBJECT
public:
  Ig2DWindow (IgPage *page);
  ~Ig2DWindow (void);
  // implicit copy constructor
  // implicit assignment operator
protected:
  virtual void initMenu (void);
  virtual void initCategories (void);    
  virtual void initToolBar (void);
    
  Ig2DViewPropertiesCategory	*m_2DViewPropertiesCategory;

private slots:
  virtual void show2DViewProperties (void);    

private:
  Ig2DModel	*m_2dmodel;    
};

#endif // IGUANA_GL_BROWSERS_IG_2D_WINDOW_H
