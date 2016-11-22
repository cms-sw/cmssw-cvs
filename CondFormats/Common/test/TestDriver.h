#ifndef COND_TESTDRIVER_H
#define COND_TESTDRIVER_H


#include <string>
#include <vector>
#include "POOLCore/Exception.h"
#include "CoralBase/MessageStream.h"
#include "StorageSvc/DbStatus.h"

namespace pool  {
  
  class IDataSvc;
  class IFileCatalog;
  class DbType;

}

namespace cond {

  class TestBase {
  protected: 
    pool::IDataSvc*        m_dataSvc;
    pool::IFileCatalog*    m_fileCatalog;
    bool m_init_test;
  public: 
    
    /// Standard Constructor
    TestBase();
    
    /// Constructor for a specific output directory
    TestBase(const std::string& catalogFileName);
    
    /// Standard Destructor
    virtual ~TestBase();
    
    /// Utility function to check for errors during execution
    void checkError( pool::DbStatus sc, const std::string& msg);
    
    std::string fileId(const std::string& pfn, const pool::DbType& technology);
    
    //// Utility function to print exception messages
    void printException( const pool::Exception& e);
    
    static void setDbLogin( const std::string& userName, const std::string & passwd);
    
    virtual void execute()=0;
    
    void run();
    
    void setMessageVerbosityLevel( coral::MsgLevel level );
    
  private:
    void init_(const std::string& catalogFileName);
    
  };
  
  
  
  class TestDriver : public TestBase 
  {
  public:
    TestDriver( const std::string& connection );
    ~TestDriver();
    
  private:
    void cleanUp();
    
    void createDatabase(unsigned int nObjects);
    
    void readBackData(unsigned int nExpectedObjects );
    
    void execute();
    
  private:

    std::string m_connectionString;
    
  };
  
}

#endif
