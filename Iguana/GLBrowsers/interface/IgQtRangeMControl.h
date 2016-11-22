#ifndef IGUANA_GL_BROWSERS_IG_QT_RANGE_MCONTROL_H
# define IGUANA_GL_BROWSERS_IG_QT_RANGE_MCONTROL_H

# include "Iguana/GLBrowsers/interface/IgQtRangeControlCommon.h"

class QComboBox;

class IGUANA_GL_BROWSERS_API IgQtRangeMControl : public IgQtRangeControlCommon
{
    Q_OBJECT
public:
    IgQtRangeMControl (QWidget *parent, int parts,
                       unsigned int size = 1,
		       const char *name = 0);

    int			currentIndex (void) const;
    
    virtual void	resizeIndex (unsigned int size);

public slots:
    virtual void	newIndex (int index);

protected slots:
    virtual void	updateTextValue (void);

protected:

    QComboBox		*m_index;
    bool		m_indexPart;
};

#endif // IGUANA_GL_BROWSERS_IG_QT_RANGE_MCONTROL_H
