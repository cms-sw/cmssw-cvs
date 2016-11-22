#ifndef IG3DMATERIAL_EDITOR_IG_MAT_ED_SLIDER_H
# define IG3DMATERIAL_EDITOR_IG_MAT_ED_SLIDER_H

# include "Iguana/Controls/interface/config.h"
# include <QWidget>

class QLabel;
class QSlider;
class IgMatEdMainWidget;

class IG3DMATERIAL_EDITOR_API IgMatEdSlider : public QWidget
{
  Q_OBJECT
public:
  IgMatEdSlider(QWidget *parent, const char *name, float *target, IgMatEdMainWidget *mainWidget);
  void 		setValue(int value);    
  void 		setValue(float value);    
  void 		updatePosition(int value);
  void 		updatePosition(float value);

public slots:
  void 		updateValue(int newValue);
  
private:
  QLabel 	*m_label;
  QSlider 	*m_slider;
  float 	*m_target;    
  IgMatEdMainWidget *m_mainWidget;    
  QLabel 	*m_count;    
  bool 		m_updatePosition;    
};

#endif // IG3DMATERIAL_EDITOR_IG_MAT_ED_SLIDER_H
