// -*- C++ -*-
//
// Package:     Calo
// Class  :     FWDTSegments3DProxyBuilder
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:
//         Created:  Sun Jan  6 23:57:00 EST 2008
// $Id: FWDTSegments3DProxyBuilder.cc,v 1.1 2009/05/14 20:29:26 yanjuntu Exp $
//

// system include files
#include "TEveManager.h"
#include "TGeoTube.h"
#include "TEveGeoNode.h"
#include "TEveElement.h"
#include "TEveCompound.h"
#include "TEvePointSet.h"

// user include files
#include "Fireworks/Core/interface/FW3DDataProxyBuilder.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/BuilderUtils.h"
#include "Fireworks/Core/interface/FW3DView.h"
#include "Fireworks/Core/interface/FWEveScalableStraightLineSet.h"
#include "Fireworks/Core/interface/FWEveValueScaler.h"
#include "Fireworks/Core/interface/DetIdToMatrix.h"

#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"

class FWDTSegments3DProxyBuilder : public FW3DDataProxyBuilder
{

public:
   FWDTSegments3DProxyBuilder();
   virtual ~FWDTSegments3DProxyBuilder();

   // ---------- const member functions ---------------------
   REGISTER_PROXYBUILDER_METHODS();

   // ---------- static member functions --------------------
private:
   virtual void build(const FWEventItem* iItem,
                      TEveElementList** product);

   FWDTSegments3DProxyBuilder(const FWDTSegments3DProxyBuilder&);    // stop default

   const FWDTSegments3DProxyBuilder& operator=(const FWDTSegments3DProxyBuilder&);    // stop default

   // ---------- member data --------------------------------
};

//
// constructors and destructor
//
FWDTSegments3DProxyBuilder::FWDTSegments3DProxyBuilder()
{
}

FWDTSegments3DProxyBuilder::~FWDTSegments3DProxyBuilder()
{
}

void
FWDTSegments3DProxyBuilder::build(const FWEventItem* iItem, TEveElementList** product)
{
   TEveElementList* tList = *product;

   if(0 == tList) {
      tList =  new TEveElementList(iItem->name().c_str(),"dtSegments",true);
      *product = tList;
      tList->SetMainColor(iItem->defaultDisplayProperties().color());
      gEve->AddElement(tList);
   } else {
      tList->DestroyElements();
   }

   const DTRecSegment4DCollection* segments = 0;
   iItem->get(segments);

   if(0 == segments ) {
      // std::cout <<"failed to get DT segments"<<std::endl;
      return;
   }
  TEveCompound* compund = new TEveCompound("dt compound", "dtSegments" );
   compund->OpenCompound();
   unsigned int index = 0;
   for (  DTRecSegment4DCollection::id_iterator chamberId = segments->id_begin();
          chamberId != segments->id_end(); ++chamberId, ++index )
   {

      std::stringstream s;
      s << "chamber" << index;
      TEveStraightLineSet* segmentSet = new TEveStraightLineSet(s.str().c_str());
      TEvePointSet* pointSet = new TEvePointSet();
      pointSet->SetMarkerStyle(2);
      pointSet->SetMarkerSize(3);
      segmentSet->SetLineWidth(3);
      segmentSet->SetMainColor(iItem->defaultDisplayProperties().color());
      segmentSet->SetRnrSelf(iItem->defaultDisplayProperties().isVisible());
      segmentSet->SetRnrChildren(iItem->defaultDisplayProperties().isVisible());
      pointSet->SetMainColor(iItem->defaultDisplayProperties().color());
      compund->AddElement( segmentSet );
      segmentSet->AddElement( pointSet );
      const TGeoHMatrix* matrix = iItem->getGeom()->getMatrix( (*chamberId).rawId() );

      DTRecSegment4DCollection::range range = segments->get(*chamberId);
      const double segmentLength = 15;
      for (DTRecSegment4DCollection::const_iterator segment = range.first;
           segment!=range.second; ++segment)
      {
         Double_t localSegmentInnerPoint[3];
         Double_t localSegmentCenterPoint[3];
         Double_t localSegmentOuterPoint[3];
         Double_t globalSegmentInnerPoint[3];
         Double_t globalSegmentCenterPoint[3];
         Double_t globalSegmentOuterPoint[3];

        //  if ( (rhoPhiProjection && !segment->hasPhi() ) ||
//               (!rhoPhiProjection && !segment->hasZed() ) ) {
//             localSegmentInnerPoint[0] = 0;
//             localSegmentInnerPoint[1] = 0;
//             localSegmentInnerPoint[2] = 0;
//             matrix->LocalToMaster( localSegmentInnerPoint, globalSegmentInnerPoint );
//             pointSet->SetNextPoint( globalSegmentInnerPoint[0], globalSegmentInnerPoint[1], globalSegmentInnerPoint[2] );
//             continue;
//          }

         localSegmentOuterPoint[0] = segment->localPosition().x() + segmentLength*segment->localDirection().x();
         localSegmentOuterPoint[1] = segment->localPosition().y() + segmentLength*segment->localDirection().y();
         localSegmentOuterPoint[2] = segmentLength*segment->localDirection().z();

         localSegmentCenterPoint[0] = segment->localPosition().x();
         localSegmentCenterPoint[1] = segment->localPosition().y();
         localSegmentCenterPoint[2] = 0;

         localSegmentInnerPoint[0] = segment->localPosition().x() - segmentLength*segment->localDirection().x();
         localSegmentInnerPoint[1] = segment->localPosition().y() - segmentLength*segment->localDirection().y();
         localSegmentInnerPoint[2] = -segmentLength*segment->localDirection().z();

         matrix->LocalToMaster( localSegmentInnerPoint, globalSegmentInnerPoint );
         matrix->LocalToMaster( localSegmentCenterPoint, globalSegmentCenterPoint );
         matrix->LocalToMaster( localSegmentOuterPoint, globalSegmentOuterPoint );

         if ( globalSegmentInnerPoint[1] *globalSegmentOuterPoint[1] > 0 ) {
            segmentSet->AddLine(globalSegmentInnerPoint[0], globalSegmentInnerPoint[1], globalSegmentInnerPoint[2],
                                globalSegmentOuterPoint[0], globalSegmentOuterPoint[1], globalSegmentOuterPoint[2] );
         } else {
            if ( fabs(globalSegmentInnerPoint[1]) > fabs(globalSegmentOuterPoint[1]) )
               segmentSet->AddLine(globalSegmentInnerPoint[0], globalSegmentInnerPoint[1], globalSegmentInnerPoint[2],
                                   globalSegmentCenterPoint[0], globalSegmentCenterPoint[1], globalSegmentCenterPoint[2] );
            else
               segmentSet->AddLine(globalSegmentCenterPoint[0], globalSegmentCenterPoint[1], globalSegmentCenterPoint[2],
                                   globalSegmentOuterPoint[0], globalSegmentOuterPoint[1], globalSegmentOuterPoint[2] );
         }
      }
   }
   tList->AddElement(compund);

}

REGISTER_FW3DDATAPROXYBUILDER(FWDTSegments3DProxyBuilder,DTRecSegment4DCollection,"DT-segments");


