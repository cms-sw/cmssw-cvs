// -*- C++ -*-

#include "GeneratorInterface/Core/interface/GeneratorFilter.h"
#include "MockHadronizer.h"

namespace edmtest
{
  typedef edm::GeneratorFilter<edmtest::MockHadronizer> MockHadronizerGeneratorFilter;
}

using edmtest::MockHadronizerGeneratorFilter;
DEFINE_FWK_MODULE(MockHadronizerGeneratorFilter);

