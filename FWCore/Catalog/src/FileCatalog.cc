//////////////////////////////////////////////////////////////////////
//
// $Id: FileCatalog.cc,v 1.7 2008/03/14 03:46:02 wmtan Exp $
//
// Original Author: Luca Lista
// Current Author: Bill Tanenbaum
//
//////////////////////////////////////////////////////////////////////

#include "FWCore/Catalog/interface/FileCatalog.h"

namespace edm {

  FileCatalog::FileCatalog(PoolCatalog & poolcat) :
      catalog_(poolcat.catalog_),
      url_(),
      active_(false) {
  }

  FileCatalog::~FileCatalog() {
    if (active_) catalog_.commit();
    catalog_.disconnect();
  }

  void FileCatalog::commitCatalog() {
    catalog_.commit();
    catalog_.start();
  }

}
