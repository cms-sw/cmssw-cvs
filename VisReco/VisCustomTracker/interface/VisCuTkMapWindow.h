/****************************************************************************
**	VisCuTkMapWindow.h
**
****************************************************************************/
#ifndef VIS_CU_TK_MAP_WINDOW_H
#define VIS_CU_TK_MAP_WINDOW_H

#include "VisReco/VisCustomTracker/interface/VisCuTkMap2D.h"
#include "VisReco/VisCustomTracker/interface/VisCuCmsTracker.h"
#include <QWidget>

namespace edm 
{
    class Event;
}

class QVBoxLayout; 
class QHBoxLayout;
class QGridLayout; 
class QFrame;
class QTextBrowser;
class QLabel;
class QPushButton;
class QScrollBar;
class QGrid;
class QToolBar;
class QToolButton;
class IgRepresentable;
class VisCuTrackerSimHit;
class VisCuTrackerDigi;
class VisCuTrackerCluster;
class VisCuTrackerRecHit;

class VisCuTkMapWindow : public QWidget
{
    Q_OBJECT
      
      public:
    VisCuTkMapWindow( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0,IgRepresentable* igparent=0,VisCuTrackerSimHit *simhitdraw=0, VisCuTrackerDigi *digidraw=0,VisCuTrackerCluster *clusterdraw=0,VisCuTrackerRecHit *rechitdraw=0 );
    ~VisCuTkMapWindow();
    void update();
    void setTracker(VisCuCmsTracker * tr);
    void setEvent(const edm::Event& iEvent,const edm::EventSetup &eventSetup);
    void pan(QPoint pos, int dx, int dy);
    VisCuTkMap2D* map;
    bool drawsimhits;
    bool drawdigi;
    bool drawcluster;
    bool drawrechit;
    QLabel* selectedModuleInfo;
    
 protected:
    void initbGrid();
    void inittGrid();
    
    public slots:
      void drawall();
    void separate();
    void drawSimHits();
    void drawDigi();
    void drawCluster();
    void drawRechit();
    void integSign();

 private:
    QToolBar *toolbar;
    QGridLayout *mainGrid;

    QWidget* bGrid;
    QToolButton* printButton;
    QToolButton* zoominButton;
    QToolButton* zoomoutButton;
    QPushButton* PushButton1;
    QPushButton* PushButton2;
    QPushButton* PushButton3;
    QPushButton* PushButton4;
    QPushButton* PushButton5;
    QPushButton* PushButton6;
    QPushButton* PushButton7;
    QScrollBar* ScrollBar1;
    IgRepresentable* igp;
    
    int numModifiche;
    bool drawallf;
    bool separatef;
    bool acc;
    VisCuCmsTracker * tracker;
};
#endif // CU_TK_MAP_WINDOW_H
