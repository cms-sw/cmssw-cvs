#include "VisReco/VisRecoJets/interface/VisRecoJetContent.h"
#include "VisReco/VisRecoJets/interface/VisRecoCandidateContent.h"
#include "Iguana/Framework/interface/IgExtensionDB.h"
#include "Iguana/Framework/interface/IgModuleDef.h"

DEFINE_IGUANA_MODULE ();
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisRecoJetContent, VisRecoJetContent::catalogLabel ());
DEFINE_IGUANA_PLUGIN (IgExtensionDB, VisRecoCandidateContent, VisRecoCandidateContent::catalogLabel ());
