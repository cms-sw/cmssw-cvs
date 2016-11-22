import FWCore.ParameterSet.Config as cms

VisConfigurationService = cms.Service('VisConfigurationService',
                                      EnabledTwigs = cms.untracked.vstring('/Objects/CMS Event and Detector/DTs/Wheel 0', 
                                                                           '/Objects/CMS Event and Detector/Muon DT Event/DT 4DSegment'),
                                      Views = cms.untracked.vstring('3D Window'),
                                      ContentProxies = cms.untracked.vstring('Framework/EventSetup', 
                                                                             'Reco/MuonDT', 
                                                                             'Reco/MuonCSC', 
                                                                             'Reco/MuonRPC', 
                                                                             'Reco/Ecal', 
                                                                             'Reco/Hcal', 
                                                                             'Reco/Tracker', 
                                                                             'Reco/Muon', 
                                                                             'Reco/Jets',
                                                                             'Reco/Trigger',
                                                                             'Reco/Detector',
                                                                             'Simulation/Hits'),
                                      EventDataCollections = cms.untracked.bool(False),
                                      TwigBrowser = cms.untracked.bool(True),
                                      TextBrowser = cms.untracked.bool(True),
                                      VisAnimate = cms.untracked.bool(False),
                                      VisAnnotateGeometry = cms.untracked.bool(False),
                                      VisAppMode = cms.untracked.string("normal"),
                                      VisAutoPrint = cms.untracked.bool(False),
                                      VisAutoStart = cms.untracked.bool(False),
                                      VisBField = cms.untracked.double(4),
                                      VisCaloAnnotation = cms.untracked.bool(True),
                                      VisCaloEnergyScale = cms.untracked.double(1),
                                      VisCounterFrequency = cms.untracked.double(0.5),
                                      VisCounterMax = cms.untracked.double(12500),
                                      VisCounterMin = cms.untracked.double(-2500),
                                      VisCuTkYSize = cms.untracked.uint32(200),
                                      VisCuTkXSize = cms.untracked.uint32(250),
                                      VisCuTkTrackProducer = cms.untracked.string("generalTracks"),
                                      VisCuTkStripRechitProducer = cms.untracked.string("siStripMatchedRecHits"),
                                      VisCuTkStripDigiProducer = cms.untracked.string("ProdStripDigi"),
                                      VisCuTkStripDigiLabel = cms.untracked.string(""),
                                      VisCuTkStripClusterProducer = cms.untracked.string("siStripClusters"),
                                      VisCuTkStripClusterLabel = cms.untracked.string(""),
                                      VisCuTkSimhitProducer = cms.untracked.string("g4SimHits"),
                                      VisCuTkSignalToIntegrate = cms.untracked.string("digi"),
                                      VisCuTkPixelRechitProducer = cms.untracked.string("siPixelRecHits"),
                                      VisCuTkPixelDigiProducer = cms.untracked.string("pixdigi"),
                                      VisCuTkPixelDigiLabel = cms.untracked.string(""),
                                      VisCuTkPixelClusterProducer = cms.untracked.string("siPixelClusters"),
                                      VisCuTkPixelClusterLabel = cms.untracked.string(""),
                                      VisCuTkNevToIntegrate = cms.untracked.uint32(100),
                                      VisCuTkCrossedModulesOnly = cms.untracked.bool(True),
                                      VisCuTkAutoIntegrate = cms.untracked.bool(False),
                                      VisCuTkAccessDbConnection = cms.untracked.bool(False),
                                      VisEcalEnergyCut = cms.untracked.double(0.1),
                                      VisEcalRMax = cms.untracked.double(-1),
                                      VisEnergyCut = cms.untracked.double(1),
                                      VisEventTimer = cms.untracked.uint32(30000),
                                      VisExceptionMessage = cms.untracked.string("GUI"),
                                      VisHaloTracks = cms.untracked.bool(False),
                                      VisHcalColorCode = cms.untracked.string("mono"),
                                      VisHcalShowHit = cms.untracked.bool(True),
                                      VisHcalShowTower = cms.untracked.bool(False),
                                      VisHcalRMax = cms.untracked.double(-1),
                                      VisHiddenLineView = cms.untracked.bool(False),
                                      VisGsfTrackDirection = cms.untracked.bool(True),
                                      VisGsfTrackShape = cms.untracked.string("spline"),
                                      VisLegoScale = cms.untracked.double(10),
                                      VisMuonAnnotation = cms.untracked.bool(True),
                                      VisMuonDt = cms.untracked.double(5000),
                                      VisMuonT0 = cms.untracked.double(0),
                                      VisPhiDelta = cms.untracked.double(0.15708),
                                      VisPhiStart = cms.untracked.double(1.5708),
                                      VisPrintTimer = cms.untracked.uint32(30000),
                                      VisPropagatorIn = cms.untracked.double(0),
                                      VisPropagatorOut = cms.untracked.double(0),
                                      VisPropagatorStep = cms.untracked.double(0.05),
                                      VisPtCut = cms.untracked.double(1),
                                      VisRMax = cms.untracked.double(1.2),
                                      VisSelectedTrackNumber = cms.untracked.int32(-1),
                                      VisShowDT4DSegmentChamber = cms.untracked.bool(True),
                                      VisShowHitDets = cms.untracked.bool(True),
                                      VisSimTrackAnnotation = cms.untracked.bool(True),
                                      VisSimHitConnect = cms.untracked.bool(True),
                                      VisSimTrackTime = cms.untracked.double(10),
                                      VisSiStripClusterPointSize = cms.untracked.uint32(4),
                                      VisSiStripClusterRep = cms.untracked.string("point"),
                                      VisSiStripCluster2dRep = cms.untracked.string("strip"),
                                      VisTrackerTrackLineThickness = cms.untracked.uint32(10),
                                      VisTrackShowHits = cms.untracked.bool(False),
                                      VisTrackShowDets = cms.untracked.bool(False),
                                      VisTrackShowRefPoint = cms.untracked.bool(False),
                                      VisTrajectorySeedScale = cms.untracked.double(60.0),
                                      VisVolatileGeometry = cms.untracked.bool(False),
                                      VisZMax = cms.untracked.double(3)
                                      )
