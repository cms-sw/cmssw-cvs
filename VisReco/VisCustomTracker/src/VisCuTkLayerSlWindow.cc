/****************************************************************************
**	VisCuTkLayerSlWindow.cc
**
****************************************************************************/

#include "VisReco/VisCustomTracker/interface/VisCuTkLayerSlWindow.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayer.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkPartDetector.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkLayerSelection.h"
#include "VisReco/VisCustomTracker/interface/VisCuTkState.h"
#include <QGridLayout>
#include <QLabel>
#include <string>
#include <iostream>
#include <sstream>


using namespace std;   
/* 
 *  Constructs a VisCuTkSlWindow which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
VisCuTkLayerSlWindow::VisCuTkLayerSlWindow( QWidget* parent, const char* name, Qt::WFlags fl , VisCuTkState* state, VisCuTkSelectionBar* sb)
    : QWidget( parent, name, fl )
{
  if ( !name )
    setName( "VisCuTkLayerSlWindow" );
  
  qw = sb; 
  layer=0;
  if(dynamic_cast<VisCuTkLayer*>(state))
    layer = dynamic_cast<VisCuTkLayer*>(state);

  //layer=state;

  //window size
  setMinimumSize( 400, 400 );
  setMaximumSize( 400, 400 );
  VisCuTkLayer* lay = dynamic_cast<VisCuTkLayer*>(state);
  ostringstream os(ostringstream::out);
  os << lay->getOwner()->getName() << "  "<<lay->getId()<<" modules selection window";
  const QString s ( QString::fromStdString (os.str()));
  setCaption( s );   
  
  layerSelection = new VisCuTkLayerSelection(this,"layer",state);

  gridLayer = new QGridLayout( this, 2, 1, 0 );

  labelinfo = new QLabel(this);
  // labelinfo->setGeometry( QRect(50 ,375, 280, 18 ) );
  labelinfo->setText( trUtf8( " " ) );
  
  gridLayer->addWidget( layerSelection, 0, 0 );
  gridLayer->addWidget( labelinfo, 1, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
VisCuTkLayerSlWindow::~VisCuTkLayerSlWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

void VisCuTkLayerSlWindow:: closeWindow()
{
    if(layer!=0)layer->setSlWindow(0);	
}
void VisCuTkLayerSlWindow:: update()
{
	
}

void VisCuTkLayerSlWindow:: notifyEvent()
{
}
