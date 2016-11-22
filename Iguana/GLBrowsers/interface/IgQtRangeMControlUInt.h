#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_MCONTROL_UINT_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_MCONTROL_UINT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMControlImp.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//////////////////////////////////////////////////////////////////////

class IGUANA_GL_BROWSERS_API IgQtRangeMControlUInt : public IgQtRangeMControlImp<unsigned int>
{
    Q_OBJECT
public:
    IgQtRangeMControlUInt (QWidget *parent, int parts,
                          unsigned int value = 0,
		          const char *name = 0);

    IgQtRangeMControlUInt (QWidget *parent, int parts,
                          unsigned int size,
		          const unsigned int* values,
		          const char *name = 0);

    IgQtRangeMControlUInt (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name = 0);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    unsigned int	stringToNumber (const QString& val) const
    { return val.toUInt(); }
    
signals:
    void		valueChanged (std::vector<unsigned int> delta);
    void		newValue (std::vector<unsigned int> newval);

protected:
    void 		emitSignals (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_MCONTROL_UINT_H
