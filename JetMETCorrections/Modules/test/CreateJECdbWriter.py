#! /usr/bin/env python
import os
#******************   definitions  **********************************
ERA         = 'Summer09_7TeV_ReReco332'
TYPE_LIST   = ['JPT','Calo','PF']
ALGO_LIST   = ['IC5','AK5','AK7','KT4','KT6']
LEVEL_LIST  = ['L2Relative','L3Absolute']
FLAVOR_LIST = ['bJ','cJ','qJ','gJ','bT','cT','qT','gT']
PARTON_LIST = ['bJ','cJ','qJ','gJ','jJ','bT','cT','qT','tT']
#*********************************************************************
filename = 'JetCorrectionDBWriter_cfg.py'
file = open(filename,'w')
file.write('import FWCore.ParameterSet.Config as cms \n')
file.write('process = cms.Process(\'jecdb\') \n')
file.write('process.load(\'CondCore.DBCommon.CondDBCommon_cfi\') \n')
file.write('process.CondDBCommon.connect = \'sqlite_file:JEC_'+ERA+'.db\' \n')
file.write('process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1)) \n')
file.write('process.source = cms.Source(\'EmptySource\') \n')
file.write('process.PoolDBOutputService = cms.Service(\'PoolDBOutputService\', \n')
file.write('   process.CondDBCommon, \n')
file.write('   toPut = cms.VPSet( \n')
for ll in LEVEL_LIST: #loop for default corrections
  for aa in ALGO_LIST: #loop for jet algorithms
    for tt in TYPE_LIST: #loop for jet types
      if ((tt=='Calo') or (tt=='PF') or ((tt=='JPT') and ((aa=='IC5') or (aa=='AK5')))):
        ss = ll+'_'+aa+tt
        print ss 
        file.write('      cms.PSet( \n')
        file.write('         record = cms.string(\''+ss+'\'), \n')
        file.write('         tag    = cms.string(\''+ss+'\'), \n')
        file.write('         label  = cms.string(\''+ss+'\') \n')
        file.write('      ), \n') 

for ff in FLAVOR_LIST: #loop over flavor correction options
  ss = 'L5Flavor_IC5Calo_'+ff
  print ss 
  file.write('      cms.PSet( \n')
  file.write('         record = cms.string(\''+ss+'\'), \n')
  file.write('         tag    = cms.string(\''+ss+'\'), \n')
  file.write('         label  = cms.string(\''+ss+'\') \n')
  file.write('      ), \n')

for pp in PARTON_LIST: #loop over flavor correction options
  for aa in ALGO_LIST:
    ss = 'L7Parton_'+aa+'_'+pp
    print ss 
    file.write('      cms.PSet( \n')
    file.write('         record = cms.string(\''+ss+'\'), \n')
    file.write('         tag    = cms.string(\''+ss+'\'), \n')
    file.write('         label  = cms.string(\''+ss+'\') \n')
    file.write('      ), \n')

ss = 'L4EMF_AK5Calo'
print ss 
file.write('      cms.PSet( \n')
file.write('         record = cms.string(\''+ss+'\'), \n')
file.write('         tag    = cms.string(\''+ss+'\'), \n')
file.write('         label  = cms.string(\''+ss+'\') \n')
file.write('      ) \n')

file.write('   ) \n')
file.write(') \n')
MODULE_LIST = []
for ll in LEVEL_LIST: #loop for default corrections
  for aa in ALGO_LIST: #loop for jet algorithms
    for tt in TYPE_LIST: #loop for jet types
      if ((tt=='Calo') or (tt=='PF') or ((tt=='JPT') and ((aa=='IC5') or (aa=='AK5')))):
        ss = 'dbWriter'+ll+aa+tt
        MODULE_LIST.append(ss) 
        file.write('process.'+ss+' = cms.EDAnalyzer(\'JetCorrectorDBWriter\', \n')
        file.write('   inputTxtFile = cms.untracked.string(\''+ERA+'_'+ll+'_'+aa+tt+'.txt\'), \n')
        file.write('   option       = cms.untracked.string(\'\'), \n')
        file.write('   label        = cms.untracked.string(\''+ll+'_'+aa+tt+'\') \n')
        file.write(') \n')

for ff in FLAVOR_LIST:
  ss = 'dbWriterL5'+ff
  MODULE_LIST.append(ss) 
  file.write('process.'+ss+' = cms.EDAnalyzer(\'JetCorrectorDBWriter\', \n')
  file.write('   inputTxtFile = cms.untracked.string(\'L5Flavor_IC5Calo.txt\'), \n')
  file.write('   option       = cms.untracked.string(\''+ff+'\'), \n')
  file.write('   label        = cms.untracked.string(\'L5Flavor_IC5Calo\') \n')
  file.write(') \n')

for pp in PARTON_LIST:
  for aa in ALGO_LIST:
    ss = 'dbWriterL7'+aa+pp
    MODULE_LIST.append(ss) 
    file.write('process.'+ss+' = cms.EDAnalyzer(\'JetCorrectorDBWriter\', \n')
    file.write('   inputTxtFile = cms.untracked.string(\'L7Parton_'+aa+'.txt\'), \n')
    file.write('   option       = cms.untracked.string(\''+pp+'\'), \n')
    file.write('   label        = cms.untracked.string(\'L7Parton_'+aa+'\') \n')
    file.write(') \n')

ss = 'dbWriterL4'
MODULE_LIST.append(ss) 
file.write('process.'+ss+' = cms.EDAnalyzer(\'JetCorrectorDBWriter\', \n')
file.write('   inputTxtFile = cms.untracked.string(\'L4EMF_AK5Calo.txt\'), \n')
file.write('   option       = cms.untracked.string(\'\'), \n')
file.write('   label        = cms.untracked.string(\'L4EMF_AK5Calo\') \n')
file.write(') \n')

file.write('process.p = cms.Path( \n')
path = ''
for mm in MODULE_LIST:
  if mm==MODULE_LIST[-1]:
    file.write('process.'+mm+'\n')
  else:
    file.write('process.'+mm+' * \n')
file.write(') \n')
file.close()
