// -*- C++ -*-
//
// Package:     Framework
// Class  :     NoRecordException
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Fri Aug 28 13:27:25 CDT 2009
// $Id: NoRecordException.cc,v 1.1 2009/08/28 19:09:49 chrjones Exp $
//

// system include files

// user include files
#include "FWCore/Framework/interface/NoRecordException.h"


void
edm::eventsetup::no_record_exception_message_builder(cms::Exception& oException,const char* iName) {
   oException
   << "No \"" 
   << iName
   << "\" record found in the EventSetup.\n Please add an ESSource or ESProducer that delivers such a record.\n";
}   

void
edm::eventsetup::no_dependent_record_exception_message_builder(cms::Exception& oException, const EventSetupRecordKey& iKey, const char* iName) {
   oException
   << "No \"" 
   << iName
   << "\" record found in the dependent record \""<<iKey.type().name()
   << "\".\n Please add an ESSource or ESProducer that delivers the \""
   << iName<<"\" record.";
}
