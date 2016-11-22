import FWCore.ParameterSet.Config as cms
from GeneratorInterface.ThePEGInterface.herwigDefaults_cff import *
from GeneratorInterface.ThePEGInterface.herwigValidation_cff import *

configurationMetadata = cms.untracked.PSet(
	version = cms.untracked.string('$Revision: 1.4 $'),
	name = cms.untracked.string('$Source: /afs/cern.ch/work/c/cmsbuild/public/cvs/CMSSW/CMSSW/GeneratorInterface/ThePEGInterface/test/testThePEGGeneratorFilter.py,v $'),
	annotation = cms.untracked.string('Herwig++ example - QCD validation')
)

generator = cms.EDProducer("ThePEGGeneratorFilter",
	herwigDefaultsBlock,
	herwigValidationBlock,

	configFiles = cms.vstring(
#		'MSSM.model'
	),

	parameterSets = cms.vstring(
		'cmsDefaults', 
#		'validationMSSM',
		'validationQCD'
	),
)
