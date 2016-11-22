# This is an example PAT configuration showing the usage of caloTaus

# Starting with a skeleton process which gets imported with the following line
from PhysicsTools.PatAlgos.patTemplate_cfg import *

# switch to CaloTau (outputs will be allLayer0CaloTaus, allLayer1Taus, selectedLayer1Taus)
from PhysicsTools.PatAlgos.tools.tauTools import switchToCaloTau
switchToCaloTau(process)

# let it run
process.p = cms.Path(
            process.patDefaultSequence
            )

# In addition you usually want to change the following parameters:
#
#   process.GlobalTag.globaltag =  ...      (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
#   process.source.fileNames = [ ... ]      (e.g. 'file:AOD.root')
#   process.maxEvents.input = ...           (e.g. -1 to run on all events)
#   process.out.outputCommands = [ ... ]    (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
#   process.out.fileName = ...              (e.g. 'myTuple.root')
#   process.options.wantSummary = False     (to suppress the long output at the end of the job)

