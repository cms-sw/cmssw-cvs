//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisMuonCSC/interface/VisCSCDigiTools.h"
// This is to become a class better...
// Some common Digi visualiszation parameters
//
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
//
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoFont.h>
#include <qstring.h>
#include <iostream>
#include <iomanip>
#include <vector>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

// function is to be called once only
bool VisCaptions(float const layer_z, 
		 float const layer_phi, 
		 float &OurPhi, 
		 float &OurZ, 
		 SoSeparator *sep){
  OurPhi = layer_phi; // init
  OurZ = layer_z; // init
  //
  SoTranslation *trans_me1 = new SoTranslation;
  SoTranslation *trans_me2 = new SoTranslation;
  SoTranslation *trans_me3 = new SoTranslation;
  SoTranslation *trans_me4 = new SoTranslation;
  //
  trans_me1->translation.setValue(MEsHorisontal_X,MEsHorisontal_Y,0.);
  trans_me2->translation.setValue(MEsHorisontal_X+MEsHorisontal_stepX,MEsHorisontal_Y,0.);
  trans_me3->translation.setValue(MEsHorisontal_X+2.*MEsHorisontal_stepX,MEsHorisontal_Y,0.);
  trans_me4->translation.setValue(MEsHorisontal_X+3.*MEsHorisontal_stepX,MEsHorisontal_Y,0.);
  //
  SoTranslation *trans_me1_v = new SoTranslation;
  SoTranslation *trans_me2_v = new SoTranslation;
  SoTranslation *trans_me3_v = new SoTranslation;
  SoTranslation *trans_me4_v = new SoTranslation;
  //
  trans_me1_v->translation.setValue(MEsVertical_X,
				    MEsVertical_Y,0.);
  trans_me2_v->translation.setValue(MEsVertical_X,
				    MEsVertical_Y-MEsVertical_stepY,0.);
  trans_me3_v->translation.setValue(MEsVertical_X,
				    MEsVertical_Y-2.*MEsVertical_stepY,0.);
  trans_me4_v->translation.setValue(MEsVertical_X,
				    MEsVertical_Y-3.*MEsVertical_stepY,0.);
  //
  SoTranslation *trans_strips = new SoTranslation;
  SoTranslation *trans_wires = new SoTranslation;
  SoTranslation *trans_phi = new SoTranslation;
  SoTranslation *trans_format = new SoTranslation;
  //
  trans_strips->translation.setValue(Digis_X,Digis_Y,0.);
  trans_wires->translation.setValue(Digis_X+Digis_stepX,Digis_Y,0.);
  trans_phi->translation.setValue(PHI_X,PHI_Y,0.);
  trans_format->translation.setValue(PHI_X-0.5,PHI_Y-0.3,0.);
  //  
  SoText3 *TextME1 = new SoText3;
  SoText3 *TextME2 = new SoText3;
  SoText3 *TextME3 = new SoText3;
  SoText3 *TextME4 = new SoText3;
  //
  TextME1->string = "ME1";
  TextME2->string = "ME2";
  TextME3->string = "ME3";
  TextME4->string = "ME4";
  //
  SoText3 *TextStrips = new SoText3;
  SoText3 *TextWires = new SoText3;
  SoText3 *TextPhi = new SoText3;
  SoText3 *TextFormat = new SoText3;
  //
  TextStrips->string = "Strips";
  TextWires->string = "Wire groups";
  TextPhi->string = "Phi:";
  //TextFormat->string = "----------------------------------------------------------------------------";
  //
  SoMaterial *mat = new SoMaterial;
  mat->diffuseColor.setValue (0.0, 0.0, 0.0);
  //
  SoMaterial *matInfo = new SoMaterial;
  matInfo->diffuseColor.setValue (0.0, 0.0, 1.0);
  //
  SoMaterial *matText = new SoMaterial;
  matText->diffuseColor.setValue (1.0, 0.0, 0.0);
  //root->addChild(africaSep);
  //
  SoSeparator *Station1_h = new SoSeparator;
  SoSeparator *Station2_h = new SoSeparator;
  SoSeparator *Station3_h = new SoSeparator;
  SoSeparator *Station4_h = new SoSeparator;
  //
  SoSeparator *Station1_v = new SoSeparator;
  SoSeparator *Station2_v = new SoSeparator;
  SoSeparator *Station3_v = new SoSeparator;
  SoSeparator *Station4_v = new SoSeparator;
  //
  SoSeparator *strips_txt =  new SoSeparator;
  SoSeparator *wires_txt =  new SoSeparator;
  SoSeparator *phi_txt =  new SoSeparator;
  SoSeparator *format_txt =  new SoSeparator;
  //
  
  Station1_h->addChild(trans_me1);
  Station1_h->addChild(TextME1);
  //
  Station2_h->addChild(trans_me2);
  Station2_h->addChild(TextME2);
  //
  Station3_h->addChild(trans_me3);
  Station3_h->addChild(TextME3);
  //
  Station4_h->addChild(trans_me4);
  Station4_h->addChild(TextME4);
  //
  Station1_v->addChild(trans_me1_v);
  Station1_v->addChild(TextME1);
  //
  Station2_v->addChild(trans_me2_v);
  Station2_v->addChild(TextME2);
  //
  Station3_v->addChild(trans_me3_v);
  Station3_v->addChild(TextME3);
  //
  Station4_v->addChild(trans_me4_v);
  Station4_v->addChild(TextME4);
  //
  strips_txt->addChild(trans_strips);
  strips_txt->addChild(matInfo);
  strips_txt->addChild(TextStrips);
  //
  wires_txt->addChild(trans_wires);
  wires_txt->addChild(matInfo);
  wires_txt->addChild(TextWires);
  //
  phi_txt->addChild(trans_phi);
  phi_txt->addChild(mat);
  phi_txt->addChild(TextPhi);
  //
  format_txt->addChild(trans_format);
  format_txt->addChild(mat);
  format_txt->addChild(TextFormat);
  //
  SoFont *font = new SoFont;
  //font->name.setValue("Courier-BoldOblique");
  font->name.setValue(VisFont);
  font->size.setValue(VisFontSize);
  //
  SoSeparator *TitleStations = new SoSeparator;
  SoSeparator *info = new SoSeparator;
  //
  TitleStations->addChild(font);
  TitleStations->addChild(matText);
  TitleStations->addChild(Station1_h);
  TitleStations->addChild(Station2_h);
  TitleStations->addChild(Station3_h);
  TitleStations->addChild(Station4_h);
  TitleStations->addChild(Station1_v);
  TitleStations->addChild(Station2_v);
  TitleStations->addChild(Station3_v);
  TitleStations->addChild(Station4_v);
  //
  info->addChild(font);
  info->addChild(strips_txt);
  info->addChild(wires_txt);
  info->addChild(phi_txt);
  info->addChild(format_txt);
  //
  sep->addChild (TitleStations);
  sep->addChild (info);
  //
  return true;
}
//		
bool VisFrames(const std::vector<float> trapezBounds, 
	       SoLineSet *trapez,
	       SoLineSet *rect_w, 
	       SoLineSet *rect_s){
  if(4!=trapezBounds.size()) return false;
  // Layer (chamber) bounds
  int nVrx = 0;
  float x, y, z;
  //	   
  std::vector<LocalPoint> CrossPoint;
  CrossPoint.push_back( LocalPoint(-trapezBounds[0], -trapezBounds[3], 0.));
  CrossPoint.push_back( LocalPoint(trapezBounds[0], -trapezBounds[3], 0.));
  CrossPoint.push_back( LocalPoint(trapezBounds[1], trapezBounds[3], 0.));
  CrossPoint.push_back( LocalPoint(-trapezBounds[1], trapezBounds[3], 0.));
  CrossPoint.push_back( LocalPoint(-trapezBounds[0], -trapezBounds[3], 0.));
  //
  SoVertexProperty *vert = new SoVertexProperty;
  for (std::vector<LocalPoint>::iterator _crossP = 
	 CrossPoint.begin (); _crossP != CrossPoint.end (); ++_crossP){
    x = _crossP->x()/100.0;  // cm -> m
    y = _crossP->y()/100.0;  // cm -> m
    z = _crossP->z()/100.0;  // cm -> m
    vert->vertex.set1Value (nVrx++,SbVec3f (x, y, z) );
  }
  vert->vertex.setNum (nVrx);
  trapez->vertexProperty.setValue (vert);
  trapez->numVertices[CrossPoint.size()];
  // "Strip" bounds
  CrossPoint.clear();
  CrossPoint.push_back( LocalPoint(-trapezBounds[1], -trapezBounds[2], 0.));
  CrossPoint.push_back( LocalPoint(trapezBounds[1], -trapezBounds[2], 0.));
  CrossPoint.push_back( LocalPoint(trapezBounds[1], trapezBounds[2], 0.));
  CrossPoint.push_back( LocalPoint(-trapezBounds[1], trapezBounds[2], 0.));
  CrossPoint.push_back( LocalPoint(-trapezBounds[1], -trapezBounds[2], 0.));
  //
  vert = new SoVertexProperty;
  nVrx = 0;   
  for (std::vector<LocalPoint>::iterator _crossP = 
	 CrossPoint.begin (); _crossP != CrossPoint.end (); ++_crossP){
    x = _crossP->x()/100.0;  // cm -> m
    y = _crossP->y()/4.;  // cm -> m
    z = _crossP->z()/100.0;  // cm -> m
    vert->vertex.set1Value (nVrx++,SbVec3f (x, y, z) );
  }
  vert->vertex.setNum (nVrx);
  rect_s->vertexProperty.setValue (vert);
  rect_s->numVertices[CrossPoint.size()];
  // "Wire" bounds 
  CrossPoint.clear();
  CrossPoint.push_back( LocalPoint(-trapezBounds[3], -trapezBounds[2], 0.));
  CrossPoint.push_back( LocalPoint(trapezBounds[3], -trapezBounds[2], 0.));
  CrossPoint.push_back( LocalPoint(trapezBounds[3], trapezBounds[2], 0.));
  CrossPoint.push_back( LocalPoint(-trapezBounds[3], trapezBounds[2], 0.));
  CrossPoint.push_back( LocalPoint(-trapezBounds[3], -trapezBounds[2], 0.));
  //
  vert = new SoVertexProperty;
  nVrx = 0;  
  for (std::vector<LocalPoint>::iterator _crossP = 
	 CrossPoint.begin (); _crossP != CrossPoint.end (); ++_crossP){
    x = _crossP->x()/100.0;  // cm -> m
    y = _crossP->y()/4.;  // cm -> m
    z = _crossP->z()/100.0;  // cm -> m
    vert->vertex.set1Value (nVrx++,SbVec3f (x, y, z) );
  }
  vert->vertex.setNum (nVrx);
  rect_w->vertexProperty.setValue (vert);
  rect_w->numVertices[CrossPoint.size()];
  //
  return true;	
}

bool MakeTrans(const std::vector<float> trapezBounds,
	       float const station_id, 
	       float const layer_r,
	       float const layer_phi,
	       float const layer_z,
	       float const OurPhi,
	       float const OurZ,
               SoTranslation *trans,
	       SoTranslation *trans_caption,
	       SoTranslation *trans_s,
	       SoTranslation *trans_w){
  // Translations
  float Xtrans = MEsFrames_X + 
    MEsFrames_Xstep*(station_id-1); // put diff. Stations at diff. place
  float Xtrans_m = MEsFrames_X + 
    MEsFrames_Xstep*(station_id-1) - MEsFrames_Xstep/10.;
  float Xtrans_p = MEsFrames_X + 
    MEsFrames_Xstep*(station_id-1) + MEsFrames_Xstep/10.;
  //	  
  trans->translation = SbVec3f(Xtrans,layer_r,0.);
  trans_caption->translation.setValue(Xtrans-0.4,0.,0.);
  //
  float Ytrans = MEsFrames_Y - MEsFrames_Ystep*(station_id-1);
  float Ytrans_m = MEsFrames_Y - 
    MEsFrames_Ystep*(station_id-1) - MEsFrames_Ystep/3.;
  float Ytrans_p = MEsFrames_Y - 
    MEsFrames_Ystep*(station_id-1) + MEsFrames_Ystep/3.;
  trans_s->translation = SbVec3f(MEsFrames_X,Ytrans,0.);
  //
  float Xtrans_w = layer_r-MEsFrames_Xstep;
  trans_w->translation = SbVec3f(Xtrans_w,Ytrans,0.);
  // Colors, shifts
  if(OurPhi != layer_phi){
    float Ytrans_strip;
    if(OurZ < layer_z){ // shift chambers
      Ytrans_strip=Ytrans_m;
      trans_w->translation = SbVec3f(Xtrans_w,Ytrans_m,0.);
    }
    else{
      Ytrans_strip=Ytrans_p;
      trans_w->translation = SbVec3f(Xtrans_w,Ytrans_p,0.);
    }
    if (layer_phi<OurPhi){ // shift chambers a bit
      trans->translation = SbVec3f(Xtrans_m,layer_r,0.);
      trans_caption->translation.setValue(Xtrans_m-0.8,0.,0.);
      trans_s->translation = SbVec3f(MEsFrames_X-2.*0.8*trapezBounds[1]/100.,
				     Ytrans_strip,0.);
    }
    else{ 
      trans->translation = SbVec3f(Xtrans_p,layer_r,0.);
      trans_caption->translation.setValue(Xtrans_p,0.,0.);
      trans_s->translation = SbVec3f(MEsFrames_X+2.*0.8*trapezBounds[1]/100.,
				     Ytrans_strip,0.);
    }
  }
  return true;
}

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

