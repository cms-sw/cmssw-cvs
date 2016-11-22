#ifndef IGUANA_STUDIO_IG_QT_SCROLL_VIEW_H
# define IGUANA_STUDIO_IG_QT_SCROLL_VIEW_H

# include "Iguana/Studio/interface/config.h"
# include <QScrollArea>

class IGUANA_STUDIO_API IgQtScrollView : public QScrollArea
{
  Q_OBJECT
public:
  IgQtScrollView(QWidget * parent = 0, const char * name = 0, Qt::WFlags f = 0);
};

#endif // IGUANA_STUDIO_IG_QT_SCROLL_VIEW_H
