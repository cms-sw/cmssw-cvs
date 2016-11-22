#ifndef IGUANA_STUDIO_IG_QT_PIX_BUTTON_H
# define IGUANA_STUDIO_IG_QT_PIX_BUTTON_H

# include "Iguana/Studio/interface/config.h"
# include <QToolButton>

class QPixmap;

class IGUANA_STUDIO_API IgQtPixButton : public QToolButton
{
    Q_OBJECT
public:
    IgQtPixButton (QWidget *parent, const char *name = 0);
    IgQtPixButton (QPixmap pixmap, QWidget *parent, const char *name = 0);

protected:
    void	initialise (QPixmap pixmap);
};

#endif // IGUANA_STUDIO_IG_QT_PIX_BUTTON_H
