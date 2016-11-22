#include "Iguana/GLBrowsers/interface/IgIVView.h"
#include "Iguana/Framework/interface/IgArgsElement.h"
#include "Iguana/Framework/interface/IgBrowserDB.h"
#include "Iguana/Framework/interface/IgDocumentDataRoot.h"
#include "Iguana/Framework/interface/IgSelectionService.h"
#include "Iguana/GLBrowsers/interface/Ig3DBaseWindow.h"
#include "Iguana/GLModels/interface/IgSoTwig.h"
#include "Iguana/GLModels/interface/Ig3DBaseRep.h"
#include "Iguana/GLModels/interface/Ig3DModel.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Studio/interface/IgFileFetcher.h"
#include "Iguana/Studio/interface/IgModelManager.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Studio/interface/IgQtAppWindowService.h"
#include "Iguana/Studio/interface/IgViewDB.h"
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoGroup.h>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <iostream>

IgIVView::IgIVView (IgPage *page)
    :IgView (page),
     m_state (0),
     m_browser (0),
     m_bt (0)
{
    QString filename;
    QString workingDirectory ("./");
    QWidget *mainWindow = dynamic_cast<QMainWindow *> (IgQtAppWindowService::get (page->state ())->mainWindow ());
    
    IgArgsElement *args = IgArgsElement::get (page->state ());
    std::string fname;
    std::vector<std::string> arg;
    arg.push_back ("--ivfile");
    arg.push_back ("-iv");
    
    if (args->find (arg, fname, true)) filename = fname.c_str ();
    
    if (filename.isEmpty ())
        filename = QFileDialog::getOpenFileName (workingDirectory,
						 "Open Inventor Scenegraph (*.iv)",
						 mainWindow,
						 "Select Scenegraph to View");
    
    if (! filename.isEmpty ())
    {
        IgDocumentDataManager *dm = IgDocumentDataManager::get (page->state ());
        IgState *s =  dm->lookup (0, filename.latin1 ());
	
	if (s)
	{
	    // Do not open the same file gain 
	    IgDocumentDataRoot::currentRoot (filename.latin1 ());
	    return;
	}
	
	// Read in the file
        IgFileFetcher *fetcher = IgFileFetcher::get (page->state ());
	if (! fetcher)
	    fetcher = new IgFileFetcher (page->state ());
	
	std::string local = fetcher->fetch (filename.ascii ());
	if (local.empty ())
	{
	    QMessageBox::warning (mainWindow,
				  "File open error", 
				  "\""+ filename + "\" file not found.", "Ok");
	    return;
	}
	
	SoNode      *node = 0;
	SoInput     file;
	if (! file.openFile (local.c_str ()) 
	    || ! SoDB::read (&file, node) 
	    || ! node)
	{
	    QMessageBox::warning (mainWindow,
				  "File Access Error", 
				  "Can not open file \""+ filename + "\" for reading.", "Ok");
	    return;
        }
	IgDocumentDataRoot::currentRoot (filename.latin1 ());
	
	// Construct twigs from the node tree.  This always produces a
	// root that contains magic stuff to go into the browser's root
	// node, i.e. global magic clip planes etc.
	IgSoTwig    *twigs = IgSoTwig::buildFrom (node);
	dump (0, twigs);
	std::cout << std::flush;
	
	IgDocumentData::get (page->state ())->root (dynamic_cast<IgSoTwig*>(twigs->child(0)));
     
        m_state = new IgState (page->state ());
	new IgSelectionService (m_state);
	
        Ig3DModel *model = 0;
	IgModelManager::get (m_state)->lookup (m_state, &model);
	
	model->attachPoint ()->magic (twigs->magic ());
	model->attachPoint ()->node (twigs->node ());
	
	m_browser = dynamic_cast<Ig3DBaseWindow *> (IgViewDB::get ()->create ("3D Window", page ));
	
	m_bt = IgBrowserDB::get()->create ("Twig", m_state, page);
	m_bt->browse (twigs->child (0));
    } 
    else 
    {
        QMessageBox::warning (mainWindow,
			      "No file selected", 
			      "No OpenInventor file has been selected", "Ok");
    }
}

IgIVView::~IgIVView (void)
{
    if (m_browser)
	delete m_browser;
    if (m_bt)
	delete m_bt;
    if (m_state)
	delete m_state;
}

void
IgIVView::dump (unsigned level, IgTwig *twig)
{
    assert (twig);
    for (unsigned n = 0; n < twig->children (); ++n)
        dump (level + 2, twig->child (n));
}
