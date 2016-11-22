#include <stdexcept>
#include "OnlineDB/Oracle/interface/Oracle.h"

#include "OnlineDB/EcalCondDB/interface/ODSRPCycle.h"

using namespace std;
using namespace oracle::occi;

ODSRPCycle::ODSRPCycle()
{
  m_env = NULL;
  m_conn = NULL;
  m_writeStmt = NULL;
  m_readStmt = NULL;
  //
  m_ID = 0;
  m_srp_config_id = 0;
}


ODSRPCycle::~ODSRPCycle()
{
}


void ODSRPCycle::prepareWrite()
  throw(runtime_error)
{
  this->checkConnection();

  try {
    m_writeStmt = m_conn->createStatement();
    m_writeStmt->setSQL("INSERT INTO ECAL_SRP_Cycle (cycle_id, srp_configuration_id ) "
		 "VALUES (:1, :2 )");
  } catch (SQLException &e) {
    throw(runtime_error("ODSRPCycle::prepareWrite():  "+e.getMessage()));
  }
}


void ODSRPCycle::writeDB()  throw(runtime_error)
{
  this->checkConnection();
  this->checkPrepare();

  try {

    m_writeStmt->setInt(1, this->getId());
    m_writeStmt->setInt(2, this->getSRPConfigurationID());

    m_writeStmt->executeUpdate();


  } catch (SQLException &e) {
    throw(runtime_error("ODSRPCycle::writeDB:  "+e.getMessage()));
  }

  // Now get the ID
  if (!this->fetchID()) {
    throw(runtime_error("ODSRPCycle::writeDB:  Failed to write"));
  }
  
 
}

void ODSRPCycle::clear(){
  m_srp_config_id=0;
}


int ODSRPCycle::fetchID()
  throw(runtime_error)
{
  // Return from memory if available
  if (m_ID) {
    return m_ID;
  }

  this->checkConnection();

  try {
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("SELECT cycle_id, srp_configuration_id FROM ecal_srp_cycle "
		 "WHERE cycle_id = :1 ");
    stmt->setInt(1, m_ID);
    ResultSet* rset = stmt->executeQuery();

    if (rset->next()) {
      m_ID = rset->getInt(1);
      m_srp_config_id = rset->getInt(2);
    } else {
      m_ID = 0;
    }
    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("ODSRPCycle::fetchID:  "+e.getMessage()));
  }

  return m_ID;
}



void ODSRPCycle::setByID(int id) 
  throw(std::runtime_error)
{
   this->checkConnection();


  try {
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("SELECT cycle_id, srp_configuration_id FROM ecal_srp_cycle "
		 "WHERE cycle_id = :1 ");
    stmt->setInt(1, id);
    ResultSet* rset = stmt->executeQuery();

    if (rset->next()) {
      m_ID = rset->getInt(1);
      m_srp_config_id = rset->getInt(2);
    } else {
      m_ID = 0;
    }
    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("ODSRPCycle::fetchID:  "+e.getMessage()));
  }
}



void ODSRPCycle::fetchData(ODSRPCycle * result)
  throw(runtime_error)
{
  this->checkConnection();
  result->clear();

  if(result->getId()==0){
    throw(runtime_error("ODSRPConfig::fetchData(): no Id defined for this ODSRPConfig "));
  }

  try {

    m_readStmt->setSQL("SELECT  srp_configuration_id FROM ecal_srp_cycle "
		 "WHERE cycle_id = :1 ");

    m_readStmt->setInt(1, result->getId());
    ResultSet* rset = m_readStmt->executeQuery();

    rset->next();

    result->setSRPConfigurationID(       rset->getInt(1) );

  } catch (SQLException &e) {
    throw(runtime_error("ODSRPCycle::fetchData():  "+e.getMessage()));
  }
}

void ODSRPCycle::insertConfig()
  throw(std::runtime_error)
{
  try {

    prepareWrite();
    writeDB();
    m_conn->commit();
    terminateWriteStatement();
  } catch (std::runtime_error &e) {
    m_conn->rollback();
    throw(e);
  } catch (...) {
    m_conn->rollback();
    throw(std::runtime_error("EcalCondDBInterface::insertDataSet:  Unknown exception caught"));
  }
}
