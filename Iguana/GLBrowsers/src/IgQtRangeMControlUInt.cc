//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/IgQtRangeMControlUInt.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtRangeMControlUInt::IgQtRangeMControlUInt (QWidget *parent, int parts,
					      unsigned int value /*= 0*/,
					      const char *name /*= 0*/)

    : inherited  (parent, parts, value, name)
{ }

IgQtRangeMControlUInt::IgQtRangeMControlUInt (QWidget *parent, int parts,
					      unsigned int size,
					      const unsigned int* values,
					      const char *name /*= 0*/)
    : inherited  (parent, parts, size, values, name)
{ }

IgQtRangeMControlUInt::IgQtRangeMControlUInt (QWidget *parent, int parts,
					      const rangeMValues& values,
					      const char *name /*= 0*/)
    : inherited  (parent, parts, values, name)
{ }

void
IgQtRangeMControlUInt::emitSignals (void)
{
    emit newValue (value ());
    emit valueChanged (valueChange ());
}
