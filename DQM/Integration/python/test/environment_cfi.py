import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQM_cfg import *

DQM.collectorHost = 'srv-c2d05-19.cms'
DQM.collectorPort = 9090

from DQMServices.Components.DQMEnvironment_cfi import *

dqmSaver.convention = 'Online'
dqmSaver.dirName = '/home/dqmprolocal/output'
dqmSaver.producer = 'DQM'
dqmSaver.saveByTime = 4
dqmSaver.saveByLumiSection = -1
dqmSaver.saveByMinute = 8
dqmSaver.saveByRun = -1
dqmSaver.saveAtJobEnd = False

