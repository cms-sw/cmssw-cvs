#ifndef VIS_MUON_CSC_VIS_CSCDIGI_TOOLS_H
# define VIS_MUON_CSC_VIS_CSCDIGI_TOOLS_H

//<<<<<< INCLUDES                                                       >>>>>>

#include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>

// This is to become a class better...
// Some common Digi visualiszation parameters
//
//static struct CommonVar{
//  float OurUsedPhi;
//  float OurUsedZ;
//} CommonVar;
float const MEsFrames_X = -3;
float const MEsFrames_Xstep = 2.;
float const MEsFrames_Y = -1.;
float const MEsFrames_Ystep = 1.;
 
float const MEsHorisontal_Y = 7.5;
float const MEsHorisontal_X = -3.55;
float const MEsHorisontal_stepX = 2.0;


float const MEsVertical_X = -6.;
float const MEsVertical_Y = -1.15;
float const MEsVertical_stepY = 1.;

float const Digis_Y = -5.5;
float const Digis_X = -3.5;
float const Digis_stepX = 4.5;


float const PHI_Y = 0.;
float const PHI_X = -5.5;

char const VisFont[10] = "Helvetica";
float const VisFontSize = 0.5;

//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoSeparator;
class SoLineSet;
class SoTranslation;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

// function is to be called once only
bool VisCaptions(float const layer_z, 
		 float const layer_phi, 
		 float &OurPhi, 
		 float &OurZ, 
		 SoSeparator *sep);
//		
bool VisFrames(const std::vector<float> trapezBounds, 
	       SoLineSet *trapez,
	       SoLineSet *rect_w, 
	       SoLineSet *rect_s);
//
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
	       SoTranslation *trans_w);

//<<<<<< CLASS DECLARATIONS                                             >>>>>>
//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_MUON_CSC_VIS_CSCDIGI_TOOLS_H
