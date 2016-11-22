/****************************************************************************
**	VisCuTkSlWindow.h
**
****************************************************************************/
#ifndef VIS_CU_TK_SL_WINDOW_H
#define VIS_CU_TK_SL_WINDOW_H

# include <QWidget>
# include "VisFramework/VisConfigService/interface/VisConfigurationService.h"

class QGridLayout; 
class QPushButton;
class QTabWidget;
class QLabel;
class VisCuTkSelectionBar;
class VisCuCmsTracker;
class VisCuTkMap2D;
class VisCuTkState;
class IgRepresentable;

class VisCuTkSlWindow : public QWidget
{
    Q_OBJECT

public:
    VisCuTkSlWindow( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0,IgRepresentable* igparent=0 );
    ~VisCuTkSlWindow();
    QGridLayout *mainGrid; 
    QLabel * labelinfop; 
    void update();
    void setTracker(VisCuCmsTracker * tr);
    void setMap(VisCuTkMap2D* map);
    void drawpart(VisCuTkState* state);
    void repaintAll();
    void repaintAllBut3D();
    bool crossedModulesOnly;

public slots:
    void notifyEvent();
    void drawall();
    void pxep();
    void pxbp();
    void tibp();
    void tobp();
    void tidp();
    void tecp();
    void crossedOnly();

private:
    const edm::ParameterSet *pset;
    VisCuCmsTracker* tracker;
    VisCuTkMap2D* map2D;
    VisCuTkSelectionBar* selectionBar;
    QTabWidget* tabWidget;
    QPushButton *clear;
    QPushButton *crossed;
    IgRepresentable* igp;
    
    int nlay;
};
#endif // VIS_CU_TK_SL_WINDOW_H
