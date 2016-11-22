#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_MCONTROL_LONG_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_MCONTROL_LONG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMControlImp.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//////////////////////////////////////////////////////////////////////

class IGUANA_GL_BROWSERS_API IgQtRangeMControlLong : public IgQtRangeMControlImp<long>
{
    Q_OBJECT
public:
    IgQtRangeMControlLong (QWidget *parent, int parts,
                          long value = 0,
		          const char *name = 0);

    IgQtRangeMControlLong (QWidget *parent, int parts,
                          unsigned int size,
		          const long* values,
		          const char *name = 0);

    IgQtRangeMControlLong (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name = 0);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    long		stringToNumber (const QString& val) const
    { return val.toLong(); }
    
signals:
    void		valueChanged (std::vector<long> delta);
    void		newValue (std::vector<long> newval);

protected:
    void 		emitSignals (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_MCONTROL_LONG_H
