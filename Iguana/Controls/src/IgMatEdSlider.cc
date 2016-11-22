#include "Iguana/Controls/src/IgMatEdSlider.h"
#include "Iguana/Controls/interface/IgMatEdMainWidget.h"
#include "Iguana/Controls/src/IgMatEdModificationAction.h"
#include "Iguana/Controls/interface/IgSoMaterialMap.h"
#include <Inventor/nodes/SoNode.h> 
#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <iostream>

IgMatEdSlider::IgMatEdSlider(QWidget *parent, const char *name, float *target, IgMatEdMainWidget *mainWidget)
  :QWidget(parent),
   m_target(target),
   m_mainWidget(mainWidget),
   m_updatePosition(false)
{
  assert(m_target);
  assert(m_mainWidget);

  m_label = new QLabel(name,this);
  m_slider = new QSlider (0, 100, 1, 0, Qt::Horizontal, this);
  m_slider->setTickmarks (QSlider::TicksBelow);
  m_slider->setValue ((int) ((*target)*100));    
  m_slider->setTickInterval (10);
    
  m_count = new QLabel (QString::number ((*target)*100) + "%",this);
    
  /*Makes sure that labels and count are aligned*/
  m_label->setMinimumWidth (QApplication::fontMetrics ().width ("Green")+10);    
  m_count->setMinimumWidth (QApplication::fontMetrics ().width ("100%")+10);    

  connect (m_slider, SIGNAL (valueChanged (int)), this, SLOT (updateValue (int)));

  QWidget *sliderBox = new QWidget;
  QHBoxLayout *sliderBoxLayout = new QHBoxLayout;
  sliderBoxLayout->addWidget(m_label);
  sliderBoxLayout->addWidget(m_count);
  sliderBox->setLayout(sliderBoxLayout);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(sliderBox);
  layout->addWidget(m_slider);
  this->setLayout(layout);
}

void
IgMatEdSlider::updateValue (int newValue)
{
  *m_target = ((float) newValue)/100;
  m_count->setText (QString::number (newValue) + "%");    
  IgMatEdModificationAction *action = new IgMatEdModificationAction ();
  //action->apply ((SoNode *) m_mainWidget->m_currentMaterial);
  if (m_updatePosition == false)
  {
    action->apply ((SoNode *) (*(m_mainWidget->m_soMaterialMap))[m_mainWidget->m_comboBox->currentText ().toStdString ()]);        
  }	
}

void
IgMatEdSlider::setValue (int value)
{
  m_slider->setValue (value);
  updateValue (value);    
}

void
IgMatEdSlider::setValue (float value)
{
  m_slider->setValue ((int)(100*value));    
}

void 
IgMatEdSlider::updatePosition (int newValue)
{
  m_updatePosition = true;    
  *m_target = ((float) newValue)/100;
  m_count->setText (QString::number (newValue) + "%");    
  m_slider->setValue (newValue);    
  m_updatePosition = false;    
}

void
IgMatEdSlider::updatePosition (float newValue)
{
  m_updatePosition = true;    
  *m_target = newValue;
  m_count->setText (QString::number ((int) (100*newValue)) + "%");    
  m_slider->setValue ((int) (100*newValue));    
  m_updatePosition = false;    
}
