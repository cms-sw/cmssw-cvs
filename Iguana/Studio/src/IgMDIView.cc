#include "Iguana/Studio/interface/IgMDIView.h"
#include "Iguana/Studio/interface/IgViewDB.h"
#include "Iguana/Studio/interface/IgPage.h"
#include "Iguana/Studio/interface/IgView.h"
#include "Iguana/Studio/interface/IgDocumentData.h"
#include "Iguana/Framework/interface/IgTwig.h"
#include "Iguana/Framework/interface/IgBrowser.h"
#include "Iguana/Framework/interface/IgBrowserDB.h"
#include <QMdiArea>

IgMDIView::IgMDIView (IgPage *page, const char *window /*= 0*/)
{
  IgBrowser *twig = IgBrowserDB::get()->create ("Twig", page->state (),
						page);
  IgBrowser *text = IgBrowserDB::get()->create ("Text", page->state (),
						page);
  QString name ("3D Window");
  if (window)
    name = window;
  IgBrowser *browser = dynamic_cast<IgBrowser*>(
    IgViewDB::get()->create (name.latin1 (), page ));
    
  IgRepresentable *obj = IgDocumentData::get (page->state ())->root ();
  browser->browse (obj);
  twig->browse (obj);
  text->browse (obj);

  page->workspace()->tileSubWindows();
}
