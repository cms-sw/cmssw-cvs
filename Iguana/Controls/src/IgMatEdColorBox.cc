#include "Iguana/Controls/src/IgMatEdColorBox.h"
#include "Iguana/Controls/src/IgMatEdSlider.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>

IgMatEdColorBox::IgMatEdColorBox(QWidget *parent, const char *name, float *colorArray, IgMatEdMainWidget *mainWidget)
  :QWidget(parent),
   m_colorArray(colorArray),
   m_mainWidget(mainWidget)
{
  assert(m_colorArray);
  assert(m_mainWidget);    
    
  m_label = new QLabel (name, this);
    
  QWidget *hbox = new QWidget(this); 
  QFrame *colorFrame = new QFrame;
  colorFrame->setFrameStyle (QFrame::Panel);
  QWidget *vbox = new QWidget(this); 

  /*Sliders*/
  m_redSlider = new IgMatEdSlider (this, "Red", &m_colorArray[0], m_mainWidget);    
  m_greenSlider = new IgMatEdSlider (this, "Green", &m_colorArray[1], m_mainWidget);    
  m_blueSlider = new IgMatEdSlider (this, "Blue", &m_colorArray[2], m_mainWidget);

  QVBoxLayout *vlayout = new QVBoxLayout;
  vlayout->addWidget(m_redSlider);
  vlayout->addWidget(m_greenSlider);
  vlayout->addWidget(m_blueSlider);
  vbox->setLayout(vlayout);

  QHBoxLayout *hlayout = new QHBoxLayout;
  hlayout->addWidget(vbox);
  hlayout->addWidget(colorFrame);
  hbox->setLayout(hlayout);    

  QVBoxLayout *toplayout = new QVBoxLayout;
  toplayout->addWidget(hbox);
  this->setLayout(toplayout);
}

void
IgMatEdColorBox::setColor ( SoMFColor &color)
{
  m_redSlider->setValue ((color[0])[0]);
  m_greenSlider->setValue ((color[0])[1]);
  m_blueSlider->setValue ((color[0])[2]);    
}

void
IgMatEdColorBox::updatePosition (SoMFColor &color)
{
  m_redSlider->updatePosition ((color[0])[0]);
  m_greenSlider->updatePosition ((color[0])[1]);
  m_blueSlider->updatePosition ((color[0])[2]);
}
