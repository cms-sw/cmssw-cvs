#ifndef VIS_HCAL_VIS_DRAW_TOWER_HELPER_H
# define VIS_HCAL_VIS_DRAW_TOWER_HELPER_H

# include <Inventor/nodes/SoGroup.h>
# include <Inventor/nodes/SoIndexedFaceSet.h>
# include <Inventor/nodes/SoIndexedLineSet.h>
# include <vector>

class VisDrawTowerHelper
{
public:
  VisDrawTowerHelper(SoGroup *);
  ~VisDrawTowerHelper(void) {}
  
  /**
      Add a single tower whose height does not scale(i.e. tower floor is front face, roof is back face)
  */
  void addTower(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4);

  /**
      Add a single tower outline whose height does not scale(i.e. tower floor is front face, roof is back face)
  */
  void addTowerOutline(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
		       SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4);

  /**
      Add a single tower whose floor is the front face and whose height scales according to
      some scalar variable(heightContent) and an overall scale(heightScale)
  */
  void addTower(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4,
                float heightContent,
                float heightScale);

  /**
      Add a single tower whose floor is offset(heightOffset) from the front face
      and whose height scales according to
      some scalar variable(heightContent) and an overall scale(heightScale)
  */
  void addTower(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4,
                float heightContent, float heightOffset,
                float heightScale);
  /**
     Add a single lego tower with two energy fractions 
   */
  void addLegoTower(SbVec2f position, float energy, float emFraction, float scaleFactor,
		    float etaWidth, float phiWidth);
  /**
      Add a single box centred on the original box but scaled linearly by scaleFraction
  */
  void addScaledBox(SbVec3f &f1,  SbVec3f &f2,  SbVec3f &f3,  SbVec3f &f4,
                    SbVec3f &b1,  SbVec3f &b2,  SbVec3f &b3,  SbVec3f &b4,
                    float scaleFraction);

private:

  int m_c;  /// counter for corners
  int m_l;  /// counter for line indices
  int m_f;  /// counter for face indices
  SoVertexProperty     *m_vertices;
  SoIndexedLineSet     *m_lineSet;
  SoIndexedFaceSet     *m_faceSet;
  SoGroup              *m_group;

  std::vector<int>      m_faceIndices;
  std::vector<int>      m_lineIndices;
  std::vector<SbVec3f>  m_corners;

  void setLineIndices(int p1, int p2, int p3, int p4, int p5);
  void setLineIndices(int p1, int p2);
  void setFaceIndices(int p1, int p2, int p3, int p4);
  void drawTower(SbVec3f &sf1, SbVec3f &sf2, SbVec3f &sf3, SbVec3f &sf4,
                 SbVec3f &sb1, SbVec3f &sb2, SbVec3f &sb3, SbVec3f &sb4);
  void drawTowerOutline(SbVec3f &sf1, SbVec3f &sf2, SbVec3f &sf3, SbVec3f &sf4,
			SbVec3f &sb1, SbVec3f &sb2, SbVec3f &sb3, SbVec3f &sb4);
};

#endif // VIS_HCAL_VIS_DRAW_TOWER_HELPER_H
