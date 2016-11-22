import FWCore.ParameterSet.Config as cms

# $Id: preshowerClusterShape_cfi.py,v 1.2 2008/04/21 03:24:18 rpw Exp $
# Preshower cluster producer
preshowerClusterShape = cms.EDProducer("PreshowerClusterShapeProducer",
    preshStripEnergyCut = cms.double(0.0),
    # building preshower clusters
    preshRecHitProducer = cms.InputTag("ecalPreshowerRecHit","EcalRecHitsES"),
    preshPi0Nstrip = cms.int32(5),
    endcapSClusterProducer = cms.InputTag("islandSuperClusters","islandEndcapSuperClusters"),
    #    string corrPhoProducer = "correctedPhotons"
    #    string correctedPhotonCollection   = ""
    PreshowerClusterShapeCollectionX = cms.string('preshowerXClustersShape'),
    PreshowerClusterShapeCollectionY = cms.string('preshowerYClustersShape'),
    # DEBUG: very verbose  INFO: minimal printout
    debugLevel = cms.string('INFO')
)


