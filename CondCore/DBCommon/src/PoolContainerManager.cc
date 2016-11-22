#include "CondCore/DBCommon/interface/PoolContainerManager.h"
#include "CondCore/DBCommon/interface/DbTransaction.h"
#include "PersistencySvc/ISession.h"
#include "PersistencySvc/IDatabase.h"
#include "PersistencySvc/ITokenIterator.h"
#include "PersistencySvc/IContainer.h"
#include "POOLCore/Token.h"
#include "StorageSvc/DbType.h"
#include "DataSvc/IDataSvc.h"
#include "DataSvc/RefBase.h"
#include "DataSvc/AnyPtr.h"
#include <algorithm>

#if ROOT_VERSION_CODE < ROOT_VERSION(5,19,0)
 using namespace ROOT;
#endif

cond::PoolContainerManager::PoolContainerManager(  cond::DbSession& pooldb ) : m_pooldb(pooldb){  
}
void 
cond::PoolContainerManager::listAll( std::vector<std::string>& containers ){
  std::string con=m_pooldb.connectionString();
  pool::IDatabase* mydb=m_pooldb.poolCache().session().databaseHandle(con, pool::DatabaseSpecification::PFN);
  mydb->connectForRead();
  mydb->containers().swap(containers);
  mydb->disconnect();
  delete mydb;
}
void 
cond::PoolContainerManager::exportContainer( cond::DbSession& destdb,
                                             const std::string& containername,
                                             const std::string& className){
  std::string con=m_pooldb.connectionString();
  pool::IDataSvc* datasvc=&(m_pooldb.poolCache());
  const Reflex::Type myclassType=Reflex::Type::ByName(className);
  pool::Placement destPlace;
  destPlace.setDatabase(destdb.connectionString(), 
			pool::DatabaseSpecification::PFN );
  destPlace.setContainerName(containername);
  destPlace.setTechnology(pool::POOL_RDBMS_HOMOGENEOUS_StorageType.type());
  pool::ITokenIterator* tokenIt=datasvc->session().databaseHandle(con,pool::DatabaseSpecification::PFN)->containerHandle(containername)->tokens("");
  pool::Token* myToken=0;
  while( (myToken=tokenIt->next())!=0 ){
    pool::RefBase myobj(datasvc,*myToken,myclassType.TypeInfo() );
    const pool::AnyPtr myPtr=myobj.object().get();
    pool::RefBase mycopy(&(destdb.poolCache()),myPtr,myclassType.TypeInfo());
    mycopy.markWrite(destPlace);
    myToken->release();
  }
  delete tokenIt;//?????what about others?
}
