//
// $Id: Centrality.h,v 1.5 2009/09/08 12:33:11 edwenger Exp $
//

#ifndef DataFormats_Centrality_h
#define DataFormats_Centrality_h

#include <string>
#include <vector>

namespace reco { class Centrality {
public:
  Centrality(double d=0, std::string label = "");
  virtual ~Centrality();

  double    raw()               const { return value_; }
  std::string label()           const { return label_; }

private:
  double value_;
  std::string label_;

};

 typedef std::vector<reco::Centrality> CentralityCollection;

}

#endif 


