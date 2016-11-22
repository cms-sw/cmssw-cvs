#include "VisReco/VisMuon/interface/VisRefitTrackTwig.h"
#include "VisFramework/VisEventSetup/interface/VisEventSetupService.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <sstream>
#include <iomanip>
#include <classlib/utils/Error.h>

VisRefitTrackTwig::VisRefitTrackTwig (IgState *state, IgTwig *parent,
				      const std::string &name /* = "" */,
				      reco::TrackRef track)
    : VisQueuedTwig (state, parent, name),
      m_track (track),
      m_in (state, lat::CreateCallback (this, &VisRefitTrackTwig::configChanged)),
      m_out (state, lat::CreateCallback (this, &VisRefitTrackTwig::configChanged)),
      m_step (state, lat::CreateCallback (this, &VisRefitTrackTwig::configChanged)),
      m_track3DSep(0),
      m_trackRPhiSep(0),
      m_trackRZSep(0)
{}

void
VisRefitTrackTwig::onNewEvent (const edm::Event &event,
			       const edm::EventSetup &eventSetup)
{
  if(m_track3DSep != 0)
  {
    m_track3DSep->unref();
    m_track3DSep =0;
  }
  if(m_trackRPhiSep != 0)
  {
    m_trackRPhiSep->unref();
    m_trackRPhiSep =0;
  }
  if(m_trackRZSep != 0)
  {
    m_trackRZSep->unref();
    m_trackRZSep =0;
  }
}

void
VisRefitTrackTwig::configChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }

void
VisRefitTrackTwig::update (Ig3DRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);

  rep->clear ();
  if(m_track3DSep == 0)
  {
    m_track3DSep = new SoSeparator;
    m_track3DSep->ref();
    if (m_track.isNonnull () && m_track.isAvailable ()) 
    {
      SoSeparator *sep = new SoSeparator;
      m_track3DSep->addChild (sep);
      SoMaterial *mat = new SoMaterial;
      mat->ambientColor.setValue (0.0039215689, 0.35294119, 1);
      mat->diffuseColor.setValue (0.0039215689, 0.35294119, 1);
      mat->specularColor.setValue (0.0039215689, 0.35294119, 1);
      mat->emissiveColor.setValue (0.0039215689, 0.35294119, 1);
      sep->addChild (mat);
      SoDrawStyle *style = new SoDrawStyle;
      style->style = SoDrawStyle::LINES;
      style->lineWidth.setValue (3.0);
      sep->addChild (style);

      VisEventSetupService *esService = VisEventSetupService::get (state ());
      if (esService)
      {
	SoSeparator *trackSep = esService->refitTrack (m_track, m_in.value (), m_out.value (), m_step.value ());
	if (trackSep != NULL)
	  sep->addChild (trackSep);
      }	
    }
  }
  rep->node ()->addChild (m_track3DSep);
}

void
VisRefitTrackTwig::update (IgRPhiRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);

  rep->clear ();
  if(m_trackRPhiSep == 0)
  {
    m_trackRPhiSep = new SoSeparator;
    m_trackRPhiSep->ref();

    if (m_track.isNonnull () && m_track.isAvailable ()) 
    {
      SoSeparator *sep = new SoSeparator;
      m_trackRPhiSep->addChild(sep);
      SoMaterial *mat = new SoMaterial;
      mat->ambientColor.setValue (0.0039215689, 0.35294119, 1);
      mat->diffuseColor.setValue (0.0039215689, 0.35294119, 1);
      mat->specularColor.setValue (0.0039215689, 0.35294119, 1);
      mat->emissiveColor.setValue (0.0039215689, 0.35294119, 1);
      sep->addChild (mat);
      SoDrawStyle *style = new SoDrawStyle;
      style->style = SoDrawStyle::LINES;
      style->lineWidth.setValue (3.0);
      sep->addChild (style);

      VisEventSetupService *esService = VisEventSetupService::get (state ());
      if (esService)
      {
	SoSeparator *trackSep = esService->refitTrack (m_track, m_in.value (), m_out.value (), m_step.value ());
	if (trackSep != NULL)
	  sep->addChild (trackSep);
      }	
    }
  }
  rep->node ()->addChild (m_trackRPhiSep);
}

void
VisRefitTrackTwig::update (IgRZRep *rep)
{
  // Get debugging dump.
  VisQueuedTwig::update (rep);

  rep->clear ();
  if(m_trackRZSep == 0)
  {
    m_trackRZSep = new SoSeparator;
    m_trackRZSep->ref();

    if (m_track.isNonnull () && m_track.isAvailable ()) 
    {
      SoSeparator *sep = new SoSeparator;
      m_trackRZSep->addChild(sep);
      SoMaterial *mat = new SoMaterial;
      mat->ambientColor.setValue (0.0039215689, 0.35294119, 1);
      mat->diffuseColor.setValue (0.0039215689, 0.35294119, 1);
      mat->specularColor.setValue (0.0039215689, 0.35294119, 1);
      mat->emissiveColor.setValue (0.0039215689, 0.35294119, 1);
      sep->addChild (mat);
      SoDrawStyle *style = new SoDrawStyle;
      style->style = SoDrawStyle::LINES;
      style->lineWidth.setValue (3.0);
      sep->addChild (style);

      VisEventSetupService *esService = VisEventSetupService::get (state ());
      if (esService)
      {
	SoSeparator *trackSep = esService->refitTrack (m_track, m_in.value (), m_out.value (), m_step.value ());
	if (trackSep != NULL)
	  sep->addChild (trackSep);
      }	
    }
  }
  rep->node ()->addChild (m_trackRZSep);
}
