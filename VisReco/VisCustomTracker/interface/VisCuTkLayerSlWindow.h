/****************************************************************************
**	VisCuTkLayerSlWindow.h
**
****************************************************************************/
#ifndef VIS_CU_TK_LAYER_SL_WINDOW_H
#define VIS_CU_TK_LAYER_SL_WINDOW_H

#include <QWidget>

class QGridLayout;
class QLabel;
class VisCuTkLayerSelection;
class VisCuTkState;
class VisCuTkLayer;
class VisCuTkSelectionBar;

class VisCuTkLayerSlWindow : public QWidget
{
    Q_OBJECT

public:
    VisCuTkLayerSlWindow( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0, VisCuTkState* state = 0 ,VisCuTkSelectionBar* sb = 0);
    ~VisCuTkLayerSlWindow();

    QGridLayout *gridLayer; 
    QLabel * labelinfo;
    VisCuTkSelectionBar *qw;
    VisCuTkLayerSelection* layerSelection; 

    void update();

public slots:
    void notifyEvent();
private slots:
 void closeWindow();

private:
 VisCuTkLayer* layer;
};
#endif // VIS_CU_TK_LAYER_SL_WINDOW_H
