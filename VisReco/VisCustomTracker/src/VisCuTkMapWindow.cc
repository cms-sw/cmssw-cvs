/****************************************************************************
**	VisCuTkMapWindow.cc
**
****************************************************************************/
#define QT_NO_EMIT
#include "VisReco/VisCustomTracker/interface/VisCuTkMapWindow.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkScrollView.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerSimHit.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerDigi.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerCluster.h"
#include "VisReco/VisCustomTracker/interface/VisCuTrackerRecHit.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include <classlib/utils/DebugAids.h>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <iostream>

using namespace edm;

/* 
 *  Constructs a VisCuTkMapWindow which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
VisCuTkMapWindow::VisCuTkMapWindow( QWidget* parent,  const char* name, Qt::WFlags fl,IgRepresentable* igparent,VisCuTrackerSimHit *simhitdraw,VisCuTrackerDigi *digidraw,VisCuTrackerCluster *clusterdraw,VisCuTrackerRecHit *rechitdraw)
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "VisCuTkMapWindow" );
    igp = igparent;
    resize( 630, 630 ); 
    setCaption( trUtf8( "2D Visualisation of Tracker" ) );

    QGridLayout *mainGrid = new QGridLayout( this, 4, 1, 2 );
     
    VisCuTkScrollView* sv = new VisCuTkScrollView(this);
    sv->setGeometry( QRect( 0, 1, 600, 600 ) );
    map = new VisCuTkMap2D(sv->widget(),"Tracker Map",simhitdraw, digidraw, clusterdraw, rechitdraw); 
    map->setGeometry( QRect( 0, 1, 600, 600 ) );
    map->setParent(this );
    sv->setWidget(map);

    simhitdraw->setMap(map);
    digidraw->setMap(map);
    clusterdraw->setMap(map);
    rechitdraw->setMap(map);

    initbGrid();
    inittGrid(); 
    
    drawallf=false;
    separatef=false;
    drawsimhits=false;
    drawdigi=false;
    drawcluster=false;
    drawrechit=false;
    acc=false;
    
    mainGrid->addWidget( toolbar,0 , 0 );
    mainGrid->addWidget( selectedModuleInfo,1 , 0 );
    mainGrid->addWidget( sv, 2, 0 );
    mainGrid->addWidget( bGrid, 3, 0 );
}

void VisCuTkMapWindow:: initbGrid()
{
  bGrid = new QWidget(this);
  PushButton1 = new QPushButton( bGrid, "PushButton1" );
  PushButton1->setText( trUtf8( "all tracker" ) );
  connect( PushButton1, SIGNAL(clicked()), this, SLOT( drawall()));

  PushButton2 = new QPushButton( bGrid, "PushButton2" );
  PushButton2->setText( trUtf8( "separate modules" ) );
  connect( PushButton2, SIGNAL(clicked()), this, SLOT( separate()));

  PushButton5 = new QPushButton( bGrid, "PushButton5" );
  PushButton5->setText( trUtf8( "IntegratedSignal" ) );
  connect( PushButton5, SIGNAL(clicked()), this, SLOT( integSign()));

  PushButton3 = new QPushButton( bGrid, "PushButton3" );
  PushButton3->setText( trUtf8( "SimHits" ) );
  connect( PushButton3, SIGNAL(clicked()), this, SLOT( drawSimHits()));
  
  PushButton4 = new QPushButton( bGrid, "PushButton4" );
  PushButton4->setText( trUtf8( "Digi" ) );
  connect( PushButton4, SIGNAL(clicked()), this, SLOT( drawDigi()));
 
  PushButton6 = new QPushButton( bGrid, "PushButton6" );
  PushButton6->setText( trUtf8( "Cluster" ) );
  connect( PushButton6, SIGNAL(clicked()), this, SLOT( drawCluster()));
 
  PushButton7 = new QPushButton( bGrid, "PushButton7" );
  PushButton7->setText( trUtf8( "RecHit" ) );
  connect( PushButton7, SIGNAL(clicked()), this, SLOT( drawRechit()));
 
}

void VisCuTkMapWindow:: inittGrid()
{
  toolbar = new QToolBar(this);
  toolbar->setFixedWidth( 180 );
  printButton = new QToolButton(QPixmap(QString::fromUtf8(":/icons/fileprint.xpm")), "print", QString::null,map,SLOT(print()), toolbar ,0 );
  printButton = new QToolButton(QPixmap(QString::fromUtf8(":/icons/filesave.xpm")), "save", QString::null,map,SLOT(save()), toolbar ,0 );
  zoominButton = new QToolButton(QPixmap(QString::fromUtf8(":/icons/zoom_plus.xpm")), "zoomIn", QString::null,map,SLOT(zoomIn()), toolbar ,0 );
  zoominButton->setEnabled(true);
  zoomoutButton = new QToolButton(QPixmap(QString::fromUtf8(":/icons/zoom_minus.xpm")), "zoomOut", QString::null,map,SLOT(zoomOut()), toolbar ,0 );
  zoomoutButton->setEnabled(true);
  selectedModuleInfo = new QLabel(this);
  selectedModuleInfo->setText( trUtf8( "selected module:" ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
VisCuTkMapWindow::~VisCuTkMapWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

void VisCuTkMapWindow:: setTracker(VisCuCmsTracker *tr)
{
   tracker = tr;
   map->setTracker(tracker);
   ASSERT (tracker);
}

void VisCuTkMapWindow:: update()
{
}

void VisCuTkMapWindow:: setEvent(const edm::Event& iEvent,const edm::EventSetup &eventSetup)
{
  map->setEvent(iEvent,eventSetup);
  map->repaint();
}

void VisCuTkMapWindow:: drawall()
{
  if(drawallf){
    drawallf=false;
    PushButton1->setText( trUtf8( "all tracker" ) );
    zoominButton->setEnabled(true);
    zoomoutButton->setEnabled(true);
  }else {
    drawallf=true;
    PushButton1->setText( trUtf8( "single layer" ) );
    zoominButton->setEnabled(false);
    zoomoutButton->setEnabled(false);
  }  
  map-> drawAllTracker();
}

void VisCuTkMapWindow:: separate()
{
    if(separatef)separatef=false;else separatef=true;
    if(separatef)PushButton2->setText( trUtf8( "overlayed modules" ) );
      else PushButton2->setText( trUtf8( "separate modules" ) );
    map->separateModules();
}

void VisCuTkMapWindow:: drawSimHits()
{
  if(drawsimhits)drawsimhits=false;else drawsimhits=true;
  if(drawsimhits)PushButton3->setText( trUtf8( "no SimHits" ) );
  else PushButton3->setText( trUtf8( "drawSimHits" ) );
  map-> drawSimHits();
  IgRepSet::invalidate (igp, IgTwig::SELF_MASK);
}

void VisCuTkMapWindow:: drawDigi() 
{
  if(drawdigi)drawdigi=false;else drawdigi=true;
  if(drawdigi)PushButton4->setText( trUtf8( "no Digi" ) );  
  else PushButton4->setText( trUtf8( "drawDigi" ) );    
  map-> drawDigi(); 
  IgRepSet::invalidate (igp, IgTwig::SELF_MASK);
}
void VisCuTkMapWindow:: drawCluster() 
{
  if(drawcluster)drawcluster=false;else drawcluster=true;
  if(drawcluster)PushButton6->setText( trUtf8( "no Cluster" ) );  
  else PushButton6->setText( trUtf8( "drawCluster" ) );    
  map-> drawCluster(); 
  IgRepSet::invalidate (igp, IgTwig::SELF_MASK);
}
void VisCuTkMapWindow:: drawRechit() 
{
  if(drawrechit)drawrechit=false;else drawrechit=true;
  if(drawrechit)PushButton7->setText( trUtf8( "no RecHit" ) );  
  else PushButton7->setText( trUtf8( "drawRecHit" ) );    
  map-> drawRechit(); 
  IgRepSet::invalidate (igp, IgTwig::SELF_MASK);
}

void VisCuTkMapWindow:: integSign()
{
  if(acc)acc=false;else acc=true;
  if(acc)PushButton5->setText( trUtf8( "Single Event" ) );
  else PushButton5->setText( trUtf8( "Integrated Signal" ) );
  map-> integratedSignal(acc);
}

void VisCuTkMapWindow:: pan(QPoint pos, int dx, int dy)
{
 map->pan(pos,dx,dy);
}
