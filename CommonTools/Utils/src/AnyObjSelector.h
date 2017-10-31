#ifndef CommonTools_Utils_AnyObjSelector_h
#define CommonTools_Utils_AnyObjSelector_h
#include "CommonTools/Utils/src/SelectorBase.h"

namespace reco {
  namespace parser {
    class AnyObjSelector : public SelectorBase {
      virtual bool operator()(const Reflex::Object & c) const { return true; }
    };
  }
}

#endif
