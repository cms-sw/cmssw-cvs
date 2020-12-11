//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/Ig3DAngleControl.h"
#include "Iguana/GLBrowsers/interface/Ig3DRotationControl.h"
#include <Inventor/fields/SoSFFloat.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DAngleControl::Ig3DAngleControl (IgControlCategory *pane, QString label,
				    int parts /* = IgQtRangeControl::ValuePart */)
    : Ig3DFloatControl (pane, label, parts)
{}

//////////////////////////////////////////////////////////////////////
void
Ig3DAngleControl::updateValue (void)
{ setValue (Ig3DRotationControl::rad2deg (fieldValue ())); }

void
Ig3DAngleControl::applyValue (void)
{ field ()->setValue (Ig3DRotationControl::deg2rad (value ())); }
