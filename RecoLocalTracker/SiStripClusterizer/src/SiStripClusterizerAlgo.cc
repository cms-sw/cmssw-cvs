#include "RecoLocalTracker/SiStripClusterizer/interface/SiStripClusterizerAlgo.h"
#include "CalibTracker/Records/interface/SiStripGainRcd.h"
#include "CalibTracker/Records/interface/SiStripQualityRcd.h"
#include "CondFormats/DataRecord/interface/SiStripNoisesRcd.h"
#include <algorithm>

// -----------------------------------------------------------------------------
//
SiStripClusterizerAlgo::SiStripClusterizerAlgo( const edm::ParameterSet& pset) :
  noise_(0),
  quality_(0),
  gain_(0),
  nCacheId_(0),
  qCacheId_(0),
  gCacheId_(0),
  noiseRange_( SiStripNoises::Range( SiStripNoises::ContainerIterator(0),
				     SiStripNoises::ContainerIterator(0) ) ),
  qualityRange_( SiStripQuality::Range( SiStripQuality::ContainerIterator(0),
					SiStripQuality::ContainerIterator(0) ) ),
  gainRange_( SiStripApvGain::Range( SiStripApvGain::ContainerIterator(0),
				     SiStripApvGain::ContainerIterator(0) ) ),
  detId_(0)
{;}

// -----------------------------------------------------------------------------
//
SiStripClusterizerAlgo::~SiStripClusterizerAlgo() {;}

// -----------------------------------------------------------------------------
//
void SiStripClusterizerAlgo::clusterize( const DigisDSVnew& digis, 
					 ClustersDSVnew& clusters ) {
  DigisDSVnew::const_iterator idigis = digis.begin();
  DigisDSVnew::const_iterator jdigis = digis.end();
  for ( ; idigis != jdigis; ++idigis ) {
    ClustersV out;
    clusterize( *idigis, out );
    if ( !out.empty() ) { clusters.insert( idigis->id(), &*out.begin(), out.size() ); }
  }
}

// -----------------------------------------------------------------------------
//
void SiStripClusterizerAlgo::clusterize( const DigisDSV& digis, 
					 ClustersDSVnew& clusters ) {
  DigisDSV::const_iterator idigis = digis.begin();
  DigisDSV::const_iterator jdigis = digis.end();
  for ( ; idigis != jdigis; ++idigis ) {
    DigisDSnew in( idigis->id, &*idigis->data.begin(), idigis->data.size() );
    ClustersV out;
    clusterize( in, out );
    if ( !out.empty() ) { clusters.insert( idigis->id, &*out.begin(), out.size() ); }
  }
}

// -----------------------------------------------------------------------------
//
void SiStripClusterizerAlgo::clusterize( const DigisDSV& digis, 
					 ClustersDSV& clusters ) {
  DigisDSV::const_iterator idigis = digis.begin();
  DigisDSV::const_iterator jdigis = digis.end();
  for ( ; idigis != jdigis; ++idigis ) {
    DigisDSnew in( idigis->id, &*idigis->data.begin(), idigis->data.size() );
    ClustersDS out( idigis->id );
    clusterize( in, out.data ); 
    if ( !out.empty() ) { clusters.insert( out ); }
  }
}

// -----------------------------------------------------------------------------
//
void SiStripClusterizerAlgo::eventSetup( const edm::EventSetup& setup ) {
  
  uint32_t n_cache_id = setup.get<SiStripNoisesRcd>().cacheIdentifier();
  uint32_t q_cache_id = setup.get<SiStripQualityRcd>().cacheIdentifier();
  uint32_t g_cache_id = setup.get<SiStripGainRcd>().cacheIdentifier();
  
  if ( nCacheId_ != n_cache_id ) {
    edm::ESHandle<SiStripNoises> n;
    setup.get<SiStripNoisesRcd>().get(n);
    noise_ = n.product();
    nCacheId_ = n_cache_id;
  }
  
  if ( qCacheId_ != q_cache_id ) {
    edm::ESHandle<SiStripQuality> q;
    setup.get<SiStripQualityRcd>().get(q);
    quality_ = q.product();
    qCacheId_ = q_cache_id;
  }
  
  if ( gCacheId_ != g_cache_id ) {
    edm::ESHandle<SiStripGain> g;
    setup.get<SiStripGainRcd>().get(g);
    gain_ = g.product();
    gCacheId_ = g_cache_id;
  }
  
}

// -----------------------------------------------------------------------------
// 
void SiStripClusterizerAlgo::conditions( uint32_t det_id ) { 
  
  if ( checkDetId( det_id ) ) { return; }
  detId_ = det_id;
  
  // all horrible! need to add "invalid range" to condition objects
  
  if ( noise_ ) { noiseRange_ = noise_->getRange(detId_); }
  else { noiseRange_ = SiStripNoises::Range( SiStripNoises::ContainerIterator(0),
					     SiStripNoises::ContainerIterator(0) );
  }

  if ( quality_ ) { qualityRange_ = quality_->getRange(detId_); }
  else { qualityRange_ = SiStripQuality::Range( SiStripQuality::ContainerIterator(0),
						SiStripQuality::ContainerIterator(0) );
  }
  
  if ( gain_ ) { gainRange_ = gain_->getRange(detId_); }
  else { gainRange_ = SiStripApvGain::Range( SiStripApvGain::ContainerIterator(0),
					     SiStripApvGain::ContainerIterator(0) );
  }
  
}

