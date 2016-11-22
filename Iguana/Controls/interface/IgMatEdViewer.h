#ifndef IG3DMATERIAL_EDITOR_IG_MAT_ED_VIEWER_H
# define IG3DMATERIAL_EDITOR_IG_MAT_ED_VIEWER_H

# include "Iguana/Controls/interface/config.h"
# include <QWidget>

class SoSeparator;
class SoOrthographicCamera;
class SoDirectionalLight;
class SoQtRenderArea;
class SoNode;

class IG3DMATERIAL_EDITOR_API IgMatEdViewer : public QWidget
{
public:
  IgMatEdViewer(QWidget *parent, SoNode *model);
private:
  QWidget 			*m_parent;    
  SoQtRenderArea 		*m_renderArea;    
  SoDirectionalLight		*m_light;    
  SoOrthographicCamera		*m_camera;    
  SoSeparator			*m_root;
  SoSeparator			*m_userSeparator;
  SoNode			*m_model;    
};

#endif // IG3DMATERIAL_EDITOR_IG_MAT_ED_VIEWER_H
