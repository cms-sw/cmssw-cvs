/****************************************************************************
**	VisCuTkSlWindow.cc
**
****************************************************************************/
#define QT_NO_EMIT
#include "VisReco/VisCustomTracker/interface/VisCuTracker.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSlWindow.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSelectionBar.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayerSelection.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkMap2D.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSubDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkPartDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayer.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkSubLayer.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkModule.h"
#include "VisReco/VisCustomTracker/interface/VisCuCmsTracker.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include <classlib/utils/DebugAids.h>

#include <QPushButton>
#include <QTabWidget>
#include <QLabel>
#include <QGridLayout>
#include <iostream>
#include <qobject.h>

/* 
 *  Constructs a CuTkSlWindow which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
VisCuTkSlWindow::VisCuTkSlWindow( QWidget* parent,  const char* name, Qt::WFlags fl ,IgRepresentable* igparent)
    : QWidget( parent, name, fl )
{
  if ( !name )
    setName( "VisCuTkSlWindow" );
  igp = igparent;
  //window size
  setMinimumSize( 550, 360 );
  //setMaximumSize( 550, 350 );
  
  setCaption( trUtf8( "Tracker parts selection window" ) );
  pset=VisConfigurationService::pSet();
  if(!pset)
    {
      edm::ParameterSet p;
      VisConfigurationService *visService = new VisConfigurationService(p);
      pset = visService->pSet();
      delete visService;
    } 
                                                                                
   
  mainGrid = new QGridLayout( this, 3, 1, 2 );
  QWidget* bGrid = new QWidget(this);
  
  tabWidget = new QTabWidget( this );
  
  selectionBar = new VisCuTkSelectionBar(this, tabWidget);        
  tabWidget->addTab( selectionBar, "&Layer/Ring" );
  
  QWidget *petal_rod = new QWidget( tabWidget );
  tabWidget->addTab( petal_rod, "&Petal/Rod" );
  QLabel * label1 = new QLabel( petal_rod, "notimplemented" );
  label1->setGeometry( QRect( 130, 160, 280, 24 ) );
  label1->setText( trUtf8( "Not yet implemented" ) );     
  
  labelinfop = new QLabel(this); 
  labelinfop->setFont(QFont("Times", 12));
  labelinfop->setText( trUtf8( " " ) );
  
  clear = new QPushButton( "clear", bGrid);
  clear->setFixedWidth( 50 );
  connect( clear, SIGNAL( clicked() ), this, SLOT( notifyEvent() ) );
  QPushButton * allt = new QPushButton( "all tracker", bGrid);
  allt->setFixedWidth( 80 );
  connect( allt, SIGNAL( clicked() ), this, SLOT( drawall() ) );
  QPushButton * pxb = new QPushButton( "PXB", bGrid);
  pxb->setFixedWidth( 50 );
  connect( pxb, SIGNAL( clicked() ), this, SLOT( pxbp() ) );
  QPushButton * pxe = new QPushButton( "PXE", bGrid);
  pxe->setFixedWidth( 50 );
  connect( pxe, SIGNAL( clicked() ), this, SLOT( pxep() ) );
  QPushButton * tib = new QPushButton( "TIB", bGrid);
  tib->setFixedWidth( 50 );
  connect( tib, SIGNAL( clicked() ), this, SLOT( tibp() ) );
  QPushButton * tob = new QPushButton( "TOB", bGrid);
  tob->setFixedWidth( 50 );
  connect( tob, SIGNAL( clicked() ), this, SLOT( tobp() ) );
  QPushButton * tid = new QPushButton( "TID", bGrid);
  tid->setFixedWidth( 50 );
  connect( tid, SIGNAL( clicked() ), this, SLOT( tidp() ) );
  QPushButton * tec = new QPushButton( "TEC", bGrid);
  tec->setFixedWidth( 50 );
  connect( tec, SIGNAL( clicked() ), this, SLOT( tecp() ) );
  crossed = new QPushButton( "Crossed", bGrid);
  crossed->setFixedWidth( 80 );
  connect( crossed, SIGNAL( clicked() ), this, SLOT( crossedOnly() ) );
  //if(!crossedModulesOnly)crossed->setEnabled(false);
  
  mainGrid->addWidget( tabWidget, 0, 0 );
  mainGrid->addWidget(labelinfop, 1, 0,Qt::AlignCenter );
  mainGrid->addWidget( bGrid, 2, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
VisCuTkSlWindow::~VisCuTkSlWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

void VisCuTkSlWindow:: setTracker(VisCuCmsTracker *tr)
{
   tracker = tr;
   selectionBar->setTracker(tracker);
   ASSERT (tracker);
   cout << "tracker components " << tracker->components()<< endl;
}

void VisCuTkSlWindow:: setMap(VisCuTkMap2D *map)
{
   map2D = map;
   ASSERT (map2D);
}

void VisCuTkSlWindow:: update()
{

}

void VisCuTkSlWindow:: drawpart(VisCuTkState* object)
{
  object->setVisible();
  bool yn = object->isVisible();
  
  if(dynamic_cast<VisCuTkLayer*>(object)){
    VisCuTkLayer* lay = dynamic_cast<VisCuTkLayer*>(object);
    lay->setVisible(yn);
    cout << "Layer "<< lay->getId()<< " of "<< lay->getOwner()->getName()<< " selected" << endl;
    for(unsigned int i=0; i<lay->components(); i++){
      VisCuTkSubLayer* subly = lay->getComponent(i+1);
      subly->setVisible(yn);
      for(unsigned int j=0; j<subly->components(); j++){
        VisCuTkModule* module = subly->getComponent(j+1);
        module->setVisible(yn);
      }
    }
  }
  if(dynamic_cast<VisCuTkSubLayer*>(object)){
    VisCuTkSubLayer* subly = dynamic_cast<VisCuTkSubLayer*>(object);
    subly->setVisible(yn);
    cout << "Ring "<< subly->getId()<< " layer "<<subly->getOwner()->getId()<<" of "<< subly->getOwner()->getOwner()->getName()<< " selected" << endl;
    for(unsigned int i=0; i<subly->components(); i++){
      VisCuTkModule* module = subly->getComponent(i+1);
      module->setVisible(yn);
    }
  }
  if(dynamic_cast<VisCuTkModule*>(object)){
    VisCuTkModule* module = dynamic_cast<VisCuTkModule*>(object) ;
    cout << "Module " << module->getId() << " ring "<<module->getOwner()->getId()<<" in " << module->getOwner()->getOwner()->getOwner()->getName() << " "<< module->getOwner()->getOwner()->getId()<< endl;
  }
  repaintAll();
}
void VisCuTkSlWindow::repaintAllBut3D()
{
  map2D->repaint();
  for(unsigned int i=0; i<tracker->components(); i++){
    VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);
    for(unsigned int j=0; j<subDetector->components(); j++ ){
      VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
      for(unsigned int k=0; k<partDetector->components(); k++ ){
        VisCuTkLayer* layer = partDetector->getComponent(k+1);
        if(layer->getSlWindow()!=0 )layer->getSlWindow()->layerSelection->repaint();
        }
      }
    }

  selectionBar->repaint();

}
void VisCuTkSlWindow:: repaintAll()
{
    repaintAllBut3D();
  IgRepSet::invalidate (igp, IgTwig::SELF_MASK);
}

void VisCuTkSlWindow:: notifyEvent()
{
  for(unsigned int i=0; i<tracker->components(); i++){
    VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);
    for(unsigned int j=0; j<subDetector->components(); j++ ){
      VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
      for(unsigned int k=0; k<partDetector->components(); k++ ){       
        VisCuTkLayer* layer = partDetector->getComponent(k+1);
        layer->setVisible(false);
        for(unsigned int l=0; l<layer->components(); l++ ){
          VisCuTkSubLayer* sublayer = layer->getComponent(l+1);
          sublayer->setVisible(false);
          for(unsigned int m=0; m<sublayer->components(); m++ ){
            VisCuTkModule* module = sublayer->getComponent(m+1);
	    module->setVisible(false);
	  }
	}
      }
    }
  }                                                        
  repaintAll();
  labelinfop->setText( trUtf8( " " ) );
}

void VisCuTkSlWindow:: crossedOnly()
{
 if(tracker->type==1){tracker->type=3;crossed->setText(trUtf8( "All" )); } else {tracker->type=1;crossed->setText(trUtf8( "Crossed" ));notifyEvent();}
 }
void VisCuTkSlWindow:: drawall()
{
  for(unsigned int i=0; i<tracker->components(); i++){
    VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);
    for(unsigned int j=0; j<subDetector->components(); j++ ){
      VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
      for(unsigned int k=0; k<partDetector->components(); k++ ){   
        VisCuTkLayer* layer = partDetector->getComponent(k+1);
        layer->setVisible(true);
        for(unsigned int l=0; l<layer->components(); l++ ){
          VisCuTkSubLayer* sublayer = layer->getComponent(l+1);
          sublayer->setVisible(true);
          for(unsigned int m=0; m<sublayer->components(); m++ ){
            VisCuTkModule* module = sublayer->getComponent(m+1);
	    module->setVisible(true);
	  }
	}
      }
    }
  }                                                        
  repaintAll();
  labelinfop->setText( trUtf8( " " ) );
  labelinfop->setText( trUtf8( "All tracker selected" ) );
}

void VisCuTkSlWindow:: pxep()
{
  for(unsigned int i=0; i<tracker->components(); i++){
    VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);
    for(unsigned int j=0; j<subDetector->components(); j++ ){
      VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
      for(unsigned int k=0; k<partDetector->components(); k++ ){  
	nlay=VisCuTracker::nlayer(i+1,j+1,k+1);
        if(nlay>12 && nlay<19 ){
        VisCuTkLayer* layer = partDetector->getComponent(k+1);
        layer->setVisible(true);
        for(unsigned int l=0; l<layer->components(); l++ ){
          VisCuTkSubLayer* sublayer = layer->getComponent(l+1);
          sublayer->setVisible(true);
	    for(unsigned int m=0; m<sublayer->components(); m++ ){
	      VisCuTkModule* module = sublayer->getComponent(m+1);
	      module->setVisible(true);
	    }
	  }
	}
      }
    }
  }                                                        
  repaintAll();
  labelinfop->setText( trUtf8( "Pixel endcap selected" ) );
}

void VisCuTkSlWindow:: pxbp()
{
  for(unsigned int i=0; i<tracker->components(); i++){
    VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);
    for(unsigned int j=0; j<subDetector->components(); j++ ){
      VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
      for(unsigned int k=0; k<partDetector->components(); k++ ){  
	nlay=VisCuTracker::nlayer(i+1,j+1,k+1);
        if(nlay>30 && nlay<34){
        VisCuTkLayer* layer = partDetector->getComponent(k+1);
        layer->setVisible(true);
        for(unsigned int l=0; l<layer->components(); l++ ){
          VisCuTkSubLayer* sublayer = layer->getComponent(l+1);
          sublayer->setVisible(true);
	    for(unsigned int m=0; m<sublayer->components(); m++ ){
	      VisCuTkModule* module = sublayer->getComponent(m+1);
	      module->setVisible(true);
	    }
	  }
	}
      }
    }
  }                                                        
  repaintAll();
  labelinfop->setText( trUtf8( "Pixel barrel selected" ) );
}

void VisCuTkSlWindow:: tidp()
{
  for(unsigned int i=0; i<tracker->components(); i++){
    VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);
    for(unsigned int j=0; j<subDetector->components(); j++ ){
      VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
      for(unsigned int k=0; k<partDetector->components(); k++ ){  
	nlay=VisCuTracker::nlayer(i+1,j+1,k+1);
	if((nlay>18 && nlay<22 )|| (nlay>9 && nlay<13)){
        VisCuTkLayer* layer = partDetector->getComponent(k+1);
        layer->setVisible(true);
        for(unsigned int l=0; l<layer->components(); l++ ){
          VisCuTkSubLayer* sublayer = layer->getComponent(l+1);
          sublayer->setVisible(true);
	    for(unsigned int m=0; m<sublayer->components(); m++ ){
	      VisCuTkModule* module = sublayer->getComponent(m+1);
	      module->setVisible(true);
	    }
	  }
	}
      }
    }
  }                                                        
  repaintAll();
  labelinfop->setText( trUtf8( "Inner disks selected" ) );
}

void VisCuTkSlWindow:: tibp()
{
  for(unsigned int i=0; i<tracker->components(); i++){
    VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);
    for(unsigned int j=0; j<subDetector->components(); j++ ){
      VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
      for(unsigned int k=0; k<partDetector->components(); k++ ){  
	nlay=VisCuTracker::nlayer(i+1,j+1,k+1);
        if(nlay>33 && nlay<38){
        VisCuTkLayer* layer = partDetector->getComponent(k+1);
        layer->setVisible(true);
        for(unsigned int l=0; l<layer->components(); l++ ){
          VisCuTkSubLayer* sublayer = layer->getComponent(l+1);
          sublayer->setVisible(true);
	    for(unsigned int m=0; m<sublayer->components(); m++ ){
	      VisCuTkModule* module = sublayer->getComponent(m+1);
	      module->setVisible(true);
	    }
	  }
	}
      }
    }
  }                                                        
  repaintAll();
  labelinfop->setText( trUtf8( "Inner Barrel selected" ) );
}

void VisCuTkSlWindow:: tecp()
{
  for(unsigned int i=0; i<tracker->components(); i++){
    VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);
    for(unsigned int j=0; j<subDetector->components(); j++ ){
      VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
      for(unsigned int k=0; k<partDetector->components(); k++ ){  
	nlay=VisCuTracker::nlayer(i+1,j+1,k+1);
        if(nlay<10 || (nlay>21 && nlay<31)){
        VisCuTkLayer* layer = partDetector->getComponent(k+1);
        layer->setVisible(true);
        for(unsigned int l=0; l<layer->components(); l++ ){
          VisCuTkSubLayer* sublayer = layer->getComponent(l+1);
          sublayer->setVisible(true);
	    for(unsigned int m=0; m<sublayer->components(); m++ ){
	      VisCuTkModule* module = sublayer->getComponent(m+1);
	      module->setVisible(true);
	    }
	  }
	}
      }
    }
  }                                                        
  repaintAll();
  labelinfop->setText( trUtf8( "Endcap selected" ) );
}

void VisCuTkSlWindow:: tobp()
{
  for(unsigned int i=0; i<tracker->components(); i++){
    VisCuTkSubDetector* subDetector = tracker->getComponent(i+1);
    for(unsigned int j=0; j<subDetector->components(); j++ ){
      VisCuTkPartDetector* partDetector = subDetector->getComponent(j+1);
      for(unsigned int k=0; k<partDetector->components(); k++ ){  
	nlay=VisCuTracker::nlayer(i+1,j+1,k+1);
        if(nlay>37){
        VisCuTkLayer* layer = partDetector->getComponent(k+1);
        layer->setVisible(true);
        for(unsigned int l=0; l<layer->components(); l++ ){
          VisCuTkSubLayer* sublayer = layer->getComponent(l+1);
          sublayer->setVisible(true);
	    for(unsigned int m=0; m<sublayer->components(); m++ ){
	      VisCuTkModule* module = sublayer->getComponent(m+1);
	      module->setVisible(true);
	    }
	  }
	}
      }
    }
  }                                                        
  repaintAll();
  labelinfop->setText( trUtf8( "Outer Barrel selected" ) );
}


