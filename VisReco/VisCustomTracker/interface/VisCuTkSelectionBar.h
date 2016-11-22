#ifndef VIS_CU_TK_SELECTION_BAR_H
#define VIS_CU_TK_SELECTION_BAR_H

#include <qwidget.h>
//Added by qt3to4:
#include <QPaintEvent>
#include <QMouseEvent>
#include "VisReco/VisCustomTracker/interface/VisCuCmsTracker.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkState.h"
//#include "VisReco/VisCustomTracker/interface/VisCuTkObserver.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayerSlWindow.h"
#include <qlabel.h>   
#include <qstring.h>

class VisCuTkSlWindow;

class VisCuTkSelectionBar: public QWidget
{

public:
    VisCuTkSelectionBar(VisCuTkSlWindow* sw, QWidget *parent=0, const char *name=0);
    void drawSelectionBars();
    void setTracker(VisCuCmsTracker * tr);
    void update();
    VisCuTkSlWindow* gpp;
protected:
    void paintEvent(QPaintEvent* );	
    void mousePressEvent(QMouseEvent *e);

private:
    VisCuCmsTracker * tracker;
    QString label, label1;
    VisCuTkLayerSlWindow *layerWindow;
    QRect rsqr[400];
    VisCuTkState* state[400]; 
    int nelem;
    int ntotlay;
    double xmin, xmax, ymin, ymax;
    int xpixel(double x){
      return(int)((x-xmin)/(xmax-xmin)*530);
    }
    int ypixel(double y){
      double y1;
      y1 = (y-ymin)/(ymax-ymin);
      return 250-(int)(y1*250);
    }
    QSizePolicy sizePolicy() const;
};
#endif // VIS_CU_TK_SELECTION_BAR_H
