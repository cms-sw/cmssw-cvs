#include "Iguana/Controls/interface/IgMatEdMainWidget.h"
#include "Iguana/Controls/src/IgMatEdSlider.h"
#include "Iguana/Controls/src/IgMatEdColorBox.h"
#include "Iguana/Controls/src/IgMatEdModificationAction.h"
#include "Iguana/Controls/interface/IgMatEdViewer.h"
#include "Iguana/Controls/interface/IgSoMaterialMap.h"
#include "Iguana/Controls/interface/IgG4ToSoMaterialMap.h"

#include <iostream>
#include <fstream>

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QComboBox>
#include <QLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QFrame>

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SoOutput.h>

IgMatEdMainWidget::IgMatEdMainWidget(QWidget *parent)
  : QWidget(parent),
    m_g4ListView(0),
    m_viewer(0),
    m_rootNode(0)
{    
  QWidget *vbox = new QWidget;
  QWidget *hbox = new QWidget;
  QVBoxLayout *vlayout = new QVBoxLayout;    
  vbox->setLayout(vlayout);

  QHBoxLayout *hlayout = new QHBoxLayout;
  hbox->setLayout(hlayout);
    
  // Create a listview with all the materials
  m_g4ListView = new QTreeWidget(hbox);
  hlayout->addWidget(m_g4ListView);
  hbox->setLayout(hlayout);

  QStringList headers;
  headers << "G4 material" << "OIV material";
  m_g4ListView->setHeaderLabels(headers);

  m_g4ToSoMap = IgG4ToSoMaterialMapSingleton::instance();

  for(IgG4ToSoMaterialMap::MaterialMap::iterator i = m_g4ToSoMap->begin(), iEnd = m_g4ToSoMap->end(); i != iEnd; ++i)
  {
    QTreeWidgetItem *item = new QTreeWidgetItem(m_g4ListView);
    item->setText(0, QString::fromStdString(i->first));
    item->setText(1, QString::fromStdString(i->second));	
  }        
    
  //Makes the sphere scenegraph.
  m_rootNode = new SoSeparator;
  m_rootNode->ref ();

  m_soMaterialMap = IgSoMaterialMapSingleton::instance();
    
  assert(m_soMaterialMap);
  assert((*m_soMaterialMap)["default"]);
    
  m_rootNode->insertChild ((*m_soMaterialMap)["default"], 0);
  m_rootNode->insertChild (new SoSphere, 1);

  m_createMaterialButton = new QPushButton ("Create material", this);    
  connect (m_createMaterialButton, SIGNAL (clicked()), this, SLOT (createSoMaterial()));

  m_comboBox = new QComboBox;

  for (IgSoMaterialMap::MaterialMap::iterator i = m_soMaterialMap->begin (); 
       i != m_soMaterialMap->end (); 
       i++)
  {
    m_comboBox->insertItem (QString::fromStdString (i->first));	
  }
  connect (m_comboBox, SIGNAL (activated (const QString &)), this, SLOT (newMaterial (const QString &)));
  connect (m_comboBox, SIGNAL (highlighted (const QString &)), this, SLOT (newMaterial (const QString &)));

  QWidget *soMaterialBox = new QWidget(this); 
  QHBoxLayout *soMaterialBoxLayout = new QHBoxLayout;
  soMaterialBoxLayout->addWidget(m_createMaterialButton);
  soMaterialBoxLayout->addWidget(m_comboBox);
  soMaterialBox->setLayout(soMaterialBoxLayout);    

  connect (m_g4ListView, SIGNAL (currentItemChanged (QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT (newG4Material (QTreeWidgetItem *)));
    
  m_shininessSlider = new IgMatEdSlider(this, "Shininess", &IgMatEdModificationAction::m_shininess, this);
  m_transparencySlider = new IgMatEdSlider(this, "Transparency", &IgMatEdModificationAction::m_transparency, this);
  QWidget *sliderBox = new QWidget;
  QVBoxLayout *sliderBoxLayout = new QVBoxLayout;
  sliderBoxLayout->addWidget(m_shininessSlider);
  sliderBoxLayout->addWidget(m_transparencySlider);
  sliderBox->setLayout(sliderBoxLayout);
    
  m_diffuseColorBox = new IgMatEdColorBox(this, "Diffuse", IgMatEdModificationAction::m_diffuseColor, this);
  m_ambientColorBox = new IgMatEdColorBox(this, "Ambient", IgMatEdModificationAction::m_ambientColor, this);
  m_specularColorBox = new IgMatEdColorBox(this, "Specular", IgMatEdModificationAction::m_specularColor, this);
  m_emissiveColorBox = new IgMatEdColorBox(this, "Emissive", IgMatEdModificationAction::m_emissiveColor, this);
  QWidget *colorBox = new QWidget;
  QHBoxLayout *colorBoxLayout = new QHBoxLayout;
  colorBoxLayout->addWidget(m_diffuseColorBox);
  colorBoxLayout->addWidget(m_ambientColorBox);
  colorBoxLayout->addWidget(m_specularColorBox);
  colorBoxLayout->addWidget(m_emissiveColorBox);
  colorBox->setLayout(colorBoxLayout);
    
  // Use one of the convenient SoQt viewer classes.
  QFrame *dummyFrame = new QFrame;
  dummyFrame->setMinimumSize (200,200);
  m_viewer = new IgMatEdViewer(dummyFrame, m_rootNode);
  
  QWidget *viewerBox = new QWidget;
  QHBoxLayout *viewerBoxLayout = new QHBoxLayout;
  viewerBoxLayout->addWidget(hbox);
  viewerBoxLayout->addWidget(dummyFrame);
  viewerBox->setLayout(viewerBoxLayout);
      
  QGridLayout *toplayout = new QGridLayout;
  toplayout->addWidget(viewerBox);
  toplayout->addWidget(soMaterialBox);
  toplayout->addWidget(sliderBox);
  toplayout->addWidget(colorBox);
  this->setLayout(toplayout);
  this->show();
}

IgMatEdMainWidget::~IgMatEdMainWidget(void)
{
  assert(m_viewer);
  assert(m_rootNode);
  assert(m_g4ListView);
    
  m_rootNode->unref(); 
    
  delete m_viewer;    
  delete m_g4ListView;    
}

void
IgMatEdMainWidget::setCurrentG4Material(const QString &name)
{
  QList<QTreeWidgetItem *> items = m_g4ListView->findItems (name, Qt::MatchExactly, 0);
  //If the material name matches one of the list, then select it
  //in the list, otherwise create and select a new entry for it.
  if(items.empty())
  {
    QStringList names;
    names << name << "default";
    QTreeWidgetItem *item = new QTreeWidgetItem(m_g4ListView, names);	
    m_g4ListView->addTopLevelItem(item);
    (*m_g4ToSoMap)[name.ascii ()] = "default";
  }    
  m_g4ListView->setCurrentItem(items.first());    
  m_g4ListView->scrollToItem(items.first());
}

void
IgMatEdMainWidget::newG4Material(QTreeWidgetItem *item)
{    
  std::cerr << "newG4Material called" << std::endl;    
  newMaterial(item->text(1));    
  m_comboBox->setCurrentText(item->text(1));    
}

void 
IgMatEdMainWidget::newMaterial(const QString &text)
{
  std::cerr << "newMaterial called with argument " << text.toStdString() << std::endl;    
  assert(m_soMaterialMap);

  if(text != QString::null)
  {
    /*Set the new name for the SoMaterial*/
    assert(m_g4ListView);	
    assert(m_comboBox);
    QTreeWidgetItem *item = m_g4ListView->currentItem();
    item->setText(1, text);	

    /*Updates the G4->So mapping map*/
    (*m_g4ToSoMap) [item->text(0).ascii()] = text.ascii();		

    /*Updates the display material using the selected one*/
    SoMaterial *m_selectedMaterial = (*m_soMaterialMap) [text.ascii()];

    if(m_selectedMaterial == 0)
    {
      QMessageBox::warning(0, "Warning!",
			   "Open Inventor material " + text
			   +" not found. Using a default one for it.", "OK");
      m_selectedMaterial = new SoMaterial;
      m_soMaterialMap->addEntry(text.ascii(), m_selectedMaterial);
    }	

    assert(m_selectedMaterial);
    m_rootNode->removeChild(0);
    m_rootNode->insertChild(m_selectedMaterial, 0);
    std::cerr << m_selectedMaterial->ambientColor[0][0] << " "
	      << m_selectedMaterial->ambientColor[0][1] << " "
	      << m_selectedMaterial->ambientColor[0][2] << std::endl;
	
    /*Updates the position of the sliders*/
    assert(m_diffuseColorBox);
    assert(m_ambientColorBox);
    assert(m_specularColorBox);
    assert(m_emissiveColorBox);
    assert(m_transparencySlider);
    assert(m_shininessSlider);
	
    m_diffuseColorBox->updatePosition(m_selectedMaterial->diffuseColor);    
    m_ambientColorBox->updatePosition(m_selectedMaterial->ambientColor);
    m_specularColorBox->updatePosition(m_selectedMaterial->specularColor);
    m_emissiveColorBox->updatePosition(m_selectedMaterial->emissiveColor);
    m_transparencySlider->updatePosition((int)(m_selectedMaterial->transparency[0]*100));
    m_shininessSlider->updatePosition((int)(m_selectedMaterial->shininess[0]*100));
  }
}

void 
IgMatEdMainWidget::saveMaterialMap(void)
{
  //Get material map name, use default one "material.map" if not set.
  std::string filename;
  char *fname = getenv("MATERIAL_CONFIG");    
        
  if(fname == 0)
  {
    filename = "./materials.map";	
  }
  else
  {
    filename = fname;    
  }
    
  std::ofstream output;
  output.open(filename.c_str());

  if(output)
  {
    iterateTreeWidget(output);
    output.close();    
  }
}

void
IgMatEdMainWidget::iterateTreeWidget(std::ofstream &output, QTreeWidgetItem *parent)
{
  int count = parent ? parent->childCount() : m_g4ListView->topLevelItemCount();

  for(int i = 0; i < count; ++i)
  {
    QTreeWidgetItem *item = parent ? parent->child(i) : m_g4ListView->topLevelItem(i);	
    output << item->text(0).toStdString() << "= " << item->text(1).toStdString() << std::endl;

    iterateTreeWidget(output, item);
  }
}

void
IgMatEdMainWidget::saveSoMaterials(void)
{
  SoOutput out;
  std::string filename;
    
  char *fn = getenv ("SO_MATERIALS_IV");        
  if (!fn)
    filename = "./materials.iv";
  else
    filename = fn;
    
  out.openFile(filename.c_str());
  SoWriteAction writer(&out);
  for (IgSoMaterialMap::MaterialMap::iterator i = m_soMaterialMap->begin (), iEnd = m_soMaterialMap->end (); 
       i != iEnd; 
       ++i)
  {
    writer.apply(i->second);
  }
}

void
IgMatEdMainWidget::createSoMaterial(void)
{
  bool ok;    
  QString text = QInputDialog::getText(
    "Create new material", "Enter material name:", QLineEdit::Normal,
    QString::null, &ok, this);
  if(text != QString::null)
  {
    SoMaterial *entry = new SoMaterial;
    entry->ref();
    entry->setName(text.ascii());

    m_soMaterialMap->addEntry(text.ascii(), entry);	
    m_comboBox->insertItem(text);
  }    
}
