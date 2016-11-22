//
// $Id: Centrality.cc,v 1.3 2009/08/18 14:45:32 yilmaz Exp $
//

#include "DataFormats/HeavyIonEvent/interface/Centrality.h"

using namespace reco;

Centrality::Centrality(double d, std::string label)
  : 
value_(d),
label_(label)
{
  // default constructor
}


Centrality::~Centrality()
{
}


