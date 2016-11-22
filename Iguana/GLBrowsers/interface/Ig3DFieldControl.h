#ifndef IGUANA_GL_BROWSERS_IG_3DFIELD_CONTROL_H
# define IGUANA_GL_BROWSERS_IG_3DFIELD_CONTROL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/GLBrowsers/interface/config.h"
# include "Iguana/GLBrowsers/interface/IgControlBase.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoField;
class SoFieldSensor;
class SoSensor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API Ig3DFieldControl : public IgControlBase
{
    Q_OBJECT
public:
    Ig3DFieldControl (IgControlCategory *pane);
    ~Ig3DFieldControl (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    void		clearField (void);
    virtual void	attach (void);
    virtual void	detach (void);

public slots:
    virtual void	fieldSettingsChanged (void);
    
signals:
    void		settingsChanged (SoField*);
    void		settingsChanged (void);
    
protected:
    virtual bool	hasValue (void) const;

    void		setField (SoField *field);
    SoField *		field (void) const;

protected:
    static void		fieldChanged (void *data, SoSensor *sensor);

    SoField		*m_field;
    SoFieldSensor	*m_sensor;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_3DFIELD_CONTROL_H
