#include <iostream>
#include <stdexcept>
#include "CoralBase/MessageStream.h"
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
#include "CondFormats/Calibration/interface/big.h"
int main(){
  coral::MessageStream::setMsgVerbosity(coral::Error);
  pool::Placement place;
  //place.setDatabase("oracle://devdb10/cms_xiezhen_dev", pool::DatabaseSpecification::PFN );
  place.setDatabase("sqlite_file:testbig.db", pool::DatabaseSpecification::PFN );
  place.setContainerName("BIGRCD");
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
  big* b=new big;
  b->fill(50,25,30,"works ");
  pool::Ref< big > simple(datasvc,b);
  simple.markWrite(place);
  std::string tokenstr=simple.toString();
  datasvc->transaction().commit();
  datasvc->transaction().start(pool::ITransaction::READ);
  pool::Ref< big > result(datasvc,tokenstr);
  *result;
  std::cout<<"token "<<tokenstr<<std::endl;
  datasvc->transaction().commit();
  datasvc->session().disconnectAll();
  fileCatalog->commit();
  fileCatalog->disconnect();
  return 0;

}
