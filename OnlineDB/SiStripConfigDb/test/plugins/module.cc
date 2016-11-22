// Last commit: $Id: module.cc,v 1.4 2008/04/11 13:27:34 bainbrid Exp $

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_SEAL_MODULE();

#include "OnlineDB/SiStripConfigDb/test/plugins/testSiStripConfigDb.h"
DEFINE_ANOTHER_FWK_MODULE(testSiStripConfigDb);

