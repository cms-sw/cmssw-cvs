//
// Original Author:  Fedor Ratnikov Feb. 16, 2007
// $Id: SimpleJetCorrector.cc,v 1.1 2007/02/16 23:32:16 fedor Exp $
//
// Simplest jet corrector scaling every jet by fixed factor
//

#include "JetMETCorrections/Objects/interface/SimpleJetCorrector.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

SimpleJetCorrector::SimpleJetCorrector (const edm::ParameterSet& fConfig) 
  : mScale (fConfig.getParameter <double> ("scale"))
{}

SimpleJetCorrector::~SimpleJetCorrector () {}
