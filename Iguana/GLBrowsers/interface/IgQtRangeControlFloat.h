#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_CONTROL_FLOAT_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_CONTROL_FLOAT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/IgQtRangeControl.h"
# include "Iguana/GLBrowsers/interface/IgQtRangeValueFloat.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgQtRangeControlFloat : public IgQtRangeControl, public IgQtRangeValueFloat
{
    Q_OBJECT
public:
    typedef IgQtRangeValueFloat inherited;
    
    IgQtRangeControlFloat (QWidget *parent, int parts, float value = 0,
		           const char *name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void	setPrecision (float precision);
    
    float		stringToNumber (const QString& val) const
    { return val.toFloat (); }
    
    using IgQtRangeControl::setDirty;
    float		getSmallStep (void) const;
    float		getLongStep (void) const;

    void		setSmallStep (float size);
    void		setLongStep (float size);

    void		setRange (float min, float max);

    void		setValue (float newval);

    void		revMin (void);
    void		revLong (void);
    void		revSmall (void);
    void		fwdSmall (void);
    void		fwdLong (void);
    void		fwdMax (void);

    void		setDirty (bool dirty);
    void		editSettings (void);
    
    QString		getSettingString (void) const;
    void		setSettingString (QString &setting);
    
signals:
    void		valueChanged (float delta);
    void		newValue (float newval);

protected:
    void 		emitSignals (void);
    void		sliderChanged (int newval);
    void		textChanged (const QString &newval);
    virtual void	adjustSliderSteps (void);
    void		moveValue (float delta, MoveDirection dir);
    void		fwdEnable (bool enable);
    void		revEnable (bool enable);

    void		initialize (int parts);

    float		m_longStep;
    float		m_smallStep;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_CONTROL_FLOAT_H
