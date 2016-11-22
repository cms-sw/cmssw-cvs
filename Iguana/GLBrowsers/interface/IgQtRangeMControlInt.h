#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_MCONTROL_INT_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_MCONTROL_INT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeMControlImp.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//////////////////////////////////////////////////////////////////////

class IGUANA_GL_BROWSERS_API IgQtRangeMControlInt : public IgQtRangeMControlImp<int>
{
    Q_OBJECT
public:
    IgQtRangeMControlInt (QWidget *parent, int parts,
                          int value = 0,
		          const char *name = 0);

    IgQtRangeMControlInt (QWidget *parent, int parts,
                          unsigned int size,
		          const int* values,
		          const char *name = 0);

    IgQtRangeMControlInt (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name = 0);

    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    int			stringToNumber (const QString& val) const
    { return val.toInt(); }
    
signals:
    void		valueChanged (std::vector<int> delta);
    void		newValue (std::vector<int> newval);

protected:
    void 		emitSignals (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_MCONTROL_INT_H
