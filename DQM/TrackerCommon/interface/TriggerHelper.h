#ifndef TriggerHelper_H
#define TriggerHelper_H


// -*- C++ -*-
//
// Package:    DQM/TrackerCommon
// Class:      TriggerHelper
//
// $Id: TriggerHelper.h,v 1.6 2010/03/08 20:53:57 vadler Exp $
//
/**
  \class    TriggerHelper TriggerHelper.h "DQM/TrackerCommon/interface/TriggerHelper.h"
  \brief    Provides a code based selection for trigger and DCS information in order to have no failing filters in the CMSSW path.

   [...]

  \author   Volker Adler
  \version  $Id: TriggerHelper.h,v 1.6 2010/03/08 20:53:57 vadler Exp $
*/


#include <string>

#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

class TriggerHelper {

    L1GtUtils         l1Gt_;
    HLTConfigProvider hltConfig_;
    bool              hltConfigInit_;
    // Configuration parameters
    bool andOr_;
    bool               andOrDcs_;
    edm::InputTag      dcsInputTag_;
    std::vector< int > dcsPartitions_;
    bool               errorReplyDcs_;
    bool                       andOrGt_;
    edm::InputTag              gtInputTag_;
    std::vector< std::string > gtLogicalExpressions_;
    bool                       errorReplyGt_;
    bool                       andOrL1_;
    std::vector< std::string > l1LogicalExpressions_;
    bool                       errorReplyL1_;
    bool                       andOrHlt_;
    edm::InputTag              hltInputTag_;
    std::vector< std::string > hltLogicalExpressions_;
    bool                       errorReplyHlt_;
    // Switches
    bool on_;
    bool onDcs_;
    bool onGt_;
    bool onL1_;
    bool onHlt_;

  public:

    // Constructors and destructor
    TriggerHelper( const edm::ParameterSet & config ); // To be called from the ED module's c'tor
    ~TriggerHelper() {};

    // Public methods
    void initRun( const edm::Run & run, const edm::EventSetup & setup );    // To be called from beginRun() methods
    bool accept( const edm::Event & event, const edm::EventSetup & setup ); // To be called from analyze/filter() methods

  private:

    // Private methods

    // DCS
    bool acceptDcs( const edm::Event & event );
    bool acceptDcsPartition( const edm::Handle< DcsStatusCollection > & dcsStatus, int dcsPartition ) const;

    // GT status bits
    bool acceptGt( const edm::Event & event );
    bool acceptGtLogicalExpression( const edm::Handle< L1GlobalTriggerReadoutRecord > & gtReadoutRecord, std::string gtLogicalExpression );

    // L1
    bool acceptL1( const edm::Event & event, const edm::EventSetup & setup );
    bool acceptL1LogicalExpression( const edm::Event & event, std::string l1LogicalExpression );

    // HLT
    bool acceptHlt( const edm::Event & event );
    bool acceptHltLogicalExpression( const edm::Handle< edm::TriggerResults > & hltTriggerResults, std::string hltLogicalExpression ) const;

    // Algos
    bool negate( std::string & word ) const;

};


#endif
