#include "Iguana/GLBrowsers/interface/Ig3DColorControl.h"
#include "Iguana/GLBrowsers/interface/IgControlCategory.h"
#include "Iguana/GLBrowsers/interface/IgControlCentre.h"
#include "Iguana/GLBrowsers/interface/IgStudioCentre.h"
#include <QColorDialog>
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>

Ig3DColorControl::Ig3DColorControl (IgControlCategory *pane,
				    QString label /* = "Color" */)
    : Ig3DFieldControl (pane),
      m_value (0),
      m_color (0),
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
    connect (colorBase, SIGNAL (clicked     ()), 
             this,      SLOT   (chooseColor ()));
    connect (this,      SIGNAL (editable    (bool)), 
             vlab,      SLOT   (setEnabled  (bool)));
    connect (this,      SIGNAL (editable    (bool)), 
             m_value,   SLOT   (setEnabled  (bool)));
    connect (m_redSlider, SIGNAL (valueChanged (int)),
	     this,     SLOT (redSliderChanged (int)));
    connect (m_greenSlider, SIGNAL (valueChanged (int)),
	     this,     SLOT (greenSliderChanged (int)));
    connect (m_blueSlider, SIGNAL (valueChanged (int)),
	     this,     SLOT (blueSliderChanged (int)));
}

//////////////////////////////////////////////////////////////////////
SoSFColor *
Ig3DColorControl::field (void) const
{ return static_cast<SoSFColor *> (Ig3DFieldControl::field ()); }

void
Ig3DColorControl::setField (SoSFColor *field)
{ Ig3DFieldControl::setField (field); }

SbVec3f
Ig3DColorControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValue (); }

QWidget*
Ig3DColorControl::widget(void) const
{ return m_value; }

//////////////////////////////////////////////////////////////////////
void
Ig3DColorControl::chooseColor (void)
{
    IgControlCentre *centre = 
      IgStudioCentre::get (parentCategory ()->state ())->controlCentre ();
    QColor color
	= QColorDialog::getColor (QColor (m_redSlider->value (),
					  m_greenSlider->value (),
					  m_blueSlider->value ()),
				  centre, "Choose color");
    if (color.isValid ())
	setValue (color.red () / 255.,
		  color.green () / 255.,
		  color.blue () / 255.);
}

SbVec3f
Ig3DColorControl::value (void) const
{
    return SbVec3f (m_redSlider->value () / 255.,
                    m_greenSlider->value () / 255.,
		    m_blueSlider->value () / 255.);
}

void
Ig3DColorControl::setValue (const SbVec3f &rgb)
{ setValue (rgb [0], rgb [1], rgb [2]); }

void
Ig3DColorControl::setValue (float r, float g, float b)
{
    disconnect (m_redSlider, SIGNAL (valueChanged (int)),
	        this,     SLOT (redSliderChanged (int)));
    disconnect (m_greenSlider, SIGNAL (valueChanged (int)),
	        this,     SLOT (greenSliderChanged (int)));
    disconnect (m_blueSlider, SIGNAL (valueChanged (int)),
	        this,     SLOT (blueSliderChanged (int)));
    
    int red = int(r * 255);
    m_redSlider->setValue (red);
    
    int green = int(g * 255);
    m_greenSlider->setValue (green);
    
    int blue = int(b * 255);
    m_blueSlider->setValue (blue);
      
    m_color->setPaletteBackgroundColor (QColor (red, green, blue));
    
    connect (m_redSlider, SIGNAL (valueChanged (int)),
	     this,     SLOT (redSliderChanged (int)));
    connect (m_greenSlider, SIGNAL (valueChanged (int)),
	    this,     SLOT (greenSliderChanged (int)));
    connect (m_blueSlider, SIGNAL (valueChanged (int)),
	     this,     SLOT (blueSliderChanged (int)));
    setDirty ();
}

void
Ig3DColorControl::updateValue (void)
{ setValue (fieldValue ()); }

void
Ig3DColorControl::applyValue (void)
{ field ()->setValue (value ()); }

void
Ig3DColorControl::redSliderChanged (int value)
{
    updateColorWidget ();
}

void
Ig3DColorControl::greenSliderChanged (int value)
{
    updateColorWidget ();
}

void
Ig3DColorControl::blueSliderChanged (int value)
{
    updateColorWidget ();
}

void
Ig3DColorControl::updateColorWidget (void)
{
    m_color->setPaletteBackgroundColor (QColor (m_redSlider->value (),
                                                m_greenSlider->value (),
						m_blueSlider->value ()));
    setDirty ();
}
