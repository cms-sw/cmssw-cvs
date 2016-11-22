#include <stdexcept>
#include "OnlineDB/Oracle/interface/Oracle.h"

#include "OnlineDB/EcalCondDB/interface/LMFRunList.h"
#include "OnlineDB/EcalCondDB/interface/RunIOV.h"
#include "OnlineDB/EcalCondDB/interface/RunTag.h"
#include "OnlineDB/EcalCondDB/interface/LMFRunIOV.h"
#include "OnlineDB/EcalCondDB/interface/Tm.h"
#include "OnlineDB/EcalCondDB/interface/DateHandler.h"

using namespace std;
using namespace oracle::occi;

LMFRunList::LMFRunList()
{
  m_conn = NULL;
}

LMFRunList::~LMFRunList()
{
}

void LMFRunList::setRunTag(RunTag tag)
{
  if (tag != m_runTag) {
    m_runTag = tag;
  }
}
void LMFRunList::setLMFRunTag(LMFRunTag tag)
{
  if (tag != m_lmfrunTag) {
    m_lmfrunTag = tag;
  }
}


RunTag LMFRunList::getRunTag() const
{
  return m_runTag;
}
LMFRunTag LMFRunList::getLMFRunTag() const
{
  return m_lmfrunTag;
}

 std::vector<LMFRunIOV> LMFRunList::getRuns() 
{
  return m_vec_lmfruniov;
}


void LMFRunList::fetchRuns()
  throw(runtime_error)
{


  this->checkConnection();
  int nruns=0;

  m_runTag.setConnection(m_env, m_conn);
  int tagID =0;

  RunTag empty_rt;

  if(m_runTag != empty_rt  ){
    tagID=m_runTag.fetchID();
    //GHM cout <<"tag id="<< tagID << endl;
    if (!tagID) { 
      return ;
    }
  }

  m_lmfrunTag.setConnection(m_env, m_conn);
  int lmftagID = m_lmfrunTag.fetchID();
  //GHM  cout <<"lmf tag id="<< lmftagID << endl;
  if (!lmftagID) { 
    return ;
  }

  try {
    Statement* stmt0 = m_conn->createStatement();
    string sqlstr;
    sqlstr = "SELECT count(lmf_run_iov.lmf_iov_id) FROM lmf_run_iov, run_iov WHERE lmf_run_iov.run_iov_id= run_iov.iov_id and lmf_run_iov.tag_id=:lmftag_id " ;
    if (tagID!=0) sqlstr = sqlstr +" AND run_iov.tag_id = :tag_id "; 

    //    stmt0->setSQL("SELECT count(lmf_run_iov.lmf_iov_id) FROM lmf_run_iov, run_iov "
    //		 "WHERE lmf_run_iov.run_iov_id= run_iov.iov_id and run_iov.tag_id = :tag_id and lmf_run_iov.tag_id=:lmftag_id " );

    stmt0->setSQL(sqlstr);
    stmt0->setInt(1, lmftagID);
    if (tagID!=0) stmt0->setInt(2, tagID);
  
    ResultSet* rset0 = stmt0->executeQuery();
    if (rset0->next()) {
      nruns = rset0->getInt(1);
    }
    m_conn->terminateStatement(stmt0);

    //GHM    cout <<"number of LMF runs is ="<< nruns << endl;
    
    m_vec_lmfruniov.reserve(nruns);
    
    Statement* stmt = m_conn->createStatement();

    string sqlstr2;
    sqlstr2 = "SELECT run_iov.run_num, run_iov.run_start, run_iov.run_end, lmf_run_iov.tag_id, lmf_run_iov.run_iov_id, lmf_run_iov.subrun_num, lmf_run_iov.subrun_start, lmf_run_iov.subrun_end, lmf_run_iov.subrun_type, lmf_run_iov.db_timestamp, lmf_run_iov.lmf_iov_id FROM run_iov, lmf_run_iov WHERE lmf_run_iov.run_iov_id=run_iov.iov_id ";
    if (tagID!=0) sqlstr2 = sqlstr2 +" AND run_iov.tag_id = :tag_id "; 
    sqlstr2=sqlstr2+" order by run_iov.run_num, lmf_run_iov.subrun_num ";

    //    stmt->setSQL("SELECT run_iov.run_num, run_iov.run_start, run_iov.run_end, lmf_run_iov.tag_id, lmf_run_iov.run_iov_id, lmf_run_iov.subrun_num, lmf_run_iov.subrun_start, lmf_run_iov.subrun_end, lmf_run_iov.subrun_type, lmf_run_iov.db_timestamp, lmf_run_iov.lmf_iov_id FROM run_iov, lmf_run_iov "
    //	 "WHERE lmf_run_iov.run_iov_id=run_iov.iov_id and run_iov.tag_id = :tag_id  order by run_iov.run_num, lmf_run_iov.subrun_num " );

    stmt->setSQL(sqlstr2);

    if (tagID!=0) stmt->setInt(1, tagID);

    DateHandler dh(m_env, m_conn);
    Tm runStart;
    Tm runEnd;
    Tm lrunStart;
    Tm ldbtime;
    Tm lrunEnd;
  
    ResultSet* rset = stmt->executeQuery();
    int i=0;
    while (i<nruns) {
      rset->next();
       int runNum = rset->getInt(1);
       Date startDate = rset->getDate(2);
       Date endDate = rset->getDate(3);
       // int ltag = rset->getInt(4);
       int lid=rset->getInt(5);
       int subrun=rset->getInt(6);
       Date lmfstartDate = rset->getDate(7);
       Date lmfendDate = rset->getDate(8);
       std::string lmf_type= rset->getString(9);
       Date lmfDBtime = rset->getDate(10);
       int liov_id=rset->getInt(11);
	 
       runStart = dh.dateToTm( startDate );
       runEnd = dh.dateToTm( endDate );
       lrunStart = dh.dateToTm( lmfstartDate );
       lrunEnd = dh.dateToTm( lmfendDate );
       ldbtime = dh.dateToTm( lmfDBtime );
       
       RunIOV r ;
       r.setRunNumber(runNum);
       r.setRunStart(runStart);
       r.setRunEnd(runEnd);
       r.setRunTag(m_runTag);
       r.setID(lid);
    
       LMFRunIOV lr ;
       // da correggere qui
       lr.setRunIOV(r);
       lr.setSubRunNumber(subrun);
       lr.setSubRunStart(lrunStart);
       lr.setSubRunEnd(lrunEnd);
       lr.setDBInsertionTime(ldbtime);
       lr.setSubRunType(lmf_type);
       lr.setLMFRunTag(m_lmfrunTag);
       lr.setID(liov_id);
       m_vec_lmfruniov.push_back(lr);
      
      i++;
    }
   

    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("RunIOV::fetchID:  "+e.getMessage()));
  }


}

void LMFRunList::fetchRuns(int min_run, int max_run)
  throw(runtime_error)
{


  this->checkConnection();
  int nruns=0;

  m_runTag.setConnection(m_env, m_conn);

  int tagID =0;
  RunTag empty_rt;
  if(m_runTag != empty_rt  ){
    tagID=m_runTag.fetchID();
    //GHM    cout <<"tag id="<< tagID << endl;
    if (!tagID) { 
      return ;
    }
  }

  m_lmfrunTag.setConnection(m_env, m_conn);
  int lmftagID = m_lmfrunTag.fetchID();
  //GHM  cout <<"lmf tag id="<< lmftagID << endl;
  if (!lmftagID) { 
    return ;
  }

  int my_min_run=min_run-1;
  int my_max_run=max_run+1;
  try {
    Statement* stmt0 = m_conn->createStatement();

    //GHM cout <<"query with parameters:"<< lmftagID <<" " <<my_min_run <<" "<<my_max_run << " " << tagID<< endl;

    string sqlstr;
    sqlstr = "SELECT count(lmf_run_iov.lmf_iov_id) FROM lmf_run_iov, run_iov WHERE lmf_run_iov.run_iov_id= run_iov.iov_id and lmf_run_iov.tag_id=:lmftag_id and run_iov.run_num> :min_run and run_iov.run_num< :max_run " ;
    if (tagID!=0) sqlstr = sqlstr +" AND run_iov.tag_id = :tag_id ";

    stmt0->setSQL(sqlstr );
    stmt0->setInt(1, lmftagID);
    stmt0->setInt(2, my_min_run);
    stmt0->setInt(3, my_max_run);
    if(tagID!=0) stmt0->setInt(4, tagID);
  
    ResultSet* rset0 = stmt0->executeQuery();
    if (rset0->next()) {
      nruns = rset0->getInt(1);
    }
    m_conn->terminateStatement(stmt0);

    //GHM    cout <<"number of LMF runs="<< nruns << endl;
    
    m_vec_lmfruniov.reserve(nruns);
    
    Statement* stmt = m_conn->createStatement();


    string sqlstr2;
    sqlstr2 = "SELECT run_iov.run_num, run_iov.run_start, run_iov.run_end, lmf_run_iov.tag_id, lmf_run_iov.run_iov_id, lmf_run_iov.subrun_num, lmf_run_iov.subrun_start, lmf_run_iov.subrun_end, lmf_run_iov.lmf_iov_id, lmf_run_iov.subrun_type, lmf_run_iov.db_timestamp FROM run_iov, lmf_run_iov WHERE lmf_run_iov.run_iov_id=run_iov.iov_id and lmf_run_iov.tag_id=:lmftag_id and run_iov.run_num> :min_run and run_iov.run_num< :max_run ";
    if (tagID!=0) sqlstr2 = sqlstr2 +" AND run_iov.tag_id = :tag_id ";
    sqlstr2=sqlstr2+ " order by run_iov.run_num, lmf_run_iov.subrun_num ";
    

    stmt->setSQL(sqlstr2);

    stmt->setInt(1, lmftagID);
    stmt->setInt(2, my_min_run);
    stmt->setInt(3, my_max_run);

    if (tagID!=0) stmt->setInt(4, tagID);


    DateHandler dh(m_env, m_conn);
    Tm runStart;
    Tm runEnd;
    Tm lrunStart;
    Tm ldbtime;
    Tm lrunEnd;
  
    ResultSet* rset = stmt->executeQuery();
    int i=0;
    while (i<nruns) {
      rset->next();
       int runNum = rset->getInt(1);
       Date startDate = rset->getDate(2);
       Date endDate = rset->getDate(3);
       //int ltag = rset->getInt(4);
       int lid=rset->getInt(5);
       int subrun=rset->getInt(6);
       Date lmfstartDate = rset->getDate(7);
       Date lmfendDate = rset->getDate(8);
       int liov_id=rset->getInt(9);
       std::string lmf_type=rset->getString(10);
       Date lmfdbtime = rset->getDate(11);
	 
       runStart = dh.dateToTm( startDate );
       runEnd = dh.dateToTm( endDate );
       lrunStart = dh.dateToTm( lmfstartDate );
       lrunEnd = dh.dateToTm( lmfendDate );
       ldbtime = dh.dateToTm( lmfdbtime );
       
       RunIOV r ;
       r.setRunNumber(runNum);
       r.setRunStart(runStart);
       r.setRunEnd(runEnd);
       r.setRunTag(m_runTag);
       r.setID(lid);
    
       LMFRunIOV lr ;
       lr.setRunIOV(r);
       lr.setSubRunNumber(subrun);
       lr.setSubRunStart(lrunStart);
       lr.setSubRunEnd(lrunEnd);
       lr.setLMFRunTag(m_lmfrunTag);
       lr.setDBInsertionTime(ldbtime);
       lr.setSubRunType(lmf_type);
       lr.setID(liov_id);

       m_vec_lmfruniov.push_back(lr);
      
      i++;
    }
   

    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("RunIOV::fetchID:  "+e.getMessage()));
  }


}

void LMFRunList::fetchLastNRuns( int max_run, int n_runs  )
  throw(runtime_error)
{

  // fetch the last n_runs that come just before max_run (including max_run)

  this->checkConnection();


  m_runTag.setConnection(m_env, m_conn);
  int tagID = m_runTag.fetchID();
  //GHM  cout <<"tag id="<< tagID << endl;
  if (!tagID) { 
    return ;
  }
  m_lmfrunTag.setConnection(m_env, m_conn);
  int lmftagID = m_lmfrunTag.fetchID();
  //GHM  cout <<"lmf tag id="<< lmftagID << endl;
  if (!lmftagID) { 
    return ;
  }

  int my_max_run=max_run+1;
  try {

    int nruns=n_runs;
    m_vec_lmfruniov.reserve(nruns);
    
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("select run_num, run_start, run_end, tag_id, run_iov_id, subrun_num, subrun_start, subrun_end, lmf_iov_id, lmf_db_date, subrun_type from (SELECT run_iov.run_num, run_iov.run_start, run_iov.run_end, lmf_run_iov.tag_id, lmf_run_iov.run_iov_id, lmf_run_iov.subrun_num, lmf_run_iov.subrun_start, lmf_run_iov.subrun_end, lmf_run_iov.lmf_iov_id as lmf_iov_id, lmf_run_iov.db_timestamp as lmf_db_date, lmf_run_iov.subrun_type as subrun_type  FROM run_iov, lmf_run_iov "
		 "WHERE lmf_run_iov.run_iov_id=run_iov.iov_id and run_iov.tag_id = :tag_id "
		 " and lmf_run_iov.tag_id=:lmftag_id "
		 " and run_iov.run_num< :max_run "
		 " order by run_iov.run_num, lmf_run_iov.subrun_num DESC ) where rownum< :n_runs" );
    stmt->setInt(1, tagID);
    stmt->setInt(2, lmftagID);
    stmt->setInt(3, my_max_run);
    stmt->setInt(4, n_runs);

    DateHandler dh(m_env, m_conn);
    Tm runStart;
    Tm runEnd;
    Tm lrunStart;
    Tm lrunEnd;
    Tm ldbtime;
  
    ResultSet* rset = stmt->executeQuery();
    int i=0;
    while (i<nruns) {
      rset->next();
       int runNum = rset->getInt(1);
       Date startDate = rset->getDate(2);
       Date endDate = rset->getDate(3);
       //int ltag = rset->getInt(4);
       int lid=rset->getInt(5);
       int subrun=rset->getInt(6);
       Date lmfstartDate = rset->getDate(7);
       Date lmfendDate = rset->getDate(8);
       int liov_id=rset->getInt(9);
       Date lmfDBDate = rset->getDate(10);
       string lmf_type=rset->getString(11);

       runStart = dh.dateToTm( startDate );
       runEnd = dh.dateToTm( endDate );
       lrunStart = dh.dateToTm( lmfstartDate );
       lrunEnd = dh.dateToTm( lmfendDate );
       ldbtime = dh.dateToTm( lmfDBDate );
       
       RunIOV r ;
       r.setRunNumber(runNum);
       r.setRunStart(runStart);
       r.setRunEnd(runEnd);
       r.setRunTag(m_runTag);
       r.setID(lid);
    
       LMFRunIOV lr ;
       // da correggere qui
       lr.setRunIOV(r);
       lr.setSubRunNumber(subrun);
       lr.setSubRunStart(lrunStart);
       lr.setDBInsertionTime(ldbtime);
       lr.setSubRunEnd(lrunEnd);
       lr.setSubRunType(lmf_type);
       lr.setLMFRunTag(m_lmfrunTag);
       lr.setID(liov_id);
       m_vec_lmfruniov.push_back(lr);
      
      i++;
    }
   

    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("RunIOV::fetchID:  "+e.getMessage()));
  }


}

void LMFRunList::fetchLastNRunsBefore(uint64_t start_micro , int n_runs  )
  throw(runtime_error)
{

  // fetch the last n_runs that come just before time start_micro

  this->checkConnection();

  DateHandler dh(m_env, m_conn);
  
  Tm start_time(start_micro);
  cout << "query with start_time="<< start_time.microsTime()<<endl; 
  cout<< " which corresponds to date: "<<start_time.str()<<endl;

  m_runTag.setConnection(m_env, m_conn);
  int tagID = m_runTag.fetchID();
    cout <<"tag id="<< tagID << endl;
  if (!tagID) { 
    return ;
  }
  m_lmfrunTag.setConnection(m_env, m_conn);
  int lmftagID = m_lmfrunTag.fetchID();
    cout <<"lmf tag id="<< lmftagID << endl;
  if (!lmftagID) { 
    return ;
  }

  try {

    int nruns=n_runs;
    m_vec_lmfruniov.reserve(nruns);
    
    Statement* stmt = m_conn->createStatement();
    stmt->setSQL("select run_num, run_start, run_end, tag_id, run_iov_id, subrun_num, subrun_start, subrun_end, lmf_iov_id, lmf_db_date, subrun_type from (SELECT run_iov.run_num, run_iov.run_start, run_iov.run_end, lmf_run_iov.tag_id, lmf_run_iov.run_iov_id, lmf_run_iov.subrun_num, lmf_run_iov.subrun_start, lmf_run_iov.subrun_end, lmf_run_iov.lmf_iov_id as lmf_iov_id, lmf_run_iov.db_timestamp as lmf_db_date, lmf_run_iov.subrun_type as subrun_type  FROM run_iov, lmf_run_iov "
		 "WHERE lmf_run_iov.run_iov_id=run_iov.iov_id and run_iov.tag_id = :tag_id "
		 " and lmf_run_iov.tag_id=:lmftag_id "
		 " and lmf_run_iov.subrun_start<= :time_micro "
		 " order by lmf_run_iov.subrun_start DESC ) where rownum<= :n_runs order by subrun_start DESC " );
    stmt->setInt(1, tagID);
    stmt->setInt(2, lmftagID);
    stmt->setDate(3, dh.tmToDate(start_time));
    stmt->setInt(4, n_runs);


    DateHandler dh(m_env, m_conn);
    Tm runStart;
    Tm runEnd;
    Tm lrunStart;
    Tm lrunEnd;
    Tm ldbtime;
  
    ResultSet* rset = stmt->executeQuery();
    int i=0;
    while (i<nruns) {
      rset->next();
       int runNum = rset->getInt(1);
       Date startDate = rset->getDate(2);
       Date endDate = rset->getDate(3);
       //int ltag = rset->getInt(4);
       int lid=rset->getInt(5);
       int subrun=rset->getInt(6);
       Date lmfstartDate = rset->getDate(7);
       Date lmfendDate = rset->getDate(8);
       int liov_id=rset->getInt(9);
       Date lmfDBDate = rset->getDate(10);
       string lmf_type=rset->getString(11);

       runStart = dh.dateToTm( startDate );
       runEnd = dh.dateToTm( endDate );
       lrunStart = dh.dateToTm( lmfstartDate );
       lrunEnd = dh.dateToTm( lmfendDate );
       ldbtime = dh.dateToTm( lmfDBDate );
       
       RunIOV r ;
       r.setRunNumber(runNum);
       r.setRunStart(runStart);
       r.setRunEnd(runEnd);
       r.setRunTag(m_runTag);
       r.setID(lid);
    
       LMFRunIOV lr ;
       // da correggere qui
       lr.setRunIOV(r);
       lr.setSubRunNumber(subrun);
       lr.setSubRunStart(lrunStart);
       lr.setDBInsertionTime(ldbtime);
       lr.setSubRunEnd(lrunEnd);
       lr.setSubRunType(lmf_type);
       lr.setLMFRunTag(m_lmfrunTag);
       lr.setID(liov_id);
       m_vec_lmfruniov.push_back(lr);
      
      i++;
    }
   

    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("RunIOV::fetchID:  "+e.getMessage()));
  }


}




void LMFRunList::fetchRuns(uint64_t start_micro, int min_run, int end_run)
  throw(runtime_error)
{


  this->checkConnection();
  int nruns=0;

  m_runTag.setConnection(m_env, m_conn);

  int tagID =0;
  RunTag empty_rt;
  if(m_runTag != empty_rt  ){
    tagID=m_runTag.fetchID();
    //GHM    cout <<"tag id="<< tagID << endl;
    if (!tagID) { 
      return ;
    }
  }

  m_lmfrunTag.setConnection(m_env, m_conn);
  int lmftagID = m_lmfrunTag.fetchID();
  //GHM  cout <<"lmf tag id="<< lmftagID << endl;
  if (!lmftagID) { 
    return ;
  }

  DateHandler dh(m_env, m_conn);
  
  Date min_d= dh.minDate();
  Tm tmin_d= dh.dateToTm(min_d);
  cout << "min time="<< tmin_d.microsTime()<<endl;

  Tm start_time(start_micro);
  cout << "query with start_time="<< start_time.microsTime()<<endl; 
  cout << "query with LMF tag id:"<< lmftagID << endl;




  try {
    Statement* stmt0 = m_conn->createStatement();

    start_time.dumpTm();

    string sqlstr;
    sqlstr = "SELECT count(lmf_run_iov.lmf_iov_id) FROM lmf_run_iov, run_iov WHERE lmf_run_iov.run_iov_id= run_iov.iov_id and lmf_run_iov.tag_id=:lmftag_id and lmf_run_iov.subrun_start> :min_time and run_iov.run_num > :min_run and run_iov.run_num < :max_run  " ;
    if (tagID!=0) sqlstr = sqlstr +" AND run_iov.tag_id = :tag_id ";

    stmt0->setSQL(sqlstr );
    stmt0->setInt(1, lmftagID);
    stmt0->setDate(2, dh.tmToDate(start_time));
    stmt0->setInt(3, min_run);
    stmt0->setInt(4, end_run);

    if(tagID!=0) stmt0->setInt(5, tagID);
  
    ResultSet* rset0 = stmt0->executeQuery();
    if (rset0->next()) {
      nruns = rset0->getInt(1);
    }
    m_conn->terminateStatement(stmt0);

    //GHM    cout <<"number of LMF runs="<< nruns << endl;
    
    m_vec_lmfruniov.reserve(nruns);
    
    Statement* stmt = m_conn->createStatement();


    string sqlstr2;
    sqlstr2 = "SELECT run_iov.run_num, run_iov.run_start, run_iov.run_end, lmf_run_iov.tag_id, lmf_run_iov.run_iov_id, lmf_run_iov.subrun_num, lmf_run_iov.subrun_start, lmf_run_iov.subrun_end, lmf_run_iov.lmf_iov_id, lmf_run_iov.subrun_type, lmf_run_iov.db_timestamp FROM run_iov, lmf_run_iov WHERE lmf_run_iov.run_iov_id=run_iov.iov_id and lmf_run_iov.tag_id=:lmftag_id and lmf_run_iov.subrun_start> :min_time and run_iov.run_num > :min_run and run_iov.run_num < :max_run  ";
    if (tagID!=0) sqlstr2 = sqlstr2 +" AND run_iov.tag_id = :tag_id ";
    sqlstr2=sqlstr2+ " order by run_iov.run_num, lmf_run_iov.subrun_num ";
    
    stmt->setSQL(sqlstr2);
    stmt->setInt(1, lmftagID);
    stmt->setDate(2, dh.tmToDate(start_time));
    stmt->setInt(3, min_run);
    stmt->setInt(4, end_run);
    if (tagID!=0) stmt->setInt(5, tagID);


    DateHandler dh(m_env, m_conn);
    Tm runStart;
    Tm runEnd;
    Tm lrunStart;
    Tm ldbtime;
    Tm lrunEnd;
  
    ResultSet* rset = stmt->executeQuery();
    int i=0;
    while (i<nruns) {
      rset->next();
       int runNum = rset->getInt(1);
       Date startDate = rset->getDate(2);
       Date endDate = rset->getDate(3);
       //int ltag = rset->getInt(4);
       int lid=rset->getInt(5);
       int subrun=rset->getInt(6);
       Date lmfstartDate = rset->getDate(7);
       Date lmfendDate = rset->getDate(8);
       int liov_id=rset->getInt(9);
       std::string lmf_type=rset->getString(10);
       Date lmfdbtime = rset->getDate(11);
	 
       runStart = dh.dateToTm( startDate );
       runEnd = dh.dateToTm( endDate );
       lrunStart = dh.dateToTm( lmfstartDate );
       lrunEnd = dh.dateToTm( lmfendDate );
       ldbtime = dh.dateToTm( lmfdbtime );
       
       RunIOV r ;
       r.setRunNumber(runNum);
       r.setRunStart(runStart);
       r.setRunEnd(runEnd);
       r.setRunTag(m_runTag);
       r.setID(lid);
    
       LMFRunIOV lr ;
       // da correggere qui
       lr.setRunIOV(r);
       lr.setSubRunNumber(subrun);
       lr.setSubRunStart(lrunStart);
       lr.setSubRunEnd(lrunEnd);
       lr.setLMFRunTag(m_lmfrunTag);
       lr.setDBInsertionTime(ldbtime);
       lr.setSubRunType(lmf_type);
       lr.setID(liov_id);

       m_vec_lmfruniov.push_back(lr);
      
      i++;
    }
   

    m_conn->terminateStatement(stmt);
  } catch (SQLException &e) {
    throw(runtime_error("RunIOV::fetchID:  "+e.getMessage()));
  }


}
