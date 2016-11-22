//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/config.h"
#include <Inventor/SoOutput.h> 
#include <Inventor/SoPickedPoint.h> 
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoLineHighlightRenderAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/Qt/SoQt.h>
#include <QtGui>
#include <QTimer>
#include <cstdlib>
#include <cmath>

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#ifdef WIN32
#define main IgOpenInventor_main
#endif

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

QString makeTest (SoSeparator *root);

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

int main (int argc, char **argv)
{
    QWidget		*mainWindow = SoQt::init (argc, argv, "IGUANA Shape Test");
    SoSelection		*top = new SoSelection;
    SoSeparator		*root = new SoSeparator;
    SoQtExaminerViewer	*viewer;

    top->policy = SoSelection::SINGLE;
    top->addChild (root);
    top->ref();

    SoMaterial *redPlastic = new SoMaterial;
    redPlastic->emissiveColor.setValue (1, 0, 0);
    redPlastic->diffuseColor.setValue (1, 0, 0);
    redPlastic->transparency = .5;
    root->addChild (redPlastic);
    
    QString title = makeTest (root);
    
    if (getenv ("IGUANA_DEBUG_SCENE"))
    {
	SoOutput out;    
	SoWriteAction wa (&out);
	wa.apply (root);
    }

    mainWindow->setGeometry (0, 0, 800, 600); // (0, 0, 400, 300);
    viewer = new SoQtExaminerViewer (mainWindow);
    viewer->setTitle (title.toStdString ().c_str ());
    viewer->setGLRenderAction (new SoLineHighlightRenderAction);
    viewer->redrawOnSelectionChange (top);
    viewer->setSceneGraph (top);
    viewer->viewAll ();
    viewer->show ();
    SoQt::show (mainWindow);

    QTimer::singleShot (10 * 60 * 1000, qApp, SLOT(quit()));
    SoQt::mainLoop ();
    delete viewer;
    top->unref ();
    return 0;
}
