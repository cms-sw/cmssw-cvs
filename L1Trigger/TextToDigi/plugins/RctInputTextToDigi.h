#ifndef RCTINPUTTEXTTODIGI_H
#define RCTINPUTTEXTTODIGI_H

// -*- C++ -*-
//
// Class:      RctInputTextToDigi
// 
/**\class RctInputTextToDigi L1Trigger/TextToDigi/plugins/RctInputTextToDigi.cc L1Trigger/TextToDigi/plugins/RctInputTextToDigi.h

 Description: Creates an EcalTrigPrimDigiCollection and an
 HcalTrigPrimDigiCollection from a text file (formatted as 
 read out from saveRCTInput module), for input to the
 L1Trigger/RegionalCaloTrigger module.

*/

//
// Original Author:  jleonard
//         Created:  Fri Sep 21 16:16:27 CEST 2007
//
//


// system include files
#include <memory>
#include <string>
#include <fstream>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTLookupTables.h"
#include "CondFormats/L1TObjects/interface/L1RCTParameters.h"
#include "CondFormats/DataRecord/interface/L1RCTParametersRcd.h"

//
// class declaration
//

class RctInputTextToDigi : public edm::EDProducer {
public:
  explicit RctInputTextToDigi(const edm::ParameterSet&);
  ~RctInputTextToDigi();

private:
  virtual void beginJob();
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
      
  // ----------member data ---------------------------
  
  edm::FileInPath inputFile_;
  std::ifstream inputStream_;
  int nEvent_;
  bool oldVersion_;
  L1RCTLookupTables* lookupTables_;
};

#endif
