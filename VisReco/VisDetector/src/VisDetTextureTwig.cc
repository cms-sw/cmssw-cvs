//<<<<<< INCLUDES                                                       >>>>>>

#include "VisReco/VisDetector/interface/VisDetTextureTwig.h"
#include "Iguana/GLModels/interface/Ig3DRep.h"
#include "Iguana/GLModels/interface/IgRPhiRep.h"
#include "Iguana/GLModels/interface/IgRZRep.h"
#include "Iguana/Framework/interface/IgRepSet.h"
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

VisDetTextureTwig::VisDetTextureTwig (IgState *state, IgTwig *parent,
				      std::string name, std::string fileName1,
				      std::string fileName2, std::string fileName3)
    : IgSimpleTwig (parent, name, true, false, true),
      m_name (name),
      m_fileName1 (fileName1),
      m_fileName2 (fileName2),
      m_fileName3 (fileName3),
      m_done3D (false),
      m_doneRPhi (false),
      m_doneRZ (false),
      m_texture (state, lat::CreateCallback (this, &VisDetTextureTwig::twigChanged))
{
    std::string tname = m_texture.value ();
    m_fileName1 = tname + ".3d.tga";
    m_fileName2 = tname + ".rphi.tga";
    m_fileName3 = tname + ".rz.tga";    
}

void
VisDetTextureTwig::twigChanged (void)
{ IgRepSet::invalidate (this, SELF_MASK); }


VisDetTextureTwig::~VisDetTextureTwig (void) 
{}

void
VisDetTextureTwig::update (Ig3DRep *rep)
{
    rep->clear ();

    SoSeparator *sep = new SoSeparator;

    SoTexture2* skin = new SoTexture2;
    skin->model.setValue (SoTexture2::DECAL);
    skin->filename.setValue (SbString (m_fileName1.c_str ()));
    sep->addChild (skin);

    // Define the square's spatial coordinates
    SoCoordinate3 *coord = new SoCoordinate3;
    sep->addChild(coord);
    coord->point.set1Value(0, SbVec3f(-8, -8, 0));
    coord->point.set1Value(1, SbVec3f( 8, -8, 0));
    coord->point.set1Value(2, SbVec3f( 8,  8, 0));
    coord->point.set1Value(3, SbVec3f(-8,  8, 0));

    // Define the square's normal
    SoNormal *normal = new SoNormal;
    sep->addChild(normal);
    normal->vector.set1Value(0, SbVec3f(0, 0, 1));
    
    // Define the square's texture coordinates
    SoTextureCoordinate2 *texCoord = new SoTextureCoordinate2;
    sep->addChild(texCoord);
// //     texCoord->point.set1Value(0, SbVec2f(0, 0));
// //     texCoord->point.set1Value(1, SbVec2f(1, 0));
// //     texCoord->point.set1Value(2, SbVec2f(1, 1));
// //     texCoord->point.set1Value(3, SbVec2f(0, 1));
//     texCoord->point.set1Value(0, SbVec2f(1, 0));
//     texCoord->point.set1Value(1, SbVec2f(1, 1));
//     texCoord->point.set1Value(2, SbVec2f(0, 1));
//     texCoord->point.set1Value(3, SbVec2f(0, 0));
    texCoord->point.set1Value(0, SbVec2f(1, 1));
    texCoord->point.set1Value(1, SbVec2f(0, 1));
    texCoord->point.set1Value(2, SbVec2f(0, 0));
    texCoord->point.set1Value(3, SbVec2f(1, 0));
    
    // Define normal and texture coordinate bindings
    SoNormalBinding *nBind = new SoNormalBinding;
    SoTextureCoordinateBinding *tBind =
	new SoTextureCoordinateBinding;
    sep->addChild(nBind);
    sep->addChild(tBind);
    nBind->value.setValue(SoNormalBinding::OVERALL);
    tBind->value.setValue 
	(SoTextureCoordinateBinding::PER_VERTEX);

    // Define a FaceSet
    SoFaceSet *myFaceSet = new SoFaceSet;
    sep->addChild(myFaceSet);
    myFaceSet->numVertices.set1Value(0, 4);
    rep->node ()->addChild (sep);
}

void
VisDetTextureTwig::update (IgRPhiRep *rep)
{
    rep->clear ();

    SoSeparator *sep = new SoSeparator;
    SoTexture2* skin = new SoTexture2;
    skin->model.setValue (SoTexture2::DECAL);
    skin->filename.setValue (SbString (m_fileName2.c_str ()));
    sep->addChild (skin);

    // Define the square's spatial coordinates
    SoCoordinate3 *coord = new SoCoordinate3;
    sep->addChild(coord);
    coord->point.set1Value(0, SbVec3f(-8, -7.5, -10));
    coord->point.set1Value(1, SbVec3f( 8, -7.5, -10));
    coord->point.set1Value(2, SbVec3f( 8,  7.5, -10));
    coord->point.set1Value(3, SbVec3f(-8,  7.5, -10));

    // Define the square's normal
    SoNormal *normal = new SoNormal;
    sep->addChild(normal);
    normal->vector.set1Value(0, SbVec3f(0, 0, 1));
    
    // Define the square's texture coordinates
    SoTextureCoordinate2 *texCoord = new SoTextureCoordinate2;
    sep->addChild(texCoord);
    texCoord->point.set1Value(0, SbVec2f(1, 1));
    texCoord->point.set1Value(1, SbVec2f(0, 1));
    texCoord->point.set1Value(2, SbVec2f(0, 0));
    texCoord->point.set1Value(3, SbVec2f(1, 0));
    
    // Define normal and texture coordinate bindings
    SoNormalBinding *nBind = new SoNormalBinding;
    SoTextureCoordinateBinding *tBind =
	new SoTextureCoordinateBinding;
    sep->addChild(nBind);
    sep->addChild(tBind);
    nBind->value.setValue(SoNormalBinding::OVERALL);
    tBind->value.setValue 
	(SoTextureCoordinateBinding::PER_VERTEX);

    // Define a FaceSet
    SoFaceSet *myFaceSet = new SoFaceSet;
    sep->addChild(myFaceSet);
    myFaceSet->numVertices.set1Value(0, 4);
    rep->node ()->addChild (sep);
}

void
VisDetTextureTwig::update (IgRZRep *rep)
{
    rep->clear ();
    SoSeparator *sep = new SoSeparator;

    SoTexture2* skin = new SoTexture2;
    skin->model.setValue (SoTexture2::DECAL);
    skin->filename.setValue (SbString (m_fileName3.c_str ()));
    sep->addChild (skin);

    // Define the square's spatial coordinates
    SoCoordinate3 *coord = new SoCoordinate3;
    sep->addChild(coord);
    coord->point.set1Value(0, SbVec3f(-10, -8, -12));
    coord->point.set1Value(1, SbVec3f(-10,  8, -12));
    coord->point.set1Value(2, SbVec3f(-10,  8,  12));
    coord->point.set1Value(3, SbVec3f(-10, -8,  12));

    // Define the square's normal
    SoNormal *normal = new SoNormal;
    sep->addChild(normal);
    normal->vector.set1Value(0, SbVec3f(0, 0, 1));
    
    // Define the square's texture coordinates
    SoTextureCoordinate2 *texCoord = new SoTextureCoordinate2;
    sep->addChild(texCoord);
// //     texCoord->point.set1Value(0, SbVec2f(0, 0));
// //     texCoord->point.set1Value(1, SbVec2f(1, 0));
// //     texCoord->point.set1Value(2, SbVec2f(1, 1));
// //     texCoord->point.set1Value(3, SbVec2f(0, 1));
//     texCoord->point.set1Value(0, SbVec2f(1, 0));
//     texCoord->point.set1Value(1, SbVec2f(1, 1));
//     texCoord->point.set1Value(2, SbVec2f(0, 1));
//     texCoord->point.set1Value(3, SbVec2f(0, 0));
    texCoord->point.set1Value(0, SbVec2f(1, 1));
    texCoord->point.set1Value(1, SbVec2f(0, 1));
    texCoord->point.set1Value(2, SbVec2f(0, 0));
    texCoord->point.set1Value(3, SbVec2f(1, 0));
    
    // Define normal and texture coordinate bindings
    SoNormalBinding *nBind = new SoNormalBinding;
    SoTextureCoordinateBinding *tBind =
	new SoTextureCoordinateBinding;
    sep->addChild(nBind);
    sep->addChild(tBind);
    nBind->value.setValue(SoNormalBinding::OVERALL);
    tBind->value.setValue 
	(SoTextureCoordinateBinding::PER_VERTEX);

    // Define a FaceSet
    SoFaceSet *myFaceSet = new SoFaceSet;
    sep->addChild(myFaceSet);
    myFaceSet->numVertices.set1Value(0, 4);
    rep->node ()->addChild (sep);
}
