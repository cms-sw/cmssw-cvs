#ifndef IG3DMATERIAL_EDITOR_IG_MAT_ED_COLOR_BOX_H
# define IG3DMATERIAL_EDITOR_IG_MAT_ED_COLOR_BOX_H

# include "Iguana/Controls/interface/config.h"
# include <Inventor/fields/SoMFColor.h> 
# include <QWidget>

class QLabel;
class IgMatEdSlider;
class IgMatEdMainWidget;

class IG3DMATERIAL_EDITOR_API IgMatEdColorBox : public QWidget
{
public:
  IgMatEdColorBox(QWidget *parent, const char *name, float *colorArray, IgMatEdMainWidget *mainWidget);
  void 		setColor(SoMFColor &color);    
  void 		updatePosition(SoMFColor &color);    

private:
  float		*m_colorArray;
  QLabel 	*m_label;    
  IgMatEdMainWidget *m_mainWidget;
    
  IgMatEdSlider	*m_redSlider;
  IgMatEdSlider	*m_greenSlider;
  IgMatEdSlider	*m_blueSlider;    
};

#endif // IG3DMATERIAL_EDITOR_IG_MAT_ED_COLOR_BOX_H
