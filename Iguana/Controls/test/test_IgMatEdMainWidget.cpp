#include "Iguana/Controls/interface/IgMatEdMainWidget.h"
#include <Inventor/Qt/SoQt.h>
#include <QGridLayout>

int main (int argc, char **argv)
{
  QWidget *mainwin = SoQt::init(argc, argv, argv[0]);

  IgMatEdMainWidget  *materialEditor = new IgMatEdMainWidget(mainwin);    

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(materialEditor);
  mainwin->setLayout(layout);
  
  SoQt::show(mainwin);
  materialEditor->setCurrentG4Material("Air");
  SoQt::mainLoop();

  return 0;
}
