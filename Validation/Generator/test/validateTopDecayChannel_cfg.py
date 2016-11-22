from Validation.Generator.validationTemplate_cfg import *

## define process
process.load("Validation.Generator.TopDecayChannelChecker_cfi")

process.p = cms.Path(
  process.topDecayChannelChecker 
)

## ---
##    replace input file here
## ---
## process.source.fileNames = []


## ---
##    replace maximal number of events here
## ---
## process.maxEvents.input = -1


## ---
## replace option arguments here
## ---
## process.options.wantSummary = False


