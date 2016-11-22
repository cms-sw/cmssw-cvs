//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/Ig3DPlaneControl.h"
#include "Iguana/GLBrowsers/interface/Ig3DFloatControl.h"
#include "Iguana/GLBrowsers/interface/IgControlCategory.h"
#include "Iguana/GLBrowsers/interface/IgQtRangeControl.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DPlaneControl::Ig3DPlaneControl (IgControlCategory *pane, QString label)
    : Ig3DFieldControl (pane)
{
    ASSERT (pane);
    ASSERT (pane->bodyArea ());
    ASSERT (pane->bodyLayout ());
	m_direction = new IgPlaneDirection();

    QWidget *area = pane->bodyArea ();

    // Add a new row to the grid: just the label for this category
    QWidget 	*vlab = makeIndentedLabel (label, area);
    addGridWidget (vlab, gridRows (), 0);

    // Now add subitems for direction and distance
    pane->pushIndent ();

	// create a rotation control with 3 thumbwheels and 3 alignment buttons
	m_rotation = new Ig3DAxisRotationControl (pane, "Rotation", this);
    
    m_distance = new Ig3DFloatControl (pane, "Distance",
					IgQtRangeControl::StepperPart
					| IgQtRangeControl::ValuePart
					| IgQtRangeControl::SliderPart);    
    pane->popIndent ();
	
	m_distance->control()->setRange (-30., 30.);

    // Always reflect the state of the real value controls and vice versa.
    connect (m_direction, SIGNAL(dirty()), this, SLOT(setDirty()));

    connect (m_distance,  SIGNAL(dirty()), this, SLOT(setDirty()));
    connect (m_distance,  SIGNAL(settingsChanged (void)),
             this,        SLOT  (emitSettingsChanged (void)));

    connect (this, SIGNAL(clean()), m_distance,  SLOT(setClean()));
    connect (this, SIGNAL(editable(bool)),m_distance, SLOT(setEditable(bool)));
    connect (this, SIGNAL(editable(bool)),vlab,       SLOT(setEnabled(bool)));
}

//////////////////////////////////////////////////////////////////////
SoSFPlane *
Ig3DPlaneControl::field (void) const
{ return static_cast<SoSFPlane *> (Ig3DFieldControl::field ()); }

void
Ig3DPlaneControl::setField (SoSFPlane *field)
{ Ig3DFieldControl::setField (field); }

SbPlane
Ig3DPlaneControl::fieldValue (void) const
{ ASSERT (field ()); return field ()->getValue (); }

//////////////////////////////////////////////////////////////////////
SbPlane
Ig3DPlaneControl::value (void) const
{ return SbPlane (direction (), distance ()); }

SbVec3f
Ig3DPlaneControl::direction (void) const
{ return m_direction->value (); }

float
Ig3DPlaneControl::distance (void) const
{ return m_distance->value (); }

void
Ig3DPlaneControl::setValue (const SbPlane &newval)
{
    setDirection (newval.getNormal ());
    setDistance (newval.getDistanceFromOrigin ());
}

void
Ig3DPlaneControl::setDirection (const SbVec3f &newval)
{ m_direction->setValue (newval); }

void
Ig3DPlaneControl::setDistance (float newval)
{ m_distance->setValue (newval); }

void
Ig3DPlaneControl::updateValue (void)
{ setValue (fieldValue ()); }

void
Ig3DPlaneControl::applyValue (void)
{ field ()->setValue (value ()); }

//////////////////////////////////////////////////////////////////////
void
Ig3DPlaneControl::show (void)
{
    Ig3DFieldControl::show ();
    m_distance->show ();
}

void
Ig3DPlaneControl::hide (void)
{
    Ig3DFieldControl::hide ();
    m_distance->hide ();
}
//////////////////////////////////////////////////////////////////////

Ig3DAxisRotationControl*
Ig3DPlaneControl::axisRotationControl (void) const
{ return m_rotation; }

Ig3DFloatControl*
Ig3DPlaneControl::distanceControl  (void) const
{ return m_distance; }

QString
Ig3DPlaneControl::getSettingString (void) const
{ 
  return m_distance->control ()->getSettingString ();
}
    
void
Ig3DPlaneControl::setSettingString (QString &setting)
{
  m_distance->control ()->setSettingString (setting);
}

void
Ig3DPlaneControl::emitSettingsChanged (void)
{ fieldSettingsChanged (); }
