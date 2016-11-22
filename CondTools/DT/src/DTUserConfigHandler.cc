/*
 *  See header file for a description of this class.
 *
 *  $Date: 2009/12/08 16:37:48 $
 *  $Revision: 1.1 $
 *  \author Paolo Ronchese INFN Padova
 *
 */

//-----------------------
// This Class' Header --
//-----------------------
#include "CondTools/DT/interface/DTUserConfigHandler.h"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "CondTools/DT/interface/DTConfigHandler.h"
#include "CondTools/DT/interface/DTDBSession.h"
#include "CondFormats/DTObjects/interface/DTCCBConfig.h"
#include "CondFormats/DTObjects/interface/DTConfigList.h"
#include "CondCore/DBCommon/interface/AuthenticationMethod.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Connection.h"
#include "CondCore/DBCommon/interface/CoralTransaction.h"
#include "CondCore/DBCommon/interface/SessionConfiguration.h"
#include "RelationalAccess/ISessionProxy.h"
#include "RelationalAccess/ISchema.h"
#include "RelationalAccess/ITable.h"
#include "RelationalAccess/ICursor.h"
#include "RelationalAccess/IQuery.h"
#include "CoralBase/AttributeList.h"
#include "CoralBase/AttributeSpecification.h"
#include "CoralBase/Attribute.h"

//---------------
// C++ Headers --
//---------------


//-------------------
// Initializations --
//-------------------


//----------------
// Constructors --
//----------------
DTUserConfigHandler::DTUserConfigHandler( const edm::ParameterSet& ps ):
 dataRun(               ps.getParameter<int>         ( "run" ) ),
 dataTag(               ps.getParameter<std::string> ( "tag" ) ),
 onlineConnect(         ps.getParameter<std::string> ( "onlineDB" ) ),
 onlineAuthentication(  ps.getParameter<std::string> ( 
                        "onlineAuthentication" ) ),
 offlineAuthentication( ps.getParameter<edm::ParameterSet>( "DBParameters" )
                          .getUntrackedParameter<std::string> (
                        "authenticationPath" ) ),
 offlineConnect(        ps.getParameter<std::string> ( "offlineDB" ) ),
// catalog(               ps.getParameter<std::string> ( "catalog" ) ),
 listToken(             ps.getParameter<std::string> ( "token" ) ) {
  std::cout << " PopCon application for DT configuration export "
            <<  onlineAuthentication << " "
            << offlineAuthentication << std::endl;

  std::vector<edm::ParameterSet> dtConfigKeys (
        ps.getParameter< std::vector<edm::ParameterSet> >( "DTConfigKeys" )
  );
  std::vector<edm::ParameterSet>::const_iterator iter = dtConfigKeys.begin();
  std::vector<edm::ParameterSet>::const_iterator iend = dtConfigKeys.end();
  while ( iter != iend ) {
    const edm::ParameterSet& cp = *iter++;
    int configType = cp.getUntrackedParameter<int>( "configType" );
    int configKey  = cp.getUntrackedParameter<int>( "configKey"  );
    std::cout << "config: "
              << configType << " -> "
              << configKey << std::endl;
    DTConfigKey userKey;
    userKey.confType = configType;
    userKey.confKey  = configKey;
    userConf.push_back( userKey );
  }
}

//--------------
// Destructor --
//--------------
DTUserConfigHandler::~DTUserConfigHandler() {
}

//--------------
// Operations --
//--------------
void DTUserConfigHandler::getNewObjects() {

//  return;
  cond::DBSession* coralSession;
  cond::Connection* m_connection;
  cond::CoralTransaction* m_coraldb;

  coralSession = new cond::DBSession();
  // to get the username/passwd from $CORAL_AUTH_PATH/authentication.xml
  coralSession->configuration().setAuthenticationMethod( cond::XML );
  coralSession->configuration().setAuthenticationPath( onlineAuthentication );
  // set message level to Error or Debug
  coralSession->configuration().setMessageLevel( cond::Error );
//  coralSession->connectionConfiguration().setConnectionRetrialTimeOut( 60 );
  coralSession->open();

  m_connection = new cond::Connection( onlineConnect );
  m_connection->connect( coralSession );
  m_coraldb = &( m_connection->coralTransaction() );
  m_coraldb->start( true );

//  std::cout << "get session proxy... " << std::endl;
  isession = &( m_coraldb->coralSessionProxy() );
//  std::cout << "session proxy got" << std::endl;
  m_coraldb->start( true );

//  chkConfigList();
//  std::cout << "get run config..." << std::endl;

  //to access the information on the tag inside the offline database:
  cond::TagInfo const & ti = tagInfo();
  unsigned int last = ti.lastInterval.first;
  std::cout << "last configuration key already copied for run: "
            << last << std::endl;

/*
  std::vector<DTConfigKey> lastKey;
  if ( last == 0 ) {
    DTCCBConfig* dummyConf = new DTCCBConfig( dataTag );
    dummyConf->setStamp( 0 );
    dummyConf->setFullKey( lastKey );
    cond::Time_t snc = 1;
    m_to_transfer.push_back( std::make_pair( dummyConf, snc ) );
  }
  else {
    Ref payload = lastPayload();
    lastKey = payload->fullKey();
    std::cout << "last key: " << std::endl;
    std::vector<DTConfigKey>::const_iterator keyIter = lastKey.begin();
    std::vector<DTConfigKey>::const_iterator keyIend = lastKey.end();
    while ( keyIter != keyIend ) {
      const DTConfigKey& keyList = *keyIter++;
      std::cout << keyList.confType << " : " << keyList.confKey << std::endl;
    }
  }
*/

  //to access the information on last successful log entry for this tag:
//  cond::LogDBEntry const & lde = logDBEntry();     

  //to access the lastest payload (Ref is a smart pointer)
//  Ref payload = lastPayload();

/*
  unsigned lastRun = last;
  std::cout << "check for new runs since " << lastRun << std::endl;

  // Find latest runs
  std::map<int,int> runMap;
//  std::map<int,int> cfgMap;
  std::map<int,std::vector<DTConfigKey>*> rhcMap;
  coral::ITable& runHistoryTable =
    isession->nominalSchema().tableHandle( "RUNHISTORY" );
  std::auto_ptr<coral::IQuery>
    runHistoryQuery( runHistoryTable.newQuery() );
  runHistoryQuery->addToOutputList( "RUN" );
//  runHistoryQuery->addToOutputList( "CCBCSET" );
  runHistoryQuery->addToOutputList( "RHID" );
  coral::ICursor& runHistoryCursor = runHistoryQuery->execute();
  while( runHistoryCursor.next() ) {
    const coral::AttributeList& row = runHistoryCursor.currentRow();
    int runId = row[    "RUN"].data<int>();
//    int cfgId = static_cast<int>( row["CCBCSET"].data<long long>() );
    int rhcId = static_cast<int>( row["RHID"].data<int>() );
    if ( static_cast<unsigned>( runId ) <= lastRun ) continue;
    std::cout << "schedule config key copy for run "
              << runId << " ---> RHID " << rhcId << std::endl;
    if ( runMap.find( runId ) == runMap.end() )
         runMap.insert( std::pair<int,int>( runId, rhcId ) );
//    if ( cfgMap.find( cfgId ) == cfgMap.end() )
//         cfgMap.insert( std::pair<int,int>( cfgId, runId ) );
    if ( rhcMap.find( rhcId ) == rhcMap.end() )
         rhcMap.insert( std::pair<int,std::vector<DTConfigKey>*>( rhcId, 0 ) );
  }
  if ( !runMap.size() ) std::cout << "no new run found" << std::endl;
*/

  // get ccb identifiers map
  std::cout << "retrieve CCB map" << std::endl;
  std::map<int,DTCCBId> ccbMap;
  coral::ITable& ccbMapTable =
    isession->nominalSchema().tableHandle( "CCBMAP" );
  std::auto_ptr<coral::IQuery>
    ccbMapQuery( ccbMapTable.newQuery() );
  ccbMapQuery->addToOutputList( "CCBID" );
  ccbMapQuery->addToOutputList( "WHEEL" );
  ccbMapQuery->addToOutputList( "SECTOR" );
  ccbMapQuery->addToOutputList( "STATION" );
  coral::ICursor& ccbMapCursor = ccbMapQuery->execute();
  while( ccbMapCursor.next() ) {
    const coral::AttributeList& row = ccbMapCursor.currentRow();
    int ccb     = row["CCBID"  ].data<int>();
    int wheel   = row["WHEEL"  ].data<int>();
    int sector  = row["SECTOR" ].data<int>();
    int station = row["STATION"].data<int>();
    DTCCBId ccbId;
    ccbId.  wheelId =   wheel;
    ccbId.stationId = station;
    ccbId. sectorId =  sector;
    ccbMap.insert( std::pair<int,DTCCBId>( ccb, ccbId ) );
  }

  // get brick types
  std::cout << "retrieve brick types" << std::endl;
  std::map<int,int> bktMap;
  coral::AttributeList emptyBindVariableList;
  std::auto_ptr<coral::IQuery>
         brickTypeQuery( isession->nominalSchema().newQuery() );
  brickTypeQuery->addToTableList( "CFGBRICKS" );
  brickTypeQuery->addToTableList( "BRKT2CSETT" );
  std::string bTypeCondition = "CFGBRICKS.BRKTYPE=BRKT2CSETT.BRKTYPE";
  brickTypeQuery->addToOutputList( "CFGBRICKS.BRKID" );
  brickTypeQuery->addToOutputList( "BRKT2CSETT.CSETTYPE" );
  brickTypeQuery->setCondition( bTypeCondition, emptyBindVariableList );
  coral::ICursor& brickTypeCursor = brickTypeQuery->execute();
  while( brickTypeCursor.next() ) {
    const coral::AttributeList& row = brickTypeCursor.currentRow();
    int id = row["CFGBRICKS.BRKID"    ].data<int>();
    int bt = row["BRKT2CSETT.CSETTYPE"].data<short>();
//    std::cout << "brick " << id << " type " << bt << std::endl;
// @@FIX - TEMPORARY PATCH
    if ( bt > 3 ) bt = 3;
    bktMap.insert( std::pair<int,int>( id, bt ) );
  }

/*
  // get RH relations
  std::cout << "retrieve RH relations" << std::endl;
  std::map<int,int> cfgMap;
  coral::ITable& rhcRelTable =
    isession->nominalSchema().tableHandle( "RHRELATIONS" );
  std::auto_ptr<coral::IQuery>
    rhcRelQuery( rhcRelTable.newQuery() );
  rhcRelQuery->addToOutputList( "RHID" );
  rhcRelQuery->addToOutputList( "CONFKEY" );
  rhcRelQuery->addToOutputList( "CSETTYPEID" );
  coral::ICursor& rhcRelCursor = rhcRelQuery->execute();
  // loop over all RH relations
  while( rhcRelCursor.next() ) {
    const coral::AttributeList& row = rhcRelCursor.currentRow();
    int rhc     = row["RHID"      ].data<int>();
    int key     = row["CONFKEY"   ].data<int>();
    int cfg     = row["CSETTYPEID"].data<int>();
    // check for used configurations
    std::map<int,std::vector<DTConfigKey>*>::iterator rhcIter =
                                                      rhcMap.find( rhc );
    std::map<int,std::vector<DTConfigKey>*>::iterator rhcIend =
                                                      rhcMap.end();
    if ( rhcIter == rhcIend ) continue;
    std::vector<DTConfigKey>* keyPtr = rhcIter->second;
    if ( keyPtr == 0 ) rhcIter->second = keyPtr = new std::vector<DTConfigKey>;
    DTConfigKey confList;
    confList.confType = cfg;
    confList.confKey  = key;
    keyPtr->push_back( confList );
    if ( cfgMap.find( cfg ) == cfgMap.end() )
         cfgMap.insert( std::pair<int,int>( key, rhc ) );
  }
*/

  // get ccb config keys
  std::cout << "retrieve CCB configuration keys" << std::endl;
  std::map<int,std::map<int,int>*> keyMap;
  std::map<int,int> cckMap;
  coral::ITable& ccbRelTable =
    isession->nominalSchema().tableHandle( "CCBRELATIONS" );
  std::auto_ptr<coral::IQuery>
    ccbRelQuery( ccbRelTable.newQuery() );
  ccbRelQuery->addToOutputList( "CONFKEY" );
  ccbRelQuery->addToOutputList( "CCBID" );
  ccbRelQuery->addToOutputList( "CONFCCBKEY" );
  coral::ICursor& ccbRelCursor = ccbRelQuery->execute();
  // loop over all full configurations
  while( ccbRelCursor.next() ) {
    const coral::AttributeList& row = ccbRelCursor.currentRow();
    int cfg     = row["CONFKEY"   ].data<int>();
    int ccb     = row["CCBID"     ].data<int>();
    int key     = row["CONFCCBKEY"].data<int>();
    // check for used configurations
//    if ( cfgMap.find( cfg ) == cfgMap.end() ) continue;
    if ( userDiscardedKey( cfg ) ) continue;
    std::map<int,std::map<int,int>*>::const_iterator keyIter =
                                                     keyMap.find( cfg );
    std::map<int,std::map<int,int>*>::const_iterator keyIend =
                                                     keyMap.end();
    std::map<int,int>* mapPtr = 0;
    // check for new full configuration
    if ( keyIter != keyIend ) mapPtr = keyIter->second;
    else                      keyMap.insert(
                              std::pair<int,std::map<int,int>*>( cfg,
                              mapPtr = new std::map<int,int> ) );
    // store ccb config key
    std::map<int,int>& mapRef( *mapPtr );
    mapRef.insert( std::pair<int,int>( ccb, key ) );
    // check for new ccb config key
    if ( cckMap.find( key ) == cckMap.end() )
         cckMap.insert( std::pair<int,int>( key, ccb ) );
  }

  // get brick keys
  std::cout << "retrieve CCB configuration bricks" << std::endl;
  std::map<int,std::vector<int>*> brkMap;
  coral::ITable& confBrickTable =
    isession->nominalSchema().tableHandle( "CFG2BRKREL" );
  std::auto_ptr<coral::IQuery>
    confBrickQuery( confBrickTable.newQuery() );
  confBrickQuery->addToOutputList( "CONFID" );
  confBrickQuery->addToOutputList( "BRKID"  );
  coral::ICursor& confBrickCursor = confBrickQuery->execute();
  // loop over all brick keys
  while( confBrickCursor.next() ) {
    const coral::AttributeList& row = confBrickCursor.currentRow();
    int key = row["CONFID"].data<int>();
    int brk = row["BRKID" ].data<int>();
    // check for used ccb config key
    if ( cckMap.find( key ) == cckMap.end() ) continue;
    std::map<int,std::vector<int>*>::const_iterator brkIter =
                                                    brkMap.find( key );
    std::map<int,std::vector<int>*>::const_iterator brkIend =
                                                    brkMap.end();
    // check for new ccb config key
    std::vector<int>* brkPtr = 0;
    if ( brkIter != brkIend ) brkPtr = brkIter->second;
    else                      brkMap.insert(
                              std::pair<int,std::vector<int>*>( key,
                              brkPtr = new std::vector<int> ) );
    // store brick key
    brkPtr->push_back( brk );
  }

/*
  // loop over new runs
  std::map<int,int>::const_iterator runIter = runMap.begin();
  std::map<int,int>::const_iterator runIend = runMap.end();
  while ( runIter != runIend ) {
    const std::pair<int,int>& runEntry = *runIter++;
    // get full configuration
    int run = runEntry.first;
//    int cfg = runEntry.second;
    int rhc = runEntry.second;
    std::map<int,std::vector<DTConfigKey>*>::const_iterator
             rhcIter = rhcMap.find( rhc );
    std::map<int,std::vector<DTConfigKey>*>::const_iterator
             rhcIend = rhcMap.end();
    std::vector<DTConfigKey>& cfl = *( rhcIter->second );
    if ( sameConfigList( cfl, lastKey ) ) continue;
//    if ( cfg == lastKey ) continue;
    lastKey = cfl;
    std::cout << "retrieve configuration bricks for run " << run
              << " ---> RH " << rhc << std::endl;
    DTCCBConfig* fullConf = new DTCCBConfig( dataTag );
    // set run and full configuration in payload
    fullConf->setStamp(   run );
    fullConf->setFullKey( cfl );

    std::vector<DTConfigKey>::const_iterator cfgIter = cfl.begin();
    std::vector<DTConfigKey>::const_iterator cfgIend = cfl.end();
    while ( cfgIter != cfgIend ) {
    const DTConfigKey& cfgEntry = *cfgIter++;
    int cft = cfgEntry.confType;
    int cfg = cfgEntry.confKey;

    // retrieve ccb config map
    std::map<int,std::map<int,int>*>::const_iterator keyIter =
                                                     keyMap.find( cfg );
    std::map<int,std::map<int,int>*>::const_iterator keyIend =
                                                     keyMap.end();
    std::map<int,int>* mapPtr = 0;
    if ( keyIter != keyIend ) mapPtr = keyIter->second;
    if ( mapPtr == 0 ) continue;
    // loop over ccb
    std::map<int,int>::const_iterator ccbIter = mapPtr->begin();
    std::map<int,int>::const_iterator ccbIend = mapPtr->end();
    while ( ccbIter != ccbIend ) {
      const std::pair<int,int>& ccbEntry = *ccbIter++;
      // get ccb config key
      int ccb = ccbEntry.first;
      int key = ccbEntry.second;
      // retrieve chamber id
      std::map<int,DTCCBId>::const_iterator ccbIter = ccbMap.find( ccb );
      std::map<int,DTCCBId>::const_iterator ccbIend = ccbMap.end();
      if ( ccbIter == ccbIend ) continue;
      const DTCCBId& chaId = ccbIter->second;
      // retrieve brick id list
      std::map<int,std::vector<int>*>::const_iterator brkIter =
                                                      brkMap.find( key );
      std::map<int,std::vector<int>*>::const_iterator brkIend =
                                                      brkMap.end();
      if ( brkIter == brkIend ) continue;
      std::vector<int>* brkPtr = brkIter->second;
      if ( brkPtr == 0 ) continue;
//      const std::vector<int>& bkFull ( *brkPtr );
      // brick id lists in payload
      std::vector<int> bkList;
      bkList.reserve( 20 );
      std::map<int,int>::const_iterator bktIter = bktMap.begin();
      std::map<int,int>::const_iterator bktIend = bktMap.end();
      std::vector<int>::const_iterator bkiIter = brkPtr->begin();
      std::vector<int>::const_iterator bkiIend = brkPtr->end();
      while ( bkiIter != bkiIend ) {
        int brickId = *bkiIter++;
        bktIter = bktMap.find( brickId );
	if ( bktIter == bktIend ) continue;
        if ( bktIter->second == cft ) bkList.push_back( brickId );
      }
      fullConf->appendConfigKey( chaId.wheelId,
                                 chaId.stationId,
                                 chaId.sectorId,
                                 bkList );
    }
    }
    cond::Time_t snc = runEntry.first;
    m_to_transfer.push_back( std::make_pair( fullConf, snc ) );
    std::cout << "writing payload : " << sizeof( *fullConf ) 
              << " ( " << ( fullConf->end() - fullConf->begin() )
              << " ) " << std::endl;
  }
*/

  // set run and full configuration in payload
  DTCCBConfig* fullConf = new DTCCBConfig( dataTag );
  fullConf->setStamp( 1 );
  fullConf->setFullKey( userConf );
  std::map<int,bool> userBricks;
  std::map<int,bool>::const_iterator uBrkIter = userBricks.begin();
  std::map<int,bool>::const_iterator uBrkIend = userBricks.end();
  std::vector<DTConfigKey>::const_iterator cfgIter = userConf.begin();
  std::vector<DTConfigKey>::const_iterator cfgIend = userConf.end();
  while ( cfgIter != cfgIend ) {
    const DTConfigKey& cfgEntry = *cfgIter++;
    int cft = cfgEntry.confType;
    int cfg = cfgEntry.confKey;
    // retrieve ccb config map
    std::map<int,std::map<int,int>*>::const_iterator keyIter =
                                                     keyMap.find( cfg );
    std::map<int,std::map<int,int>*>::const_iterator keyIend =
                                                     keyMap.end();
    std::map<int,int>* mapPtr = 0;
    if ( keyIter != keyIend ) mapPtr = keyIter->second;
    if ( mapPtr == 0 ) continue;
    std::map<int,int>::const_iterator ccbIter = mapPtr->begin();
    std::map<int,int>::const_iterator ccbIend = mapPtr->end();
    while ( ccbIter != ccbIend ) {
      const std::pair<int,int>& ccbEntry = *ccbIter++;
      // get ccb config key
      int ccb = ccbEntry.first;
      int key = ccbEntry.second;
      // retrieve chamber id
      std::map<int,DTCCBId>::const_iterator ccbIter = ccbMap.find( ccb );
      std::map<int,DTCCBId>::const_iterator ccbIend = ccbMap.end();
      if ( ccbIter == ccbIend ) continue;
      const DTCCBId& chaId = ccbIter->second;
      // retrieve brick id list
      std::map<int,std::vector<int>*>::const_iterator brkIter =
                                                      brkMap.find( key );
      std::map<int,std::vector<int>*>::const_iterator brkIend =
                                                      brkMap.end();
      if ( brkIter == brkIend ) continue;
      std::vector<int>* brkPtr = brkIter->second;
      if ( brkPtr == 0 ) continue;
      // brick id lists in payload
      std::vector<int> bkList;
      bkList.reserve( 20 );
      std::map<int,int>::const_iterator bktIter = bktMap.begin();
      std::map<int,int>::const_iterator bktIend = bktMap.end();
      std::vector<int>::const_iterator bkiIter = brkPtr->begin();
      std::vector<int>::const_iterator bkiIend = brkPtr->end();
      while ( bkiIter != bkiIend ) {
        int brickId = *bkiIter++;
        bktIter = bktMap.find( brickId );
	if ( bktIter == bktIend ) continue;
        if ( bktIter->second == cft ) {
          bkList.push_back( brickId );
          uBrkIter = userBricks.find( brickId );
          if ( uBrkIter == uBrkIend ) 
              userBricks.insert( std::pair<int,bool>( brickId, true ) );
        }
      }
      fullConf->appendConfigKey( chaId.wheelId,
                                 chaId.stationId,
                                 chaId.sectorId,
                                 bkList );
    }
  }
  cond::Time_t snc = dataRun;
  m_to_transfer.push_back( std::make_pair( fullConf, snc ) );
  std::cout << "writing payload : " << sizeof( *fullConf ) 
            << " ( " << ( fullConf->end() - fullConf->begin() )
            << " ) " << std::endl;
  chkConfigList( userBricks );

  delete m_connection;
  delete coralSession;

  return;

}


void DTUserConfigHandler::chkConfigList(
                          const std::map<int,bool>& userBricks ) {

  std::cout << "check for available configurations " << std::endl
            << "create session: "
            << offlineConnect << " , "
            << offlineCatalog << " , "
            << offlineAuthentication << std::endl;

  DTDBSession* session = new DTDBSession( offlineConnect, offlineCatalog,
                                          offlineAuthentication );
  session->connect( false );
  const DTConfigList* rs;
//  std::cout << "read full list: " << listToken << std::endl;
  DTConfigHandler* ri = DTConfigHandler::create( session, listToken );
  rs = ri->getContainer();

  std::map<int,bool>::const_iterator uBrkIter = userBricks.begin();
  std::map<int,bool>::const_iterator uBrkIend = userBricks.end();

/*
  std::map<int,bool> activeConfigMap;
  coral::ITable& fullConfigTable =
    isession->nominalSchema().tableHandle( "CONFIGSETS" );
  std::auto_ptr<coral::IQuery>
    fullConfigQuery( fullConfigTable.newQuery() );
  fullConfigQuery->addToOutputList( "CONFKEY" );
  fullConfigQuery->addToOutputList( "NAME" );
  fullConfigQuery->addToOutputList( "RUN" );
  coral::ICursor& fullConfigCursor = fullConfigQuery->execute();
  while( fullConfigCursor.next() ) {
    const coral::AttributeList& row = fullConfigCursor.currentRow();
    int fullConfigId = row["CONFKEY"].data<int>();
    int fullConfigRN = row["RUN"    ].data<int>();
    if ( fullConfigRN ) activeConfigMap.insert(
                        std::pair<int,bool>( fullConfigId, true ) );
    else                activeConfigMap.insert(
                        std::pair<int,bool>( fullConfigId, false ) );
    std::string fullConfigName = row["NAME"].data<std::string>();
//    std::cout << "config " << fullConfigId
//              << " : "     << fullConfigName << std::endl;
  }

//  std::cout << " =============== CCB config list" << std::endl;
  std::map<int,bool> activeCCBCfgMap;
  coral::ITable& fullCCBCfgTable =
    isession->nominalSchema().tableHandle( "CCBRELATIONS" );
  std::auto_ptr<coral::IQuery>
    fullCCBCfgQuery( fullCCBCfgTable.newQuery() );
  fullCCBCfgQuery->addToOutputList( "CONFKEY" );
  fullCCBCfgQuery->addToOutputList( "CONFCCBKEY" );
  coral::ICursor& fullCCBCfgCursor = fullCCBCfgQuery->execute();
  while( fullCCBCfgCursor.next() ) {
    const coral::AttributeList& row = fullCCBCfgCursor.currentRow();
    int fullConfigId = row["CONFKEY"   ].data<int>();
    int fullCCBCfgId = row["CONFCCBKEY"].data<int>();
    std::map<int,bool>::const_iterator cfgIter =
                                       activeConfigMap.find( fullConfigId );
    if ( cfgIter == activeConfigMap.end() ) continue;
    if ( activeCCBCfgMap.find( fullCCBCfgId ) ==
         activeCCBCfgMap.end() ) 
         activeCCBCfgMap.insert( std::pair<int,bool>( fullCCBCfgId, true ) );
  }

//  std::cout << " =============== config brick list" << std::endl;
  std::map<int,bool> activeCfgBrkMap;
  coral::ITable& ccbConfBrkTable =
    isession->nominalSchema().tableHandle( "CFG2BRKREL" );
  std::auto_ptr<coral::IQuery>
    ccbConfBrickQuery( ccbConfBrkTable.newQuery() );
  ccbConfBrickQuery->addToOutputList( "CONFID" );
  ccbConfBrickQuery->addToOutputList( "BRKID" );
  coral::ICursor& ccbConfBrickCursor = ccbConfBrickQuery->execute();
  while( ccbConfBrickCursor.next() ) {
    const coral::AttributeList& row = ccbConfBrickCursor.currentRow();
    int fullCCBCfgId = row["CONFID"].data<int>();
    int ccbConfBrkId = row["BRKID" ].data<int>();
    std::map<int,bool>::const_iterator ccbIter =
                                       activeCCBCfgMap.find( fullCCBCfgId );
    if ( ccbIter == activeCCBCfgMap.end() ) continue;
    if ( !( ccbIter->second ) ) continue;
    if ( activeCfgBrkMap.find( ccbConfBrkId ) ==
         activeCfgBrkMap.end() )
         activeCfgBrkMap.insert( std::pair<int,bool>( ccbConfBrkId, true ) );
  }

//  std::cout << " ===============" << std::endl;
*/

  coral::ITable& brickConfigTable =
    isession->nominalSchema().tableHandle( "CFGBRICKS" );
  std::auto_ptr<coral::IQuery>
    brickConfigQuery( brickConfigTable.newQuery() );
  brickConfigQuery->addToOutputList( "BRKID" );
  brickConfigQuery->addToOutputList( "BRKNAME" );
  coral::ICursor& brickConfigCursor = brickConfigQuery->execute();
  DTConfigData* brickData = 0;
//  char brickIdString[20];
  std::vector<int> missingList;
  while( brickConfigCursor.next() ) {
    const coral::AttributeList& row = brickConfigCursor.currentRow();
    int brickConfigId = row["BRKID"].data<int>();
    uBrkIter = userBricks.find( brickConfigId );
    if ( uBrkIter == uBrkIend ) continue;
    if ( !( uBrkIter->second ) ) continue;
//    std::map<int,bool>::const_iterator brkIter =
//                                       activeCfgBrkMap.find( brickConfigId );
//    if ( brkIter == activeCfgBrkMap.end() ) continue;
//    if ( !( brkIter->second ) ) continue;
    std::string brickConfigName = row["BRKNAME"].data<std::string>();
//    std::cout << "brick " << brickConfigId
//              << " : "    << brickConfigName << std::endl;
    ri->get( brickConfigId, brickData );
    if ( brickData == 0 ) {
      std::cout << "brick " << brickConfigId << " missing, copy request"
                << std::endl;
      missingList.push_back( brickConfigId );
    }
  }

  std::vector<int>::const_iterator brickIter = missingList.begin();
  std::vector<int>::const_iterator brickIend = missingList.end();
  while ( brickIter != brickIend ) {
    int brickConfigId = *brickIter++;
    coral::AttributeList bindVariableList;
    bindVariableList.extend( "brickId", typeid(int) );
    bindVariableList["brickId"].data<int>() = brickConfigId;
    std::auto_ptr<coral::IQuery>
           brickDataQuery( isession->nominalSchema().newQuery() );
    brickDataQuery->addToTableList( "CFGRELATIONS" );
    brickDataQuery->addToTableList( "CONFIGCMDS" );
    std::string
    brickCondition  =      "CONFIGCMDS.CMDID=CFGRELATIONS.CMDID";
    brickCondition += " and CFGRELATIONS.BRKID=:brickId";
    brickDataQuery->addToOutputList( "CFGRELATIONS.BRKID" );
    brickDataQuery->addToOutputList( "CONFIGCMDS.CONFDATA" );
    brickDataQuery->setCondition( brickCondition, bindVariableList );
    coral::ICursor& brickDataCursor = brickDataQuery->execute();
    brickData = new DTConfigData();
    brickData->setId( brickConfigId );
    while( brickDataCursor.next() ) {
      const coral::AttributeList& row = brickDataCursor.currentRow();
      brickData->add( row["CONFIGCMDS.CONFDATA"].data<std::string>() );
    }
    cond::TypedRef<DTConfigData> brickRef( *session->poolDB(), brickData );
    brickRef.markWrite( "DTConfigData" );
    ri->set( brickConfigId, brickRef.token() );
  }

  std::cout << "brick list updated, disconnect session" << std::endl;
//  std::cout << "disconnect session..." << std::endl;
  session->disconnect();
//  std::cout << "delete session..." << std::endl;
  delete session;
//  std::cout << "list updated..." << std::endl;

  return;

}


std::string DTUserConfigHandler::id() const {
  return dataTag;
}


bool DTUserConfigHandler::sameConfigList(
                         const std::vector<DTConfigKey>& cfgl,
                         const std::vector<DTConfigKey>& cfgr ) {
  if ( cfgl.size() != cfgr.size() ) return false;
  std::map<int,int> lmap;
  std::vector<DTConfigKey>::const_iterator lIter = cfgl.begin();
  std::vector<DTConfigKey>::const_iterator lIend = cfgl.end();
  while ( lIter != lIend ) {
    const DTConfigKey& entry = *lIter++;
    lmap.insert( std::pair<int,int>( entry.confType, entry.confKey ) );
  }
  std::map<int,int> rmap;
  std::vector<DTConfigKey>::const_iterator rIter = cfgr.begin();
  std::vector<DTConfigKey>::const_iterator rIend = cfgr.end();
  while ( rIter != rIend ) {
    const DTConfigKey& entry = *rIter++;
    rmap.insert( std::pair<int,int>( entry.confType, entry.confKey ) );
  }
  std::map<int,int>::const_iterator lmIter = lmap.begin();
  std::map<int,int>::const_iterator lmIend = lmap.end();
  std::map<int,int>::const_iterator rmIter = rmap.begin();
  std::map<int,int>::const_iterator rmIend = rmap.end();
  while ( ( lmIter != lmIend ) &&
          ( rmIter != rmIend ) ) {
    const std::pair<int,int>& lEntry = *lmIter++;
    const std::pair<int,int>& rEntry = *rmIter++;
    if ( lEntry.first  != rEntry.first  ) return false;
    if ( lEntry.second != rEntry.second ) return false;
  }
  return true;
}


bool DTUserConfigHandler::userDiscardedKey( int key ) {
  std::vector<DTConfigKey>::const_iterator iter = userConf.begin();
  std::vector<DTConfigKey>::const_iterator iend = userConf.end();
  while ( iter != iend ) {
    const DTConfigKey& entry = *iter++;
    if ( entry.confKey == key ) return false;
  }
  return true;
}

