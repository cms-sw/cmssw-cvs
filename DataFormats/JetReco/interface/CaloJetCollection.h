// F.R.
// $Id: CaloJetCollection.h,v 1.7 2007/08/24 17:35:23 fedor Exp $
#ifndef JetReco_CaloJetCollection_h
#define JetReco_CaloJetCollection_h

#include <vector>
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"

#include "DataFormats/JetReco/interface/CaloJet.h"

namespace reco {
  /// collection of CaloJet objects 
  typedef std::vector<CaloJet> CaloJetCollection;
  /// edm references
  typedef edm::Ref<CaloJetCollection> CaloJetRef;
  typedef edm::RefVector<CaloJetCollection> CaloJetRefVector;
  typedef edm::RefProd<CaloJetCollection> CaloJetRefProd;
}
#endif
