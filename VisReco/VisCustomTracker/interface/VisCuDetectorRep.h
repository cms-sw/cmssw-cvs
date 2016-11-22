#ifndef VIS_CUSTOM_TRACKER_VIS_CU_DETECTOR_REP_H
# define VIS_CUSTOM_TRACKER_VIS_CU_DETECTOR_REP_H

//<<<<<< INCLUDES                                                       >>>>>>
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SbVec3f;
class SoSeparator;
class GeomDetUnit;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class VisCuDetectorRep
{
public:  
  enum DetectorRep { FullDet, RZDet, RPhiDet };
  
  static void	addWafer (SoSeparator *separator, const GeomDetUnit *det, DetectorRep type);
  
private:
  static void	createRotation (const GeomDetUnit *det, SbVec3f & axis, float & angle);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_CUSTOM_TRACKER_VIS_CU_DETECTOR_REP_H
