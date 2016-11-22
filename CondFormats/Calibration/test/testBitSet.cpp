#include <bitset>
#include <iostream>
#include <stdexcept>
#include "StorageSvc/DbType.h"
#include "StorageSvc/poolDb.h"
#include "DataSvc/IDataSvc.h"
#include "DataSvc/DataSvcFactory.h"
#include "FileCatalog/IFileCatalog.h"
#include "PersistencySvc/DatabaseConnectionPolicy.h"
#include "PersistencySvc/ISession.h"
#include "PersistencySvc/Placement.h"
#include "PersistencySvc/ITransaction.h"
#include "PersistencySvc/ISession.h"
#include "DataSvc/Ref.h"
#include "DataSvc/RefException.h"
#include "FileCatalog/IFileCatalog.h"
#include "CondFormats/Calibration/interface/BitArray.h"
int main(){
  std::bitset<7>* days=new std::bitset<7>;
  days->set(5);
  days->set(1);
  std::cout<<"bitfields "<<(*days)<<std::endl;
  pool::Placement place;
  place.setDatabase("sqlite_file:pippo.db", pool::DatabaseSpecification::PFN );
  place.setContainerName("Cont1");
  place.setTechnology(pool::POOL_RDBMS_HOMOGENEOUS_StorageType.type());
  pool::IFileCatalog* fileCatalog = new pool::IFileCatalog;
  fileCatalog->setWriteCatalog( "file:me.xml" );
  fileCatalog->connect();
  fileCatalog->start();
  pool::IDataSvc* datasvc = pool::DataSvcFactory::instance( fileCatalog );
  pool::DatabaseConnectionPolicy policy;
  policy.setWriteModeForNonExisting( pool::DatabaseConnectionPolicy::CREATE );
  policy.setWriteModeForExisting( pool::DatabaseConnectionPolicy::OVERWRITE);
  datasvc->session().setDefaultConnectionPolicy( policy );
  datasvc->transaction().start(pool::ITransaction::UPDATE);
  //pool::Ref< std::bitset<7> > simple(datasvc,days);
  BitArray<9>* me=new BitArray<9>(5);
  pool::Ref< BitArray<9> > simple(datasvc,me);
  simple.markWrite(place);
  datasvc->transaction().commit();
  datasvc->session().disconnectAll();
  fileCatalog->commit();
  fileCatalog->disconnect();
  return 0;

}
