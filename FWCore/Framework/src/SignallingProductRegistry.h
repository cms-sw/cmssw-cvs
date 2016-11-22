#ifndef Framework_SignallingProductRegistry_h
#define Framework_SignallingProductRegistry_h
// -*- C++ -*-
//
// Package:     Framework
// Class  :     SignallingProductRegistry
// 
/**\class SignallingProductRegistry SignallingProductRegistry.h FWCore/Framework/interface/SignallingProductRegistry.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  Chris Jones
//         Created:  Fri Sep 23 16:47:10 CEST 2005
// $Id: SignallingProductRegistry.h,v 1.6 2007/03/04 06:10:25 wmtan Exp $
//

// system include files
#include "sigc++/signal.h"
#include <map>
#include <string>

// user include files
#include "DataFormats/Provenance/interface/ProductRegistry.h"

// forward declarations
namespace edm {
   class SignallingProductRegistry : public ProductRegistry
   {

   public:
      SignallingProductRegistry() : ProductRegistry(), productAddedSignal_(), typeAddedStack_() {}
      sigc::signal<void, BranchDescription const&> productAddedSignal_;
      
   private:
      SignallingProductRegistry(const SignallingProductRegistry&); // stop default

      const SignallingProductRegistry& operator=(const SignallingProductRegistry&); // stop default

      virtual void addCalled(BranchDescription const&, bool);
      // ---------- member data --------------------------------
      std::map<std::string, unsigned int> typeAddedStack_;
   };
}

#endif
