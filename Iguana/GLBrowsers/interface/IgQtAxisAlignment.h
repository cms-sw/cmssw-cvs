#ifndef IGUANA_GL_BROWSERS_IG_QT_AXIS_ALIGNMENT_H
# define IGUANA_GL_BROWSERS_IG_QT_AXIS_ALIGNMENT_H

# include "Iguana/GLBrowsers/interface/IgControlItem.h"
# include "Iguana/GLBrowsers/interface/config.h"
# include <Inventor/SbLinear.h>

class QWidget;
class QGroupBox;
class QPushButton;

class IGUANA_GL_BROWSERS_API IgQtAxisAlignment
    : public IgControlItem
{
    Q_OBJECT
public:
    IgQtAxisAlignment (QWidget *parent, float min = 0., float max = 1.,
		       const char *name = 0);

    SbVec3f		value		(void) const;
    QWidget*		widget		()     const;

    float		minAxisAlign	(void) const;
    float		maxAxisAlign	(void) const;

    bool		xAlign		(void) const;
    bool		yAlign		(void) const;
    bool		zAlign		(void) const;

public slots:
    virtual void	setMinAxisAlign	(float newval);
    virtual void	setMaxAxisAlign	(float newval);
    virtual void	setAlign	(unsigned short axis);
    virtual void	setXAlign	(void);
    virtual void	setYAlign	(void);
    virtual void	setZAlign	(void);
        
private:
    QGroupBox		*m_widget;
    QPushButton		*m_xalign;
    QPushButton		*m_yalign;
    QPushButton		*m_zalign;
    unsigned short	m_alignAxis;
    float		m_min;
    float		m_max;
};

#endif // IGUANA_GL_BROWSERS_IG_QT_AXIS_ALIGNMENT_H
