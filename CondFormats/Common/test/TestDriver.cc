#include "TestDriver.h"


#include <iostream>
#include <memory>


#include <stdexcept>
#include "StorageSvc/DbType.h"
#include "StorageSvc/poolDb.h"
#include "DataSvc/IDataSvc.h"
#include "DataSvc/DataSvcFactory.h"
#include "DataSvc/RefException.h"
#include "DataSvc/SharedAnyPtr.h"
#include "FileCatalog/IFileCatalog.h"
#include "PersistencySvc/DatabaseConnectionPolicy.h"
#include "PersistencySvc/ISession.h"

#include "FileCatalog/URIParser.h"
#include "FileCatalog/IFCAction.h"
#include "Reflex/SharedLibrary.h"


#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/standard.h"
#include "FWCore/PluginManager/interface/SharedLibrary.h"


#include "RVersion.h"

#if ROOT_VERSION_CODE < ROOT_VERSION(5,19,0)
using namespace ROOT;
#endif


#include "RelationalAccess/IRelationalService.h"
#include "RelationalAccess/IRelationalDomain.h"
#include "RelationalAccess/ConnectionService.h"
#include "RelationalAccess/ISessionProxy.h"
#include "RelationalAccess/ISchema.h"
#include "RelationalAccess/ITable.h"
#include "RelationalAccess/ITableDataEditor.h"
#include "RelationalAccess/ITableDescription.h"
#include "RelationalAccess/ITransaction.h"
#include "Collection/Collection.h"
#include "StorageSvc/DbType.h"
#include "StorageSvc/poolDb.h"
#include "PersistencySvc/Placement.h"
#include "PersistencySvc/ITransaction.h"
#include "PersistencySvc/ISession.h"
#include "DataSvc/Ref.h"
#include "DataSvc/RefException.h"
#include "FileCatalog/IFileCatalog.h"
#include "FileCatalog/IFCAction.h"
#include "FileCatalog/IFCContainer.h"
#include "FileCatalog/FCEntry.h"
#include "FileCatalog/FCException.h"
#include <cstdlib>
#include <stdexcept>
#include "CoralCommon/Sleep.h"
#include "POOLCore/IBlobStreamingService.h"
#include "CondCore/DBCommon/interface/CoralServiceManager.h"
#include "CoralKernel/Context.h"
#include "PersistencySvc/IConfiguration.h"
#include "CondFormats/Common/interface/PayloadWrapper.h"
#include "CondFormats/Common/interface/GenericSummary.h"
#include <vector>



#include "CondFormats/THEPACKAGE/interface/THEHEADER.h"
typedef THECLASS Payload;

typedef cond::DataWrapper<Payload> SimplePtr;

namespace{
  bool withWrapper=false;
}

int main(int argc, char** ) {

  withWrapper = argc>1;
  cond::TestDriver td("sqlite_file:test.db");
  td.run();

  return 0;
}


namespace {

  void printError(std::string const & err) {
    std::cout << err << std::endl;
  }
}


void cond::TestBase::init_(const std::string& catalogFileName){
  try{
    edmplugin::PluginManager::Config config;
    edmplugin::PluginManager::configure(edmplugin::standard::config());

    CoralServiceManager m_pluginmanager;
    coral::MessageStream::setMsgVerbosity( coral::Info );
    // initialize catalog
    std::string contStr = "file:"+catalogFileName;
    if (!catalogFileName.length()) contStr = contStr+"test_catalog.xml";
    pool::URIParser pars(contStr);
    pars.parse();
    m_fileCatalog = new pool::IFileCatalog;
    try {
       m_fileCatalog->setWriteCatalog( pars.contactstring() );
    } catch (const std::exception& e){
      std::cout << "ERROR: " << e.what() << std::endl;
    } 
    m_fileCatalog->connect();
    m_fileCatalog->start();
    // initialize dataSvc
    m_dataSvc = pool::DataSvcFactory::instance( m_fileCatalog );
    coral::Context::instance().loadComponent( "COND/Services/TBufferBlobStreamingService", &m_pluginmanager );
    m_dataSvc->configuration().setBlobStreamer(&(*(coral::Context::instance().query<pool::IBlobStreamingService>())), false);

    m_init_test = true;
    pool::DatabaseConnectionPolicy policy;
    policy.setWriteModeForNonExisting( pool::DatabaseConnectionPolicy::CREATE );
    policy.setWriteModeForExisting( pool::DatabaseConnectionPolicy::OVERWRITE );
    m_dataSvc->session().setDefaultConnectionPolicy( policy );
  } catch (const std::exception& e){
    std::cerr << "test ERROR:" << e.what() << "\n";
    throw;
  }
}

/// Standard Constructor
cond::TestBase::TestBase(const std::string& catalogFileName) : m_dataSvc(0),m_fileCatalog(0),m_init_test(false){
  init_(catalogFileName); 
}

/// Standard Constructor
cond::TestBase::TestBase() : m_dataSvc(0),m_fileCatalog(0),m_init_test(false){
  init_(""); 
}

/// Standard Destructor
cond::TestBase::~TestBase(){
  delete m_dataSvc;
  m_fileCatalog->commit();
  m_fileCatalog->disconnect();
  delete m_fileCatalog;
}
    
    /// Utility function to check for errors during execution
void cond::TestBase::checkError(pool::DbStatus sc, const std::string& msg)    {
  if ( !sc.isSuccess() )  {
    std::cout << "test Error: " << msg << std::endl;
  }
}

//// Utility function to print exception messages
void cond::TestBase::printException(const pool::Exception& e){
  std::cout << "Exception: " << e.what() << "\n";
}

std::string cond::TestBase::fileId(const std::string& pfn,
                                   const pool::DbType& technology){
  std::string fid, tech;
  // try to re-use an already existing file
  pool::IFCAction action;
  m_fileCatalog->setAction( action );
  action.lookupFileByPFN(pfn, fid, tech);
  // did not exist - so try register the file
  if(fid.length()==0){
    pool::DbType majorType(technology.majorType());
    action.registerPFN(pfn,majorType.storageName(),fid);
    if ( fid.length() == 0 )    
      throw std::runtime_error("failed to register file `" + pfn +"'");
  }
  return fid;
}


void cond::TestBase::setDbLogin( const std::string& userName,const std::string & passwd){
  //  pool::POOLContext::loadComponent( "POOL/Services/EnvironmentAuthenticationService" );
  const std::string userNameEnv = "POOL_AUTH_USER=" + userName;
  ::putenv( const_cast<char*>( userNameEnv.c_str() ) );
  const std::string passwdEnv = "POOL_AUTH_PASSWORD=" + passwd;
  ::putenv( const_cast<char*>( passwdEnv.c_str() ) );
}


namespace pool  {
  int numTokenInstances();
}

void cond::TestBase::run(){
  if(m_init_test){
    try   {
      execute();    
      // disconnect 
      m_dataSvc->session().disconnectAll();
      std::cout << "Number of token instances: " << 
      pool::numTokenInstances() << std::endl;
      unsigned int nobj = pool::SharedAnyPtr::numberOfInstances();
      std::cout << "Number of object instances: " 
                << nobj << std::endl;
      if(nobj){
        pool::SharedAnyPtr::dumpRegister();
      }
    }
    catch(const std::runtime_error& e)   {
      std::cout << "The test has thrown a (runtime) exception:" << e.what() << std::endl;
    }
    catch(const std::exception& e)   {
      std::cout << "The test has thrown a (std) exception:" << e.what() << std::endl;
    }
    catch(...)    {
      std::cout << "The test has thrown an unknown exception." << std::endl;
    }
  }
}

void cond::TestBase::setMessageVerbosityLevel( coral::MsgLevel level ){
  coral::MessageStream::setMsgVerbosity( level );
}

//------------------------------------

cond::TestDriver::TestDriver( const std::string& connectionString):TestBase(),m_connectionString(connectionString){
  // setMessageVerbosityLevel( coral::Debug );
}

cond::TestDriver::~TestDriver(){
}

void cond::TestDriver::createDatabase(unsigned int nobjects){
  std::cout << "##### creating and filling POOL Database " << std::endl;  
  
  pool::Placement place1, place2;
  place1.setDatabase(m_connectionString, pool::DatabaseSpecification::PFN );
  place1.setContainerName("Cont1");
  place1.setTechnology(pool::POOL_RDBMS_HOMOGENEOUS_StorageType.type());
  place2.setDatabase(m_connectionString, pool::DatabaseSpecification::PFN );
  place2.setContainerName("Cont2");
  place2.setTechnology(pool::POOL_RDBMS_HOMOGENEOUS_StorageType.type());

  std::cout << "Starting UPDATE transaction..." << std::endl;
  // Start an update transaction
  if ( ! ( m_dataSvc->transaction().start( pool::ITransaction::UPDATE ) ) ) {
    printError("test ERROR: Could not start an UPDATE transaction");
  }

  unsigned int i;
  for (i = 0; i < nobjects; ++i )   {
    try {
      pool::Ref<Payload> simple1(m_dataSvc,new Payload);
      simple1.markWrite(place1);
      if (withWrapper) {
        pool::Ref<PayloadWrapper> simple2(m_dataSvc,new SimplePtr(new Payload, new cond::GenericSummary("THECLASS")));
        simple2.markWrite(place2);
      }
    } catch (const pool::RefException& e){
      printException(e);        
    }
  }
      
  std::cout << "Commit transaction..." << std::endl;
  if ( ! m_dataSvc->transaction().commit() ) {
     printError("test ERROR: Could not commit the WRITE transaction." );
  }
  std::cout << "Written on disk "<< i << " objects\n";

  m_dataSvc->session().disconnectAll();

}

void cond::TestDriver::cleanUp(){
  std::cout << "##### cleaning up existing schema " << std::endl;

  coral::ConnectionService  connServ;
  coral::ISessionProxy* session = connServ.connect(m_connectionString );
  coral::ISchema& schema = session->nominalSchema();
  session->transaction().start();
  session->transaction().commit();
  delete session;
  std::cout <<  "All POOL Database Tables removed." <<std::endl; 
}

void cond::TestDriver::readBackData(unsigned int expected){

  std::cout << "##### reading back data from POOL database " << std::endl;

  // Start read transaction I
  m_dataSvc->transaction().start( pool::ITransaction::READ );
  pool::Collection< Payload > simpleCollection0( m_dataSvc,
                                                   "ImplicitCollection",
                                                   "PFN:" + m_connectionString,
                                                   "Cont1",
                                                   pool::ICollection::READ );
  pool::Collection< Payload >::Iterator simpleObj0 = simpleCollection0.select();
  unsigned int numberOfObj = 0;
  while ( simpleObj0.next() ) {
    try {
    // here loading all data including the ptr
      *simpleObj0.ref();
    } catch (const pool::Exception& e){
      std::cout << "ACCESSING Payload"<<e.what()<<std::endl;
    }
    numberOfObj++;
  }
  std::cout << "Read back " <<numberOfObj<< " object of type SimplePtrClass" <<std::endl;
                   
  if(numberOfObj!=expected){
    printError("TEST ERROR: number of SimplePtrClass objects read back different from expected.");
  }
  // commit the transaction
  m_dataSvc->transaction().commit();
  if (withWrapper) {
  std::cout << "##### reading cont2... " << std::endl;
  pool::Ref< SimplePtr > lastRef;
  // Start read transaction I
  m_dataSvc->transaction().start( pool::ITransaction::READ );
  pool::Collection< SimplePtr > simpleCollection1( m_dataSvc,
                                                   "ImplicitCollection",
                                                   "PFN:" + m_connectionString,
                                                   "Cont2",
                                                   pool::ICollection::READ );
  pool::Collection< SimplePtr >::Iterator simpleObj1 = simpleCollection1.select();
  numberOfObj = 0;
  while ( simpleObj1.next() ) {
    // here loading just the summary
    simpleObj1.ref()->loadSummary();
    numberOfObj++;
    lastRef = simpleObj1.ref();
  }
  std::cout << "Read back " <<numberOfObj<< " object of type SimplePtrClass" <<std::endl;
                   
  if(numberOfObj!=expected){
    printError("TEST ERROR: number of SimplePtrClass objects read back different from expected.");
  }
  
  // commit the transaction
  m_dataSvc->transaction().commit();
  try {
    // now accessing back the ptr...=";
    lastRef->data();    
  } catch (const pool::Exception& e){
    std::cout << "OK, got the exception, underlying pointer is null."<<std::endl;
    //std::cout << e.what() << std::endl;
  }
  }
  
  m_dataSvc->session().disconnectAll();

}

void cond::TestDriver::execute(){

  createDatabase(10);
  coral::sys::sleep(1);
  readBackData(10);
  // cleanUp();
}
