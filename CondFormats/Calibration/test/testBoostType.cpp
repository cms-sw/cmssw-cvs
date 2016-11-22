#include <bitset>
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
#include "CondFormats/Calibration/interface/boostTypeObj.h"
#include <cassert>
int main(){
  coral::MessageStream::setMsgVerbosity( coral::Info );
  pool::Placement place;
  place.setDatabase("sqlite_file:me.db", pool::DatabaseSpecification::PFN );
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
  boostTypeObj trans;
  trans.a=-1;
  trans.b=-12;
  trans.aa=1;
  trans.bb=30;
  std::cout<<"transient a "<<boost::int8_t(trans.a)<<std::endl;
  assert(trans.a==-1);
  std::cout<<"transient b "<<trans.b<<std::endl;
  assert(trans.b==-12);
  std::cout<<"transient aa "<<trans.aa<<std::endl;
  std::cout<<"transient bb "<<trans.bb<<std::endl;
  boostTypeObj* me=new boostTypeObj;
  pool::Ref< boostTypeObj > simple(datasvc,me);
  simple->a=-1;
  simple->b=-12;
  simple->aa=1;
  simple->bb=30;
  simple.markWrite(place);
  std::string tokenstr=simple.toString();
  datasvc->transaction().commit();
  datasvc->transaction().start(pool::ITransaction::READ);
  pool::Ref< boostTypeObj > result(datasvc,tokenstr);
  *result;
  std::cout<<"token "<<tokenstr<<std::endl;
  std::cout<<"result.a "<<result->a<<std::endl;
  std::cout<<"result.aa "<<result->aa<<std::endl;
  assert(result->a==-1);
  std::cout<<"transient b "<<trans.b<<std::endl;
  assert(result->b==-12);
  std::cout<<"result.b "<<result->b<<std::endl;
  std::cout<<"result.bb "<<result->bb<<std::endl;
  datasvc->transaction().commit();
  datasvc->session().disconnectAll();
  fileCatalog->commit();
  fileCatalog->disconnect();
  return 0;

}
