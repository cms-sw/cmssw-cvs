#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_CONTROL_UINT_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_CONTROL_UINT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeControlImp.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgQtRangeControlUInt : public IgQtRangeControlImp<unsigned int>
{
    Q_OBJECT
public:
    IgQtRangeControlUInt (QWidget *parent, int parts, unsigned int value = 0,
		          const char *name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    unsigned int	stringToNumber (const QString& val) const
    { return val.toUInt (); }
    
signals:
    void		valueChanged (unsigned int delta);
    void		newValue (unsigned int newval);

protected:
    void 		emitSignals (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_CONTROL_UINT_H
