#ifndef VIS_CU_TK_LAYER_SELECTION_H
#define VIS_CU_TK_LAYER_SELECTION_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QPolygon>
#include <QRegion>
#include "VisReco/VisCustomTracker/interface/VisCuTkState.h"
#include <cmath>
#include <iostream>

class VisCuTkLayer;
class VisCuTkModule;
class VisCuTkLayerSlWindow;

class VisCuTkLayerSelection: public QWidget
{

public:
    VisCuTkLayerSelection(QWidget *parent=0, const char *name=0, VisCuTkState* state=0);
    void update();
    VisCuTkLayerSlWindow* gp;
protected:
    void paintEvent(QPaintEvent* );	
    void mousePressEvent(QMouseEvent *e);
    void closeEvent( QCloseEvent* );

private:
    VisCuTkLayer* lay; 
    int nlay, x_win;
    int nelem;
    QPolygon rg[2000]; 
    VisCuTkState* state[2000]; 
    double xmin, xmax, ymin, ymax;
    int xpixel(double x){
      return(int)((x-xmin)/(xmax-xmin)*x_win);
    }
    int ypixel(double y){
      double y1;
      y1 = (y-ymin)/(ymax-ymin);
      return x_win -(int)(y1*x_win);
    }
    bool posrel, horizontal_view;
    void  defwindow(int nlay);
    void  drawmodule(VisCuTkModule * mod,int nlay,QPolygon& q);
    void  drawModules();
        
    QSizePolicy sizePolicy() const;
};
#endif // VIS_CU_TK_SELECTION_LAYER_H
