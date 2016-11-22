#include "Iguana/GLBrowsers/interface/Ig3DMColorControl.h"
#include "Iguana/GLBrowsers/interface/IgControlCategory.h"
#include "Iguana/GLBrowsers/interface/IgControlCentre.h"
#include "Iguana/GLBrowsers/interface/IgStudioCentre.h"
#include <Inventor/fields/SoMFColor.h>
#include <QColorDialog>
#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include <QHBoxLayout>

Ig3DMColorControl::Ig3DMColorControl (IgControlCategory *pane,
				    QString label /* = "Color" */)
    : Ig3DFieldControl (pane),
      m_value  (0),
      m_color  (0),
      m_indexs (0),
      m_colors (1, SbColor (1., 1., 1.)),
      m_redSlider (0),
      m_greenSlider (0),
      m_blueSlider (0)
{
    ASSERT (pane);
    ASSERT (pane->bodyArea ());
    ASSERT (pane->bodyLayout ());

    QWidget	*area = pane->bodyArea ();
    int		row = gridRows ();

    ASSERT (row >= 0);

    // Add a new row to the grid, label on the left and the value on the right
    QWidget 	*vlab = makeIndentedLabel (label, area);
    addGridWidget (vlab, row, 0);

    m_value = new QWidget(area);
    QHBoxLayout *l = new QHBoxLayout (m_value, 0, 5);
    
    m_indexs = new QComboBox (m_value);
    l->addWidget (m_indexs);
    m_indexs->insertItem ("0");
    m_indexs->setCurrentItem (0);
    m_indexs->hide ();

    m_redSlider =   new QSlider (0, 255, 1, 255, Qt::Horizontal, m_value);
    m_redSlider->setFixedSize (QSize (40, 8));
    m_redSlider->setPaletteBackgroundColor (QColor (255, 0, 0));
    l->addWidget (m_redSlider);
    
    m_greenSlider = new QSlider (0, 255, 1, 255, Qt::Horizontal, m_value);
    m_greenSlider->setFixedSize (QSize (40, 8));
    m_greenSlider->setPaletteBackgroundColor (QColor (0, 255, 0));
    l->addWidget (m_greenSlider);
    
    m_blueSlider =  new QSlider (0, 255, 1, 255, Qt::Horizontal, m_value);
    m_blueSlider->setFixedSize (QSize (40, 8));
    m_blueSlider->setPaletteBackgroundColor (QColor (0, 0, 255));
    l->addWidget (m_blueSlider);
    
    QPushButton *colorBase = new QPushButton (m_value);
    colorBase->setFixedSize (QSize (20, 20));
    
    QHBoxLayout *valueLayout =  new QHBoxLayout(colorBase, 6, 0);
    m_color = new QWidget (colorBase);
    valueLayout->addWidget (m_color);
    l->addWidget (colorBase);
        
    addGridWidget (makeSpacedBox (area, m_value), row, 1);

    // Link the button press signal to popping up a color dialog
    connect (m_indexs,	SIGNAL (activated   (int)),
    	     this, 	SLOT   (newIndex    (int)));
    connect (colorBase, SIGNAL (clicked     ()), 
             this,      SLOT   (chooseColor ()));
    connect (this,      SIGNAL (editable    (bool)), 
             m_value,   SLOT   (setEnabled  (bool)));
    connect (this,      SIGNAL (editable    (bool)), 
             vlab,      SLOT   (setEnabled  (bool)));
    connect (m_redSlider, SIGNAL (valueChanged (int)),
	     this,     SLOT (redSliderChanged (int)));
    connect (m_greenSlider, SIGNAL (valueChanged (int)),
	     this,     SLOT (greenSliderChanged (int)));
    connect (m_blueSlider, SIGNAL (valueChanged (int)),
	     this,     SLOT (blueSliderChanged (int)));
}

//////////////////////////////////////////////////////////////////////
SoMFColor *
Ig3DMColorControl::field (void) const
{ return static_cast<SoMFColor *> (Ig3DFieldControl::field ()); }

void
Ig3DMColorControl::setField (SoMFColor *field)
{ Ig3DFieldControl::setField (field); }

const SbColor*
Ig3DMColorControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValues (0); }

QWidget*
Ig3DMColorControl::widget(void) const
{ return m_value; }

//////////////////////////////////////////////////////////////////////
void
Ig3DMColorControl::chooseColor (void)
{
    IgControlCentre *centre = 
      IgStudioCentre::get (parentCategory ()->state ())->controlCentre ();
    QColor color
	= QColorDialog::getColor (QColor (m_redSlider->value (),
					  m_greenSlider->value (),
					  m_blueSlider->value ()),
				  centre, "Choose color");
    if (color.isValid ())
	updateColor (color.red () / 255.,
		     color.green () / 255.,
		     color.blue () / 255.);
}

const SbColor*
Ig3DMColorControl::value (void) const
{ return &(m_colors[0]); }

void
Ig3DMColorControl::resize (unsigned int size)
{
  ASSERT (size);
  int sizeDiff = size - m_colors.size ();
  if (sizeDiff == 0) return;

  m_colors.resize  (size);
  if (sizeDiff > 0)
  {
    for (unsigned int i = size - sizeDiff; i < size; i++)
      m_indexs->insertItem (QString::number (i));
  }
  else {
    for (unsigned int i = size - sizeDiff - 1 ; i >= size; i--)
      m_indexs->removeItem (i);
  }

  if (size > 1)
    m_indexs->show ();
  else
    m_indexs->hide ();
}

void
Ig3DMColorControl::setValue (unsigned int size, const SbColor* color)
{
  int index = m_indexs->currentItem ();
  if (index < 0)
    index = 0;
  
  resize (size);
  for (unsigned int i = 0; i < size; i++)
    m_colors [i] = color [i];

  m_indexs->setCurrentItem (index);
  updateColor (color[index][0], color[index][1], color[index][2]);
}

void
Ig3DMColorControl::updateColor (float r, float g, float b)
{
    m_colors[m_indexs->currentItem ()] = SbColor (r, g, b);
    newIndex (m_indexs->currentItem ());
    setDirty ();
}

void
Ig3DMColorControl::updateValue (void)
{ setValue (static_cast<SoMFColor *>(field())->getNum (), fieldValue ()); }

void
Ig3DMColorControl::applyValue (void)
{ field ()->setValues (0, int(m_colors.size ()), value ()); }

void
Ig3DMColorControl::newIndex (int index)
{
    SbColor color = m_colors[index];
    disconnect (m_redSlider, SIGNAL (valueChanged (int)),
	        this,     SLOT (redSliderChanged (int)));
    disconnect (m_greenSlider, SIGNAL (valueChanged (int)),
	        this,     SLOT (greenSliderChanged (int)));
    disconnect (m_blueSlider, SIGNAL (valueChanged (int)),
	        this,     SLOT (blueSliderChanged (int)));

    int red = int(color[0] * 255);
    m_redSlider->setValue (red);
    
    int green = int(color[1] * 255);
    m_greenSlider->setValue (green);
    
    int blue = int(color[2] * 255);
    m_blueSlider->setValue (blue);

    m_color->setPaletteBackgroundColor (QColor (red, green, blue));

    connect (m_redSlider, SIGNAL (valueChanged (int)),
	     this,     SLOT (redSliderChanged (int)));
    connect (m_greenSlider, SIGNAL (valueChanged (int)),
	     this,     SLOT (greenSliderChanged (int)));
    connect (m_blueSlider, SIGNAL (valueChanged (int)),
	     this,     SLOT (blueSliderChanged (int)));
}

void
Ig3DMColorControl::redSliderChanged (int value)
{
    updateColorWidget ();
}

void
Ig3DMColorControl::greenSliderChanged (int value)
{
    updateColorWidget ();
}

void
Ig3DMColorControl::blueSliderChanged (int value)
{
    updateColorWidget ();
}

void
Ig3DMColorControl::updateColorWidget (void)
{
    m_colors[m_indexs->currentItem ()] = SbColor (m_redSlider->value () / 255.,
                                                  m_greenSlider->value () / 255.,
						  m_blueSlider->value () / 255.);
						  
    m_color->setPaletteBackgroundColor (QColor (m_redSlider->value (),
                                                m_greenSlider->value (),
						m_blueSlider->value ()));
    setDirty ();
}
