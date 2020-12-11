#ifndef IG3DMATERIAL_EDITOR_IG_MAT_ED_MAIN_WIDGET_H
# define IG3DMATERIAL_EDITOR_IG_MAT_ED_MAIN_WIDGET_H

# include "Iguana/Controls/interface/config.h"
# include "Iguana/Controls/interface/IgSoMaterialMap.h"
# include <QWidget>

class QTreeWidget;
class QTreeWidgetItem;
class QComboBox;
class QPushButton;
class SoSeparator;
class SoMaterial;
class IgG4ToSoMaterialMap;
class IgMatEdSlider;
class IgMatEdColorBox;
class IgMatEdViewer;

class IG3DMATERIAL_EDITOR_API IgMatEdMainWidget : public QWidget
{
  Q_OBJECT   
public:
  IgMatEdMainWidget(QWidget *parent);
  ~IgMatEdMainWidget(void);

  void 			saveSoMaterials(void);
  void 			saveMaterialMap(void);

  IgSoMaterialMap	*m_soMaterialMap;
  QComboBox		*m_comboBox;    
  QTreeWidget 		*m_g4ListView;

public slots:
  void 			setCurrentG4Material(const QString &name);    
  void 			newMaterial(const QString &text);    
  void 			newG4Material(QTreeWidgetItem *item);
  void 			createSoMaterial(void);
  void 			iterateTreeWidget(std::ofstream &output, QTreeWidgetItem *parent = 0);
  
private:
  IgMatEdViewer		*m_viewer;    
  SoSeparator		*m_rootNode;
  QPushButton		*m_createMaterialButton;    
  IgG4ToSoMaterialMap	*m_g4ToSoMap;
  IgMatEdSlider		*m_shininessSlider;
  IgMatEdSlider		*m_transparencySlider;

  IgMatEdColorBox 	*m_diffuseColorBox;
  IgMatEdColorBox 	*m_ambientColorBox;
  IgMatEdColorBox 	*m_specularColorBox;
  IgMatEdColorBox 	*m_emissiveColorBox;
};

#endif // IG3DMATERIAL_EDITOR_IG_MAT_ED_MAIN_WIDGET_H
