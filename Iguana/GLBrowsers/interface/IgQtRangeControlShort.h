#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_CONTROL_SHORT_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_CONTROL_SHORT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeControlImp.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//////////////////////////////////////////////////////////////////////

class IGUANA_GL_BROWSERS_API IgQtRangeControlShort : public IgQtRangeControlImp<short>
{
    Q_OBJECT
public:
    IgQtRangeControlShort (QWidget *parent, int parts, short value = 0,
		           const char *name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    short		stringToNumber (const QString& val) const
    { return val.toShort (); }
    
signals:
    void		valueChanged (short delta);
    void		newValue (short newval);

protected:
    void 		emitSignals (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_CONTROL_SHORT_H
