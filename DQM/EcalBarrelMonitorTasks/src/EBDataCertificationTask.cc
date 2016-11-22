#include <iostream>
#include <algorithm>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"
#include "CondFormats/RunInfo/interface/RunSummary.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"

#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include <DQM/EcalCommon/interface/Numbers.h>
#include "DQM/EcalCommon/interface/UtilsClient.h"

#include "DQM/EcalBarrelMonitorTasks/interface/EBDataCertificationTask.h"

EBDataCertificationTask::EBDataCertificationTask(const edm::ParameterSet& ps) {

  dqmStore_ = edm::Service<DQMStore>().operator->();

  // cloneME switch
  cloneME_ = ps.getUntrackedParameter<bool>("cloneME", true);

  prefixME_ = ps.getUntrackedParameter<std::string>("prefixME", "");

  enableCleanup_ = ps.getUntrackedParameter<bool>("enableCleanup", false);

  mergeRuns_ = ps.getUntrackedParameter<bool>("mergeRuns", false);

  meEBDataCertificationSummary_ = 0;
  meEBDataCertificationSummaryMap_ = 0;
  for (int i = 0; i < 36; i++) {
    meEBDataCertification_[i] = 0;
  }

  hDQM_ = 0;
  hDAQ_ = 0;
  hDCS_ = 0;
  hIntegrityByLumi_ = 0;
  hFrontendByLumi_ = 0;

}

EBDataCertificationTask::~EBDataCertificationTask() {

}

void EBDataCertificationTask::beginJob(void){

  char histo[200];
  
  if ( dqmStore_ ) {

    dqmStore_->setCurrentFolder(prefixME_ + "/EventInfo");

    sprintf(histo, "CertificationSummary");
    meEBDataCertificationSummary_ = dqmStore_->bookFloat(histo);
    meEBDataCertificationSummary_->Fill(-1.0);

    sprintf(histo, "CertificationSummaryMap");
    meEBDataCertificationSummaryMap_ = dqmStore_->book2D(histo,histo, 72, 0., 72., 34, 0., 34.);
    meEBDataCertificationSummaryMap_->setAxisTitle("jphi", 1);
    meEBDataCertificationSummaryMap_->setAxisTitle("jeta", 2);
    
    dqmStore_->setCurrentFolder(prefixME_ + "/EventInfo/CertificationContents");

    for (int i = 0; i < 36; i++) {
      sprintf(histo, "EcalBarrel_%s", Numbers::sEB(i+1).c_str());
      meEBDataCertification_[i] = dqmStore_->bookFloat(histo);
      meEBDataCertification_[i]->Fill(-1.0);
    }

  }

}

void EBDataCertificationTask::endJob(void) {

  if ( enableCleanup_ ) this->cleanup();

}

void EBDataCertificationTask::beginLuminosityBlock(const edm::LuminosityBlock& lumiBlock, const  edm::EventSetup& iSetup){

}

void EBDataCertificationTask::endLuminosityBlock(const edm::LuminosityBlock&  lumiBlock, const  edm::EventSetup& iSetup) {

  this->reset();

  char histo[200];
  
  MonitorElement* me;

  // evaluate the DQM quality of observables checked by lumi
  float DQMVal[36];
  for (int i = 0; i < 36; i++) {
    DQMVal[i] = 0.;
  }

  sprintf(histo, (prefixME_ + "/EBIntegrityTask/EBIT weighted integrity errors by lumi").c_str());
  me = dqmStore_->get(histo);
  hIntegrityByLumi_ = UtilsClient::getHisto<TH1F*>( me, cloneME_, hIntegrityByLumi_ );

  sprintf(histo, (prefixME_ + "/EBStatusFlagsTask/FEStatus/EBSFT weighted frontend errors by lumi").c_str());
  me = dqmStore_->get(histo);
  hFrontendByLumi_ = UtilsClient::getHisto<TH1F*>( me, cloneME_, hFrontendByLumi_ );

  float integrityErrSum, frontendErrSum;
  integrityErrSum = frontendErrSum = 0.;
  for ( int i=0; i<36; i++) {
    float ismIntegrityQual = 1.0;
    if( hIntegrityByLumi_ && hIntegrityByLumi_->GetBinContent(0) > 0 ) {
      float errors = hIntegrityByLumi_->GetBinContent(i+1);
      ismIntegrityQual = 1.0 - errors/hIntegrityByLumi_->GetBinContent(0);
      integrityErrSum += errors;
    }
    float ismFrontendQual = 1.0;
    if( hFrontendByLumi_ && hFrontendByLumi_->GetBinContent(0) > 0 ) {
      float errors = hFrontendByLumi_->GetBinContent(i+1);
      ismFrontendQual = 1.0 - errors/hFrontendByLumi_->GetBinContent(0);
      frontendErrSum += errors;
    }
    DQMVal[i] = std::min(ismIntegrityQual,ismFrontendQual);

    sprintf(histo, "EcalBarrel_%s", Numbers::sEB(i+1).c_str());
    me = dqmStore_->get(prefixME_ + "/EventInfo/reportSummaryContents/" + histo);
    if( me ) me->Fill(DQMVal[i]);

    sprintf(histo, "reportSummaryMap");
    me = dqmStore_->get(prefixME_ + "/EventInfo/" + histo );

    if( me ) {
      for( int iett=0; iett<17; iett++ ) {
        for( int iptt=0; iptt<4; iptt++ ) {
          int iettx = (i-1)*17 + iett + 1;
          int ipttx = (i-1)*4 + iptt + 1;
          me->setBinContent( ipttx, iettx, DQMVal[i]);
        }
      }
    }

  }

  float totDQMVal = 0.;

  float integrityQual = 1.0;
  if( hIntegrityByLumi_ && hIntegrityByLumi_->GetBinContent(0) > 0 ) integrityQual = 1.0 - integrityErrSum/hIntegrityByLumi_->GetBinContent(0);
  float frontendQual = 1.0;
  if( hFrontendByLumi_ && hFrontendByLumi_->GetBinContent(0) > 0 ) frontendQual = 1.0 - frontendErrSum/hFrontendByLumi_->GetBinContent(0);

  totDQMVal = std::min(integrityQual,frontendQual);

  sprintf(histo, (prefixME_ + "/EventInfo/reportSummary").c_str());
  me = dqmStore_->get(histo);
  if( me ) me->Fill(totDQMVal);

  // now combine reduced DQM with DCS and DAQ
  sprintf(histo, (prefixME_ + "/EventInfo/DAQSummaryMap").c_str());
  me = dqmStore_->get(histo);
  hDAQ_ = UtilsClient::getHisto<TH2F*>( me, cloneME_, hDAQ_ );

  sprintf(histo, (prefixME_ + "/EventInfo/DCSSummaryMap").c_str());
  me = dqmStore_->get(histo);
  hDCS_ = UtilsClient::getHisto<TH2F*>( me, cloneME_, hDCS_ );
  
  float sumCert = 0.;
  float sumCertEB[36];
  int nValidChannels = 0;
  int nValidChannelsEB[36];

  for (int i = 0; i < 36; i++) {
    sumCertEB[i] = 0.;
    nValidChannelsEB[i] = 0;
  }

  for ( int iett = 0; iett < 34; iett++ ) {
    for ( int iptt = 0; iptt < 72; iptt++ ) {

      int ism = ( iett<17 ) ? iptt/4+1 : 18+iptt/4+1;
      
      float xvalDQM = DQMVal[ism-1];

      float xvalDAQ, xvalDCS;
      xvalDAQ = xvalDCS = -1.;
      float xcert = -1;

      if ( hDAQ_ ) xvalDAQ = hDAQ_->GetBinContent( iptt+1, iett+1 );
      if ( hDCS_ ) xvalDCS = hDCS_->GetBinContent( iptt+1, iett+1 );

      // all white means problems: DAQ and DCS not available and DQM empty
      if ( xvalDQM == -1 && xvalDAQ == -1 && xvalDCS == -1) xcert = 0.0;
      else {
        // do not consider the white value of DAQ and DCS (problems with DB)
        xcert = fabs(xvalDQM) * fabs(xvalDAQ) * fabs(xvalDCS);
      }

      if ( meEBDataCertificationSummaryMap_ ) meEBDataCertificationSummaryMap_->setBinContent( iptt+1, iett+1, xcert );

      sumCertEB[ism-1] += xcert;
      nValidChannelsEB[ism-1]++;

      sumCert += xcert;
      nValidChannels++;

    }
  }

  if( meEBDataCertificationSummary_ ) {
    if( nValidChannels>0 ) meEBDataCertificationSummary_->Fill( sumCert/nValidChannels );
    else meEBDataCertificationSummary_->Fill( 0.0 );
  }

  for (int i = 0; i < 36; i++) {
    if( meEBDataCertification_[i] ) {
      if( nValidChannelsEB[i]>0 ) meEBDataCertification_[i]->Fill( sumCertEB[i]/nValidChannelsEB[i] );
      else meEBDataCertification_[i]->Fill( 0.0 );
    }
  }

}

void EBDataCertificationTask::beginRun(const edm::Run& r, const edm::EventSetup& c) {

  if ( ! mergeRuns_ ) this->reset();

}

void EBDataCertificationTask::endRun(const edm::Run& r, const edm::EventSetup& c) {

  this->reset();

  char histo[200];
  
  MonitorElement* me;

  sprintf(histo, (prefixME_ + "/EventInfo/reportSummaryMap").c_str());
  me = dqmStore_->get(histo);
  hDQM_ = UtilsClient::getHisto<TH2F*>( me, cloneME_, hDQM_ );

  sprintf(histo, (prefixME_ + "/EventInfo/DAQSummaryMap").c_str());
  me = dqmStore_->get(histo);
  hDAQ_ = UtilsClient::getHisto<TH2F*>( me, cloneME_, hDAQ_ );

  sprintf(histo, (prefixME_ + "/EventInfo/DCSSummaryMap").c_str());
  me = dqmStore_->get(histo);
  hDCS_ = UtilsClient::getHisto<TH2F*>( me, cloneME_, hDCS_ );

  float sumCert = 0.;
  float sumCertEB[36];
  int nValidChannels = 0;
  int nValidChannelsEB[36];

  for (int i = 0; i < 36; i++) {
    sumCertEB[i] = 0.;
    nValidChannelsEB[i] = 0;
  }

  for ( int iett = 0; iett < 34; iett++ ) {
    for ( int iptt = 0; iptt < 72; iptt++ ) {

      float xvalDQM, xvalDAQ, xvalDCS;
      xvalDQM = xvalDAQ = xvalDCS = -1.;
      float xcert = -1;

      if ( hDQM_ ) xvalDQM = hDQM_->GetBinContent( iptt+1, iett+1 );
      if ( hDAQ_ ) xvalDAQ = hDAQ_->GetBinContent( iptt+1, iett+1 );
      if ( hDCS_ ) xvalDCS = hDCS_->GetBinContent( iptt+1, iett+1 );

      // all white means problems: DAQ and DCS not available and DQM empty
      if ( xvalDQM == -1 && xvalDAQ == -1 && xvalDCS == -1) xcert = 0.0;
      else {
        // do not consider the white value of DAQ and DCS (problems with DB)
        xcert = fabs(xvalDQM) * fabs(xvalDAQ) * fabs(xvalDCS);
      }

      if ( meEBDataCertificationSummaryMap_ ) meEBDataCertificationSummaryMap_->setBinContent( iptt+1, iett+1, xcert );

      int ism = ( iett<17 ) ? iptt/4+1 : 18+iptt/4+1;

      sumCertEB[ism-1] += xcert;
      nValidChannelsEB[ism-1]++;

      sumCert += xcert;
      nValidChannels++;

    }
  }

  if( meEBDataCertificationSummary_ ) {
    if( nValidChannels>0 ) meEBDataCertificationSummary_->Fill( sumCert/nValidChannels );
    else meEBDataCertificationSummary_->Fill( 0.0 );
  }

  for (int i = 0; i < 36; i++) {
    if( meEBDataCertification_[i] ) {
      if( nValidChannelsEB[i]>0 ) meEBDataCertification_[i]->Fill( sumCertEB[i]/nValidChannelsEB[i] );
      else meEBDataCertification_[i]->Fill( 0.0 );
    }
  }

}

void EBDataCertificationTask::reset(void) {

  if ( meEBDataCertificationSummary_ ) meEBDataCertificationSummary_->Reset();

  for (int i = 0; i < 36; i++) {
    if ( meEBDataCertification_[i] ) meEBDataCertification_[i]->Reset();
  }

  if ( meEBDataCertificationSummaryMap_ ) meEBDataCertificationSummaryMap_->Reset();
  
}


void EBDataCertificationTask::cleanup(void){

  if ( cloneME_ ) {
    if( hDQM_ ) delete hDQM_;
    if( hDAQ_ ) delete hDAQ_;
    if( hDCS_ ) delete hDCS_;
    if( hIntegrityByLumi_ ) delete hIntegrityByLumi_;
    if( hFrontendByLumi_ ) delete hFrontendByLumi_;
  }
  hDQM_ = 0;
  hDAQ_ = 0;
  hDCS_ = 0;
  hIntegrityByLumi_ = 0;
  hFrontendByLumi_ = 0;

  if ( dqmStore_ ) {
    dqmStore_->setCurrentFolder(prefixME_ + "/EventInfo");
    if ( meEBDataCertificationSummary_ ) dqmStore_->removeElement( meEBDataCertificationSummary_->getName() );
    if ( meEBDataCertificationSummaryMap_ ) dqmStore_->removeElement( meEBDataCertificationSummaryMap_->getName() );

    dqmStore_->setCurrentFolder(prefixME_ + "/EventInfo/CertificationContents");
    for (int i = 0; i < 36; i++) {
      if ( meEBDataCertification_[i] ) dqmStore_->removeElement( meEBDataCertification_[i]->getName() );
    }
  }

}

void EBDataCertificationTask::analyze(const edm::Event& e, const edm::EventSetup& c){ 

}
