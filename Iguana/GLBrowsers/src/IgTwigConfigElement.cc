//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/IgTwigConfigElement.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SoOutput.h>
#include <qregexp.h>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
#define FIX_DEFAULT_TYPE(TYPE)				\
    std::string prefix (type);				\
    if (type.size() == 0){prefix=TYPE;}
    
#define FIND_TWIG_CONFIG_OBJECT(MAP, NAME)		\
    FIX_DEFAULT_TYPE(MAP##Type)				\
    NAME##Map::const_iterator itr = MAP.find(prefix);	\
    if (itr !=  MAP.end()){				\
        QString search (name.c_str()); 			\
        QRegExp rx;					\
        rx.setWildcard(true);				\
        for(NAME::const_iterator i=itr->second.begin(); \
	    i!=itr->second.end (); i++){		\
            rx.setPattern(i->first.c_str());		\
	    if (rx.exactMatch(search)){                 \
	    return i->second;}                          \
        }						\
    }							\
    std::cerr <<"WARNING: Could not find object for:"<<prefix.c_str()<<":"<<name.c_str()<<std::endl;

#define FIND_TWIG_CONFIG_ALIAS(MAP, NAME)		\
    FIX_DEFAULT_TYPE(MAP##Type)				\
    NAME##Map::const_iterator itr = MAP.find(prefix);	\
    if (itr !=  MAP.end()){				\
        for(NAME::const_iterator i=itr->second.begin(); \
	    i!=itr->second.end (); i++){		\
	    if (to==i->first){obj=i->second; break;}	\
        }						\
    }

#define ADD_SO_TWIG_CONFIG_ALIAS(MAP,NAME)		\
    So##NAME* obj = 0;					\
    FIND_TWIG_CONFIG_ALIAS(MAP,NAME);			\
    if (obj){						\
        obj->ref();					\
        MAP[prefix].push_back(NAME##Info(alias,obj));	\
    }else{std::cout <<"ALIAS FAIL:"<<alias.c_str()<<"=>"<<to.c_str()<<std::endl;}
    
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgTwigConfigElement, "Services/Global/TwigConfig");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgTwigConfigElement::IgTwigConfigElement (IgState *state)
    : m_state (state),
      m_materialType ("Default"),
      m_drawStyleType("Default"),
      m_visibilityType ("Default")
{
    ASSERT (m_state);
    m_state->put (s_key, this);
    init ();
}

IgTwigConfigElement::~IgTwigConfigElement ()
{
    m_state->detach (s_key);
    for(MaterialMap::const_iterator itr=m_material.begin();
        itr!=m_material.end (); itr++)
    {
	for(Material::const_iterator i=itr->second.begin();
	    i!=itr->second.end (); i++)
	    i->second->unref();
    }
    
    for(DrawStyleMap::const_iterator itr=m_drawStyle.begin();
        itr!=m_drawStyle.end (); itr++)
    {
        for(DrawStyle::const_iterator i=itr->second.begin();
            i!=itr->second.end (); i++)
	    i->second->unref();
    }
}

void IgTwigConfigElement::setMaterialType (const std::string& type)
{ m_materialType = type; }

void IgTwigConfigElement::setDrawStyleType (const std::string& type)
{ m_drawStyleType = type; }

void IgTwigConfigElement::setVisibilityType (const std::string& type)
{ m_visibilityType = type; }

const std::string& IgTwigConfigElement::getMaterialType () const
{ return m_materialType; }

const std::string& IgTwigConfigElement::getDrawStyleType () const
{ return m_drawStyleType; }

const std::string& IgTwigConfigElement::getVisibilityType () const
{ return m_visibilityType; }

SoMaterial*
IgTwigConfigElement::material (const std::string& name, const std::string& type) const
{
    SoMaterial	*obj = findMaterial (name,type);
    if (obj)
        obj = dynamic_cast<SoMaterial*>(obj->copy());
    return obj;
}

SoDrawStyle*
IgTwigConfigElement::drawStyle (const std::string& name, const std::string& type) const
{
    SoDrawStyle	*obj = findDrawStyle (name,type);
    if (obj)
        obj = dynamic_cast<SoDrawStyle*>(obj->copy());
    return obj;
}

bool
IgTwigConfigElement::visibility (const std::string& name, const std::string& type) const
{ return findVisibility (name,type); }

void
IgTwigConfigElement::addMaterial(const std::string& name, SoMaterial* material, const std::string& type)
{
    FIX_DEFAULT_TYPE(m_materialType);
    SoMaterial* m = dynamic_cast<SoMaterial*>(material->copy ());
    m->ref();
    m_material[prefix].push_back(MaterialInfo(name,m));
    material->setToDefaults();
}

void
IgTwigConfigElement::addDrawStyle(const std::string& name, SoDrawStyle* style, const std::string& type)
{
    FIX_DEFAULT_TYPE(m_drawStyleType);
    SoDrawStyle* s = dynamic_cast<SoDrawStyle*>(style->copy ());
    s->ref();
    m_drawStyle[prefix].push_back(DrawStyleInfo(name,s));
    style->setToDefaults();
}

void
IgTwigConfigElement::addVisibility(const std::string& name, bool visibility, const std::string& type)
{
    FIX_DEFAULT_TYPE(m_visibilityType);
    m_visibility[prefix].push_back(VisibilityInfo(name,visibility));
}

void
IgTwigConfigElement::init ()
{
    initMaterial ();
    initDrawStyle ();
    initVisibility ();
}

void
IgTwigConfigElement::initMaterial ()
{
    float rgbcomponents [4];float tmprgbcomponents [4];
    SoMaterial	*material = new SoMaterial;
    material->ref();
    std::string type("Default");
    
    //DefaultMaterial
    addMaterial("Default",material,type);
    //Keep track of default colors
    
    SoMFColor ambientDefault, diffuseDefault, emissiveDefault, specularDefault;
    SoMFFloat transparencyDefault, shininessDefault;
    ambientDefault = material->ambientColor;
    diffuseDefault = material->diffuseColor;
    emissiveDefault = material->emissiveColor;
    specularDefault = material->specularColor;
    transparencyDefault = material->transparency;
    shininessDefault = material->shininess;
    
    //Start: Defaults from VisReco/CisDetector/ivs/*.iv files
    material->shininess.setValue(0.);
    material->transparency.setValue(0.);
    material->ambientColor.setValue (0.095, 0.095, 0.095);
    material->diffuseColor.setValue (0.372, 0.371, 0.373);
    material->specularColor.setValue(0.556, 0.554, 0.556);
    material->emissiveColor.setValue(0.500, 0.500, 0.500);
    addMaterial("*",material,"Gray");
    
    material->ambientColor.setValue(0.93905598, 0.00302124, 0.027419999);
    material->diffuseColor.setValue(0.704292, 0.0022659299, 0.020564999);
    material->specularColor.setValue(0.49999201, 0.49999201, 0.49999201);
    material->shininess.setValue(0.1);
    addMaterial("VisDetIvTwig/absorber-barrel-3d.gzip.iv",material,type);

    material->ambientColor.setValue(1, 0.33333334, 0);
    material->diffuseColor.setValue(1, 0.33333334, 0);
    material->specularColor.setValue(1, 0.33333334, 0);
    material->emissiveColor.setValue(1, 0.33333334, 0);
    material->shininess.setValue(0);
    addMaterial("VisDetIvTwig/absorber-barrel-rphi.gzip.iv",material,type);

    material->ambientColor.setValue(1, 0, 0);
    material->diffuseColor.setValue(1, 0, 0);
    material->specularColor.setValue(0.79000002, 0.83999997, 0.83999997);
    material->shininess.setValue(1);
    addMaterial("VisDetIvTwig/absorber-endcap-3d.gzip.iv",material,type);

    material->ambientColor.setValue(1, 0, 0);
    material->diffuseColor.setValue(1, 0, 0);
    material->specularColor.setValue(1, 0, 0);
    material->emissiveColor.setValue(1, 0, 0);
    material->shininess.setValue(0);
    addMaterial("VisDetIvTwig/absorber-endcap-rz.gzip.iv",material,type);

    material->ambientColor.setValue(0.094999999, 0.094999999, 0.094999999);
    material->diffuseColor.setValue(0.37200001, 0.37099999, 0.373);
    material->specularColor.setValue(0.55599999, 0.55400002, 0.55599999);
    material->emissiveColor.setValue(0.5, 0.5, 0.5);
    material->shininess.setValue(0.12800001);
    addMaterial("VisDetIvTwig/beampipe.gzip.iv",material,type);
    addMaterialAlias("VisDetIvTwig/ecal-preshower-3d.gzip.iv","VisDetIvTwig/beampipe.gzip.iv",type);

    material->ambientColor.setValue(0.38, 1, 1);
    material->diffuseColor.setValue(0, 0.44, 0.75);
    material->shininess.setValue(0.1);
    material->transparency.setValue(0.49000001);
    addMaterial("VisDetIvTwig/csc-3d.gzip.iv",material,type);

    material->ambientColor.setValue(0.63921571, 0.66666669, 0.68627453);
    material->diffuseColor.setValue(0.63921571, 0.66666669, 0.68627453);
    material->specularColor.setValue(0.63921571, 0.66666669, 0.68627453);
    material->emissiveColor.setValue(0.63921571, 0.66666669, 0.68627453);
    material->shininess.setValue(0);
    addMaterial("VisDetIvTwig/driftTubes-rphi.gzip.iv",material,type);
    addMaterialAlias("VisDetIvTwig/driftTubes-rz.gzip.iv","VisDetIvTwig/driftTubes-rphi.gzip.iv",type);
    addMaterialAlias("VisDetIvTwig/csc-rz.gzip.iv","VisDetIvTwig/driftTubes-rphi.gzip.iv",type);

    material->ambientColor.setValue(0, 0, 0);
    material->diffuseColor.setValue(0, 0, 0);
    material->specularColor.setValue(0.63921571, 0.66666669, 0.68627453);
    material->emissiveColor.setValue(0.63921571, 0.66666669, 0.68627453);
    material->shininess.setValue(0.121);
    addMaterial("VisDetIvTwig/driftTubes-3d.gzip.iv",material,type);


    material->ambientColor.setValue(0.42352942, 0.43921569, 0.45490196);
    material->diffuseColor.setValue(0.42352942, 0.43921569, 0.45490196);
    material->specularColor.setValue(0.42352942, 0.43921569, 0.45490196);
    material->emissiveColor.setValue(0.42352942, 0.43921569, 0.45490196);
    material->shininess.setValue(0);
    addMaterial("VisDetIvTwig/driftTubes-superlayer-details-rphi.gzip.iv",material,type);
    addMaterialAlias("VisDetIvTwig/driftTubes-superlayers-rphi.gzip.iv","VisDetIvTwig/driftTubes-superlayer-details-rphi.gzip.iv",type);
    addMaterialAlias("VisDetIvTwig/driftTubes-superlayers-rz.gzip.iv","VisDetIvTwig/driftTubes-superlayer-details-rphi.gzip.iv",type);
    addMaterialAlias("VisDetIvTwig/hcal-barrel-layers-rphi.gzip.iv","VisDetIvTwig/driftTubes-superlayer-details-rphi.gzip.iv",type);
    addMaterialAlias("VisDetIvTwig/hcal-barrel-layers-rz.gzip.iv","VisDetIvTwig/driftTubes-superlayer-details-rphi.gzip.iv",type);
    addMaterialAlias("VisDetIvTwig/hcal-barrel-rphi.gzip.iv","VisDetIvTwig/driftTubes-superlayer-details-rphi.gzip.iv",type);
    addMaterialAlias("VisDetIvTwig/hcal-barrel-rz.gzip.iv","VisDetIvTwig/driftTubes-superlayer-details-rphi.gzip.iv",type);
    addMaterialAlias("VisDetIvTwig/hcal-barrel-towers-rphi.gzip.iv","VisDetIvTwig/driftTubes-superlayer-details-rphi.gzip.iv",type);
    addMaterialAlias("VisDetIvTwig/hcal-forward-rz.gzip.iv","VisDetIvTwig/driftTubes-superlayer-details-rphi.gzip.iv",type);
    addMaterialAlias("VisDetIvTwig/tracker-barrel-rphi.gzip.iv","VisDetIvTwig/driftTubes-superlayer-details-rphi.gzip.iv",type);
    addMaterialAlias("VisDetIvTwig/tracker-rz.gzip.iv","VisDetIvTwig/driftTubes-superlayer-details-rphi.gzip.iv",type);

    material->ambientColor.setValue(0.704, 0.72100002, 0.72100002);
    material->diffuseColor.setValue(0.48199999, 0.48100001, 0.55199999);
    material->specularColor.setValue(0.83399999, 0.56599998, 0.54000002);
    material->emissiveColor.setValue(0.5, 0.5, 1);
    material->shininess.setValue(0.236);
    addMaterial("VisDetIvTwig/ecal-barrel-3d.gzip.iv",material,type);
    addMaterialAlias("VisDetIvTwig/ecal-forward-3d.gzip.iv","VisDetIvTwig/ecal-barrel-3d.gzip.iv",type);
    addMaterialAlias("VisDetIvTwig/hcal-outer-3d.gzip.iv","VisDetIvTwig/ecal-barrel-3d.gzip.iv",type);
    

    material->ambientColor.setValue(0, 0.66666669, 1);
    material->diffuseColor.setValue(0, 0.66666669, 1);
    material->specularColor.setValue(0, 0.66666669, 1);
    material->emissiveColor.setValue(0, 0.66666669, 1);
    material->shininess.setValue(0);
    addMaterial("VisDetIvTwig/ecal-barrel-rphi.gzip.iv",material,type);
    addMaterialAlias("VisDetIvTwig/ecal-rz.gzip.iv","VisDetIvTwig/ecal-barrel-rphi.gzip.iv",type);


    material->ambientColor.setValue(0.226, 0.13699999, 0.056000002);
    material->diffuseColor.setValue(0.90399998, 0.54900002, 0.22400001);
    material->specularColor.setValue(0.094999999, 0.094999999, 0.094999999);
    material->emissiveColor.setValue(0.40000001, 0.40000001, 0.2);
    material->shininess.setValue(0.078000002);
    addMaterial("VisDetIvTwig/hcal-barrel-3d.gzip.iv",material,type);
    addMaterialAlias("VisDetIvTwig/hcal-endcap-3d.gzip.iv","VisDetIvTwig/hcal-barrel-3d.gzip.iv",type);

    material->ambientColor.setValue(0.0070000002, 0.0020000001, 0);
    material->diffuseColor.setValue(0.414, 0.074000001, 0);
    material->specularColor.setValue(1, 0.76700002, 0);
    material->emissiveColor.setValue(0.5, 0.5, 0.5);
    material->shininess.setValue(0.048);
    addMaterial("VisDetIvTwig/hcal-forward-3d.gzip.iv",material,type);

    material->ambientColor.setValue(0, 0.018646499, 0);
    material->diffuseColor.setValue(0.58999997, 0.56, 0.56);
    material->specularColor.setValue(0.66000003, 0.66000003, 0.66000003);
    material->shininess.setValue(0.43000001);
    addMaterial("VisDetIvTwig/magnet-3d.gzip.iv",material,type);

    material->ambientColor.setValue(0.66000003, 0.66000003, 0.66000003);
    material->diffuseColor.setValue(0.58999997, 0.56, 0.56);
    material->specularColor.setValue(0.66000003, 0.66000003, 0.66000003);
    material->emissiveColor.setValue(0.66000003, 0.66000003, 0.66000003);
    material->shininess.setValue(0);
    material->transparency.setValue(0);
    addMaterial("VisDetIvTwig/magnet-rphi.gzip.iv",material,type);
    addMaterialAlias("VisDetIvTwig/magnet-rz.gzip.iv","VisDetIvTwig/magnet-rphi.gzip.iv",type);

    material->ambientColor.setValue(0.0039215689, 0.35294119, 1);
    material->diffuseColor.setValue(0.0039215689, 0.35294119, 1);
    material->specularColor.setValue(0.0039215689, 0.35294119, 1);
    material->emissiveColor.setValue(0.0039215689, 0.35294119, 1);
    material->shininess.setValue(-0.00073242188);
    addMaterial("VisDetIvTwig/rpc-barrel-3d.gzip.iv",material,type);
    addMaterialAlias("VisDetIvTwig/rpc-endcap-3d.gzip.iv","VisDetIvTwig/rpc-barrel-3d.gzip.iv",type);
    addMaterialAlias("VisDetIvTwig/rpc-endcap-rz.gzip.iv","VisDetIvTwig/rpc-barrel-3d.gzip.iv",type);

    material->ambientColor.setValue(0, 0.40000001, 1);
    material->diffuseColor.setValue(0, 0.40000001, 1);
    material->specularColor.setValue(0, 0.40000001, 1);
    material->emissiveColor.setValue(0, 0.40000001, 1);
    addMaterial("VisDetIvTwig/rpc-barrel-rphi.gzip.iv",material,type);
    addMaterialAlias("VisDetIvTwig/rpc-barrel-rz.gzip.iv","VisDetIvTwig/rpc-barrel-rphi.gzip.iv",type);

    //Start: VisEventIdTwig
    IgSbColorMap::unpack (0x8b898900, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisEventIdTwig/*",material,type);
    
    //VisEventSetupService/Det 
    material->ambientColor.setValue (0.88445997, 0.840042, 0.53582799);
    material->diffuseColor.setValue (0.66334498, 0.630032, 0.401871);
    material->specularColor.setValue (0.49999201, 0.49999201, 0.49999201);
    material->shininess.setValue (0.1);
    addMaterial("VisEventSetupService/Det/FullDet",material,type);

    material->ambientColor.setValue (0.66334498, 0.66334498, 0.66334498);
    material->diffuseColor.setValue (0.66334498, 0.66334498, 0.66334498);
    material->specularColor.setValue (0.66334498, 0.66334498, 0.66334498);
    material->emissiveColor.setValue (0.66334498, 0.66334498, 0.66334498);
    addMaterial("VisEventSetupService/Det/Default",material,type);
    
    //VisEventSetupService/HitDets
    material->ambientColor.setValue (1, 0.752941, 0.796078);
    material->diffuseColor.setValue (1, 0.752941, 0.796078);
    material->specularColor.setValue (1, 0.752941, 0.796078);
    material->emissiveColor.setValue (1, 0.752941, 0.796078);
    material->shininess.setValue (0);
    addMaterial("VisEventSetupService/HitDets/FullDet",material,type);
    
    material->ambientColor.setValue (0.5490196, 0.0090196, 0.0090196);
    material->diffuseColor.setValue (0.5490196, 0.0090196, 0.0090196);
    material->specularColor.setValue (0.5490196, 0.0090196, 0.0090196);
    material->emissiveColor.setValue (0.5490196, 0.0090196, 0.0090196);
    material->shininess.setValue (0);
    addMaterial("VisEventSetupService/HitDets/Default",material,type);

    //VisEventSetupService/*
    IgSbColorMap::unpack (0x59595900, rgbcomponents);
    material->ambientColor.setValue (SbColor (rgbcomponents));
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    material->specularColor.setValue (SbColor (rgbcomponents));
    material->emissiveColor.setValue (SbColor (rgbcomponents));
    material->shininess.setValue (0);
    addMaterial("VisEventSetupService/*",material,type);
    
    /////////////////////VisReco//////////////////////////
    //VisPixelDigiTwig
    material->diffuseColor.setValue (0.0, 0.0, 1.0);
    material->emissiveColor.setValue(0.0, 0.0, 1.0);
    addMaterial("VisPixelDigiTwig/3D",material,type);
    
    material->diffuseColor.setValue (0.0, 1.0, 1.0);
    material->emissiveColor.setValue(0.0, 1.0, 1.0);
    addMaterial("VisPixelDigiTwig/3D/Cube",material,type);
    addMaterialAlias("VisPixelDigiTwig/RPhi","VisPixelDigiTwig/3D",type);
    
    material->diffuseColor.setValue (0.3, 0.8, 0.0);
    addMaterial("VisPixelDigiTwig/RZ",material,type);
    
    //VisDetTwig
    material->diffuseColor.setValue (0.0, 0.0, 0.0);
    addMaterial("VisDetTwig/*",material,type);
    
    //VisTkIdealHelixTracksTwig
    material->diffuseColor.setValue (1.0, 0.0, 0.0);
    addMaterial("VisTkIdealHelixTracksTwig/*",material,type);
    
    //VisTkRecTracksTwig
    IgSbColorMap::unpack (0x99ccff, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisTkRecTracksTwig/*",material,type);
    
    //VisTrackerClusterTwig
    IgSbColorMap::unpack (0xccff0000, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisTrackerClusterTwig/RPhi/2",material,type);

    IgSbColorMap::unpack (0x6600ff00, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisTrackerClusterTwig/*",material,type);
    
    //VisTrackerRechit2DTwig
    material->diffuseColor.setValue (1.0, 1.0, 1.0);
    addMaterial("VisTrackerRechit2DTwig/*/2",material,type);
    
    IgSbColorMap::unpack (0xde316300, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisTrackerRechit2DTwig/*",material,type);
    
    //VisTrackerDigiTwig
    IgSbColorMap::unpack (0x03c03c00, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisTrackerDigiTwig/*",material,type);
    
    //VisTrackingRecHitTwig
    IgSbColorMap::unpack (0xee2c2c, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisTrackingRecHitTwig/*",material,type);
    
    //VisTrackerPiClusterTwig
    material->diffuseColor.setValue (0.0, 0.0, 1.0);
    material->emissiveColor.setValue(0.0, 0.0, 1.0);
    addMaterial("VisTrackerPiClusterTwig/*",material,type);
     
    //VisTrackerPiDigiTwig
    material->diffuseColor.setValue (0.3, 0.8, 0.0);
    addMaterial("VisTrackerPiDigiTwig/RZ",material,type);

    material->diffuseColor.setValue (0.0, 1.0, 0.0);
    material->emissiveColor.setValue(0.0, 1.0, 0.0);
    addMaterial("VisTrackerPiDigiTwig/*",material,type);
   
    //VisTrackerPiRechitTwig
    material->diffuseColor.setValue (1.0, 0.0, 0.0);
    material->emissiveColor.setValue(1.0, 0.0, 0.0);
    addMaterial("VisTrackerPiRechitTwig/*",material,type);
    
    //VisTrajectorySeedTwig
    material->diffuseColor.setValue (0.6, 1.0, 0.0);
    addMaterial("VisTrajectorySeedTwig/*",material,type);
    
    //VisTrackerRechit2DMatchedTwig
    addMaterialAlias("VisTrackerRechit2DMatchedTwig/*","VisTrackerPiRechitTwig/*",type);
    
    //General purpose ColorMap for: VisCaloJetTwig, VisBasicClusterCollectionTwig, VisGenJetCollectionTwig
    //VisJetTagTwig, VisSuperClusterCollectionTwig
    unsigned colour [10] = { 0xff000000, 0xffff0000, 0x00ff0000, // red yellow green
		             0xff00ff00, 0x00ffff00, 0xd0dfff00, // magenta cyan
			     0xff000fff, 0x00ffff00, 0xe00fff00,
			     0xff00ee00};
    for(int i=0;i<10;i++)
    {
        IgSbColorMap::unpack (colour[i], rgbcomponents);
	material->diffuseColor.setValue (rgbcomponents);
	addMaterial(std::string("ColorMap")+QString::number(i).latin1(),material,type);
    }
    
    //VisBasicClusterCollectionTwig
    material->ambientColor.setValue (0.000, 0.000, 0.000); 
    material->diffuseColor.setValue (1.000, 0.000, 0.000);  
    material->emissiveColor.setValue (1.000, 0.000, 0.000); 
    material->shininess.setValue(0.0);
    addMaterial("VisBasicClusterCollectionTwig/3D",material,type);
    
    material->diffuseColor.setValue (0.2, 1.0, 0.4);
    addMaterial("VisBasicClusterCollectionTwig/RPhi/-ve",material,type);

    material->diffuseColor.setValue (1.0, 0.0, 0.6);
    addMaterial("VisBasicClusterCollectionTwig/*",material,type);

    //VisGenJetTwig
    IgSbColorMap::unpack (0x66330000, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisGenJetTwig/*",material,type);

    //VisGsfTrackTwig
    IgSbColorMap::unpack (0x03C03C00, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisGsfTrackTwig/*/Direction",material,type);

    IgSbColorMap::unpack (0xCD00CC00, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisGsfTrackTwig/*",material,type);
    
    //VisSuperClusterCollectionTwig
    addMaterialAlias("VisSuperClusterCollectionTwig/3D",      "VisBasicClusterCollectionTwig/3D",type);
    addMaterialAlias("VisSuperClusterCollectionTwig/RPhi/-ve","VisBasicClusterCollectionTwig/RPhi/-ve",type);
    addMaterialAlias("VisSuperClusterCollectionTwig/*",       "VisBasicClusterCollectionTwig/*",type);
    
    //VisBasicClusterTwig
    IgSbColorMap::unpack (0xff009c00, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisBasicClusterTwig/3D",material,type);
    addMaterialAlias("VisBasicClusterTwig/RPhi/-ve", "VisBasicClusterCollectionTwig/RPhi/-ve",type);
    addMaterialAlias("VisBasicClusterTwig/*",        "VisBasicClusterCollectionTwig/*",type);
    
    //VisSuperClusterTwig
    addMaterialAlias("VisSuperClusterTwig/3D",       "VisBasicClusterTwig/3D",type);
    addMaterialAlias("VisSuperClusterTwig/RPhi/-ve", "VisBasicClusterTwig/RPhi/-ve",type);
    addMaterialAlias("VisSuperClusterTwig/*",        "VisBasicClusterTwig/*",type);
    
    //VisEcalRecHitTwig
    addMaterialAlias("VisEcalRecHitTwig/3D",       "VisBasicClusterTwig/3D",type);
    addMaterialAlias("VisEcalRecHitTwig/RPhi/-ve", "VisBasicClusterTwig/RPhi/-ve",type);
    addMaterialAlias("VisEcalRecHitTwig/*",        "VisBasicClusterTwig/*",type);
    
    //VisEcalUncalibratedRecHitTwig
    addMaterialAlias("VisEcalUncalibratedRecHitTwig/RPhi/-ve", "VisEcalRecHitTwig/RPhi/-ve",type);
    addMaterialAlias("VisEcalUncalibratedRecHitTwig/*",        "VisEcalRecHitTwig/*",type);
    
    //VisHBHERecHitTwig
    IgSbColorMap::unpack (0x99ff0000, rgbcomponents); // snow4
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisHBHERecHitTwig/3D",material,type);
    
    material->diffuseColor.setValue (0.0, 0.4, 1.0);
    addMaterial("VisHBHERecHitTwig/RPhi/+ve",material,type);
    
    material->diffuseColor.setValue (1.0, 0.0, 0.0);
    addMaterial("VisHBHERecHitTwig/RPhi/-ve",material,type);

    addMaterialAlias("VisHBHERecHitTwig/RZ","VisHBHERecHitTwig/3D",type);
    addMaterialAlias("VisHBHERecHitTwig/RZ/Max","VisHBHERecHitTwig/RPhi/+ve",type);

    //VisHFRecHitTwig
    addMaterialAlias("VisHFRecHitTwig/RZ/Max", "VisHBHERecHitTwig/RZ/Max",type);
    addMaterialAlias("VisHFRecHitTwig/*",      "VisHBHERecHitTwig/3D",type);
    
    //VisHORecHitTwig
    addMaterialAlias("VisHORecHitTwig/RZ/Max", "VisHBHERecHitTwig/RZ/Max",type);
    material->diffuseColor.setValue (0.0, 0.1, 1.0);
    addMaterial("VisHORecHitTwig/RPhi/+ve",material,type);
    
    material->diffuseColor.setValue (1.0, 0.0, 0.0);
    addMaterial("VisHORecHitTwig/RPhi/-ve",material,type);
    addMaterialAlias("VisHORecHitTwig/*","VisHBHERecHitTwig/3D",type);
    
    //VisCSCSeg2HETwig
    material->ambientColor.setValue (0.135, 0.2225, 0.1575);
    addMaterial("VisCSCSeg2HETwig/*",material,type);
    
    //VisMETTwig
    material->diffuseColor.setValue (1, 0, 0);
    addMaterial("VisMETTwig/*",material,type);
    
    //VisGenMETTwig
    addMaterialAlias("VisGenMETTwig/*","VisMETTwig/*",type);
    
    //VisCaloMETTwig
    addMaterialAlias("VisCaloMETTwig/*","VisMETTwig/*",type);

    //VisCSCALCTDigiTwig
    material->diffuseColor.setValue (0.0, 1.0, 0.0);
    material->emissiveColor.setValue (0.0, 1.0, 0.0);
    material->shininess.setValue (1.);
    addMaterial("VisCSCALCTDigiTwig/3D/Strip",material,type);
    
    material->diffuseColor.setValue (0.3, 0.8, 0.0);
    addMaterial("VisCSCALCTDigiTwig/Lego/+ve",material,type);
    addMaterialAlias("VisCSCALCTDigiTwig/RPhi","VisCSCALCTDigiTwig/Lego/+ve",type);
    addMaterialAlias("VisCSCALCTDigiTwig/RZ",  "VisCSCALCTDigiTwig/Lego/+ve",type);
    
    material->diffuseColor.setValue (1.0, 1.0, 1.0);
    material->emissiveColor.setValue (1.0, 1.0, 1.0);
    addMaterial("VisCSCALCTDigiTwig/*",material,type);
    
    //VisCSCCLCTDigiTwig
    addMaterialAlias("VisCSCCLCTDigiTwig/3D",       "VisCSCALCTDigiTwig/*",type);
    addMaterialAlias("VisCSCCLCTDigiTwig/3D/Strip","VisCSCALCTDigiTwig/3D/Strip",type);
    addMaterialAlias("VisCSCCLCTDigiTwig/3D/Layer","VisCSCCLCTDigiTwig/3D",type);
    addMaterialAlias("VisCSCCLCTDigiTwig/Lego/+ve", "VisCSCALCTDigiTwig/Lego/+ve",type);
    addMaterialAlias("VisCSCCLCTDigiTwig/Lego/-ve", "VisCSCCLCTDigiTwig/3D",type);
    addMaterialAlias("VisCSCCLCTDigiTwig/RPhi",     "VisCSCCLCTDigiTwig/Lego/+ve",type);
    addMaterialAlias("VisCSCCLCTDigiTwig/RZ",       "VisCSCCLCTDigiTwig/RPhi",type);
    
    //VisCSCCorrelatedLCTDigiTwig
    addMaterialAlias("VisCSCCorrelatedLCTDigiTwig/3D",       "VisCSCALCTDigiTwig/*",type);
    addMaterialAlias("VisCSCCorrelatedLCTDigiTwig/3D/Strip","VisCSCALCTDigiTwig/3D/Strip",type);
    addMaterialAlias("VisCSCCorrelatedLCTDigiTwig/3D/Layer","VisCSCCorrelatedLCTDigiTwig/3D",type);
    addMaterialAlias("VisCSCCorrelatedLCTDigiTwig/RZ",       "VisCSCALCTDigiTwig/RZ",type);
    
    //VisCSCComparatorDigiTwig
    addMaterialAlias("VisCSCComparatorDigiTwig/3D","VisCSCALCTDigiTwig/*",type);
    
    material->diffuseColor.setValue (0.1, 1.0, 0.4);
    material->emissiveColor.setValue (0.1, 1.0, 0.4);
    material->shininess.setValue (1.);
    addMaterial("VisCSCComparatorDigiTwig/3D/Strip",material,type);
    addMaterialAlias("VisCSCComparatorDigiTwig/3D/Layer","VisCSCComparatorDigiTwig/3D",type);
    addMaterialAlias("VisCSCComparatorDigiTwig/Lego/+ve", "VisCSCALCTDigiTwig/Lego/+ve",type);
    addMaterialAlias("VisCSCComparatorDigiTwig/Lego/-ve", "VisCSCComparatorDigiTwig/3D",type);
    addMaterialAlias("VisCSCComparatorDigiTwig/*",        "VisCSCComparatorDigiTwig/Lego/+ve",type);
    
    //VisCSCRPCDigiTwig
    addMaterialAlias("VisCSCRPCDigiTwig/3D",       "VisCSCALCTDigiTwig/*",type);
    addMaterialAlias("VisCSCRPCDigiTwig/3D/Strip","VisCSCALCTDigiTwig/3D/Strip",type);
    addMaterialAlias("VisCSCRPCDigiTwig/3D/Layer","VisCSCRPCDigiTwig/3D",type);
    addMaterialAlias("VisCSCRPCDigiTwig/*",        "VisCSCALCTDigiTwig/RPhi",type);
    
    //VisCSCStripDigiTwig
    addMaterialAlias("VisCSCStripDigiTwig/3D","VisCSCALCTDigiTwig/*",type);
    
    material->diffuseColor.setValue (0.7, 1.0, 0.0);
    material->emissiveColor.setValue (0.7, 1.0, 0.0);
    material->shininess.setValue (1.);
    addMaterial("VisCSCStripDigiTwig/3D/Strip",material,type);
//    addMaterialAlias("VisCSCStripDigiTwig/3D/Layer","VisCSCStripDigiTwig/3D",type);
    addMaterialAlias("VisCSCStripDigiTwig/Lego/+ve", "VisCSCALCTDigiTwig/Lego/+ve",type);
    addMaterialAlias("VisCSCStripDigiTwig/Lego/-ve", "VisCSCStripDigiTwig/3D",type);
    addMaterialAlias("VisCSCStripDigiTwig/RPhi",     "VisCSCStripDigiTwig/3D",type);
    material->diffuseColor.setValue (0.0, 1.0, 1.0);
    material->emissiveColor.setValue (0.0, 1.0, 1.0);
    addMaterial("VisCSCStripDigiTwig/RPhi/Strip",material,type);
    addMaterialAlias("VisCSCStripDigiTwig/RZ",       "VisCSCALCTDigiTwig/RPhi",type);
    
    material->diffuseColor.setValue (0.0, 0.0, 0.0);
    addMaterial("VisCSCStripDigiTwig/Spare",material,type);
    
    material->diffuseColor.setValue (0.0, 1.0, 0.0);
    addMaterial("VisCSCStripDigiTwig/Spare/Matched/Strip",material,type);
    material->diffuseColor.setValue (0.0, 0.0, 0.0);
    addMaterial("VisCSCStripDigiTwig/Spare/Matched/Frame",material,type);
    material->diffuseColor.setValue (0.7, 1.0, 0.2);
    addMaterial("VisCSCStripDigiTwig/Spare/UnMatched/Strip",material,type);
    material->diffuseColor.setValue (0.6, 0.6, 0.6);
    addMaterial("VisCSCStripDigiTwig/Spare/UnMatched/Frame",material,type);
        
    
    //VisRPCDigiTwig
    material->diffuseColor.setValue (1.0, 1.0, 2.0);
    material->emissiveColor.setValue (1.0, 1.0, 2.0);
    addMaterial("VisRPCDigiTwig/3D",material,type);
    
    material->diffuseColor.setValue (1, 0.0, 0.0);
    material->emissiveColor.setValue (1, 0.0, 0.0);
    material->shininess.setValue (1.);
    addMaterial("VisRPCDigiTwig/3D/Strip",material,type);

    material->diffuseColor.setValue (2.0, 1.0, 1.0);
    material->emissiveColor.setValue (2.0, 1.0, 1.0);
    addMaterial("VisRPCDigiTwig/3D/Layer/Trapezoidal",material,type);

    material->diffuseColor.setValue (1.0, 0.0, 0.0);
    material->emissiveColor.setValue (1.0, 0.0, 0.0);
    addMaterial("VisRPCDigiTwig/3D/Layer/Rectangular",material,type);
    addMaterialAlias("VisRPCDigiTwig/RZ","VisCSCALCTDigiTwig/RZ",type);
    
    
    //VisCSCWireDigiTwig
    material->diffuseColor.setValue (1.0, 1.0, 1.0);
    material->emissiveColor.setValue (1.0, 1.0, 1.0);
    addMaterial("VisCSCWireDigiTwig/3D",material,type);
    
    material->diffuseColor.setValue (0.3, 0.8, 0.0);
    addMaterial("VisCSCWireDigiTwig/RZ",material,type);
    addMaterial("VisCSCWireDigiTwig/RPhi",material,type);
    

    material->diffuseColor.setValue (0.35, 0.35, 0.35);
    material->emissiveColor.setValue (0.35, 0.35, 0.35);
    addMaterial("VisCSCWireDigiTwig/3D/Layer",material,type);
    addMaterialAlias("VisCSCStripDigiTwig/3D/Layer","VisCSCWireDigiTwig/3D/Layer",type);
    material->diffuseColor.setValue (0.F, 0.F, 0.F);
    material->emissiveColor.setValue (0.F, 0.F, 0.F);
    addMaterial("VisCSCWireDigiTwig/*/Layer",material,type);
    addMaterialAlias("VisCSCStripDigiTwig/*/Layer","VisCSCWireDigiTwig/*/Layer",type);

    material->diffuseColor.setValue (0.5, 0.0, 1.0);
    material->emissiveColor.setValue (0.5, 0.0, 1.0);
    addMaterial("VisCSCWireDigiTwig/*/Wire",material,type);

    material->diffuseColor.setValue (0.0, 0.0, 0.0);
    addMaterial("VisCSCWireDigiTwig/Spare",material,type);
    
    material->diffuseColor.setValue (0.3, 0.0, 0.3);
    addMaterial("VisCSCWireDigiTwig/Spare/Matched/Wire",material,type);
    material->diffuseColor.setValue (0.0, 0.0, 0.0);
    addMaterial("VisCSCWireDigiTwig/Spare/Matched/Frame",material,type);
    material->diffuseColor.setValue (0.6, 0.0, 6.0);
    addMaterial("VisCSCWireDigiTwig/Spare/UnMatched/Wire",material,type);
    material->diffuseColor.setValue (0.6, 0.6, 0.6);
    addMaterial("VisCSCWireDigiTwig/Spare/UnMatched/Frame",material,type);
    addMaterialAlias("VisCSCWireDigiTwig/*","VisCSCWireDigiTwig/3D",type);
    
    //VisDT2DSegmentTwig
    material->diffuseColor.setValue (0., 1., 0.);
    addMaterial("VisDT2DSegmentTwig/Common",material,type);
    
    //VisDT4DSegmentTwig
    addMaterialAlias("VisDT4DSegmentTwig/Common","VisDT2DSegmentTwig/Common",type);
    IgSbColorMap::unpack (0xff000000, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisDT4DSegmentTwig/*",material,type);
    
    //VisMuonTwig
    addMaterialAlias("VisMuonTwig/*","VisEventIdTwig/*",type);
    
    //VisTrackTwig
    addMaterialAlias("VisTrackTwig/*","VisEventIdTwig/*",type);
    
    //VisCSCSegmentTwig
    material->diffuseColor.setValue (0.35, 0.35, 0.35);
    material->emissiveColor.setValue (0.35, 0.35, 0.35);
    addMaterial("VisCSCSegmentTwig/*/Layer",material,type);
    material->ambientColor.setValue (1.0, 0.0, 0.0);
    material->diffuseColor.setValue (1.0, 0.0, 0.0);
    material->specularColor.setValue (1.0, 0.0, 0.0);
    material->emissiveColor.setValue(0.0, 0.0, 0.0);
    material->shininess.setValue(0.1);
    addMaterial("VisCSCSegmentTwig/*/Segment",material,type);
    material->ambientColor.setValue (0.135, 0.2225, 0.1575);
    material->diffuseColor.setValue (1.00, 0.28, 0.49);
    material->specularColor.setValue (0.316228, 0.316228, 0.316228);
    material->emissiveColor.setValue(0.0, 0.0, 0.0);
    material->shininess.setValue(0.1);
    addMaterial("VisCSCSegmentTwig/*/SegmentEnvelope",material,type);
    material->diffuseColor.setValue (0.35, 0.35, 0.35);
    material->emissiveColor.setValue (0.35, 0.35, 0.35);
    material->shininess = shininessDefault;
    material->transparency = transparencyDefault;
    material->specularColor = specularDefault;
    material->ambientColor = ambientDefault;
    addMaterial("VisCSCSegmentTwig/*",material,type);
    material->emissiveColor = emissiveDefault;
    
    //VisDTDigiTwig/1
    IgSbColorMap::unpack (0x9900cc00, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisDTDigiTwig/1/3D",material,type);
    
    IgSbColorMap::rgb2hsv (rgbcomponents, tmprgbcomponents);
    tmprgbcomponents [2] *= 0.5;
    IgSbColorMap::hsv2rgb (tmprgbcomponents, tmprgbcomponents);
    material->ambientColor.setValue (SbColor (tmprgbcomponents));
    material->diffuseColor.setValue (SbColor (tmprgbcomponents));
    material->specularColor.setValue (SbColor (tmprgbcomponents));
    material->emissiveColor.setValue (SbColor (tmprgbcomponents));
    addMaterial("VisDTDigiTwig/1/*/Wire",material,type);
    
    material->ambientColor.setValue (SbColor (rgbcomponents));
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    material->specularColor.setValue (SbColor (rgbcomponents));
    material->emissiveColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisDTDigiTwig/1/*/Frame",material,type);
    
    addMaterialAlias("VisDTDigiTwig/Text","Default",type);
    
    //VisDTDigiTwig/*
    IgSbColorMap::unpack (0x66ff0000, rgbcomponents);
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisDTDigiTwig/*/3D",material,type);
    
    IgSbColorMap::rgb2hsv (rgbcomponents, tmprgbcomponents);
    tmprgbcomponents [2] *= 0.5;
    IgSbColorMap::hsv2rgb (tmprgbcomponents, tmprgbcomponents);
    material->ambientColor.setValue (SbColor (tmprgbcomponents));
    material->diffuseColor.setValue (SbColor (tmprgbcomponents));
    material->specularColor.setValue (SbColor (tmprgbcomponents));
    material->emissiveColor.setValue (SbColor (tmprgbcomponents));
    addMaterial("VisDTDigiTwig/*/Wire",material,type);
    
    material->ambientColor.setValue (SbColor (rgbcomponents));
    material->diffuseColor.setValue (SbColor (rgbcomponents));
    material->specularColor.setValue (SbColor (rgbcomponents));
    material->emissiveColor.setValue (SbColor (rgbcomponents));
    addMaterial("VisDTDigiTwig/*/Frame",material,type);
    
    //VisCSCRecHit2DTwig
    material->ambientColor.setValue (0.135, 0.2225, 0.1575);
    material->diffuseColor.setValue (0.54, 0.89, 0.63);
    material->specularColor.setValue (0.316228, 0.316228, 0.316228);
    material->emissiveColor.setValue(0.0, 0.0, 0.0);
    material->shininess.setValue(0.1);
    addMaterial("VisCSCRecHit2DTwig/*",material,type);
    
    //VisRPCRecHitTwig
    addMaterialAlias("VisRPCRecHitTwig/*","VisCSCRecHit2DTwig/*",type);
    
    //VisCaptions
    material->diffuseColor.setValue (0.0, 0.0, 0.0);
    addMaterial("VisCaptions",material,type);
    material->diffuseColor.setValue (0.0, 0.0, 1.0);
    addMaterial("VisCaptions/Info",material,type);
    material->diffuseColor.setValue (1.0, 0.0, 0.0);
    addMaterial("VisCaptions/Text",material,type);

    /////////////////////VisSimulation//////////////////////////
    //VisTkSimTrackTwig
    material->diffuseColor.setValue (1.0, 1.0, 0.0);
    addMaterial("VisTkSimTrackTwig/*",material,type);
    
    //VisTkSimHitTwig
    material->diffuseColor.setValue (1.0, 0.0, 1.0);
    material->emissiveColor.setValue(1.0, 0.0, 1.0);
    addMaterial("VisTkSimHitTwig/3D",material,type);
    
    material->diffuseColor.setValue (1.0, 0.0, 1.0);
    addMaterial("VisTkSimHitTwig/RPhi",material,type);
    
    material->diffuseColor.setValue (0.3, 0.8, 0.0);
    addMaterial("VisTkSimHitTwig/RZ",material,type);
    
    //VisSimVertexTwig
    material->diffuseColor.setValue (1.0, 0.0, 1.0);
    material->emissiveColor.setValue(1.0, 0.0, 1.0);
    addMaterial("VisSimVertexTwig/*",material,type);

    //VisSimTrackTwig
    material->ambientColor.setValue (0.000, 0.000, 0.000); 
    material->diffuseColor.setValue (1.000, 1.000, 0.000);  
    material->emissiveColor.setValue (1.000, 1.000, 0.000); 
    material->specularColor.setValue (0.000, 0.000, 0.000); 
    material->shininess.setValue (0.0);
    material->transparency.setValue (0.0);
    addMaterial("VisSimTrackTwig/*",material,type);

    //VisPSimHitTwig
    material->diffuseColor.setValue (0.0, 0.0, 1.0);
    material->emissiveColor.setValue(0.0, 0.0, 1.0);
    addMaterial("VisPSimHitTwig/*",material,type);

    //VisPCaloHitTwig
    material->diffuseColor.setValue (0.0, 0.0, 1.0);
    addMaterial("VisPCaloHitTwig/3D",material,type);
    
    material->diffuseColor.setValue (0.6, 0.2, 0.8);
    addMaterial("VisPCaloHitTwig/Hcal/*",material,type);
    
    material->diffuseColor.setValue (1.0 , 0.0, 0.6);
    addMaterial("VisPCaloHitTwig/Ecal/*",material,type);
    
    material->diffuseColor.setValue (0.2, 1.0, 0.4);
    addMaterial("VisPCaloHitTwig/RZ",material,type);
    
    material->unref();
}

void
IgTwigConfigElement::initDrawStyle ()
{
    SoDrawStyle	*drawStyle = new SoDrawStyle;
    drawStyle->ref();
    drawStyle->style.setValue (SoDrawStyleElement::LINES);
    
    // remember our defaults
    SoSFEnum styleDefault;
    SoSFFloat pointSizeDefault, lineWidthDefault;
    SoSFUShort linePatternDefault;
    styleDefault = drawStyle->style;
    pointSizeDefault = drawStyle->pointSize;
    lineWidthDefault = drawStyle->lineWidth;
    linePatternDefault = drawStyle->linePattern;
    
    std::string type("Default");
    
    drawStyle->lineWidth.setValue(5);
    type = "Thick3";
    addDrawStyle("*",drawStyle,type);
    
    drawStyle->lineWidth.setValue(5);
    type = "Thick5";
    addDrawStyle("*",drawStyle,type);
    
    drawStyle->lineWidth.setValue(2.0F);
    type = "Default";
    addDrawStyle("VisCSCWireDigiTwig/*/Layer",drawStyle,type);
    
    drawStyle->lineWidth = lineWidthDefault; // or 0=>use default width
    drawStyle->pointSize.setValue(1.0F);
    type = "Default";
    addDrawStyle("VisCSCWireDigiTwig/Spare/",drawStyle,type);
    addDrawStyleAlias("VisCSCStripDigiTwig/Spare","VisCSCWireDigiTwig/Spare/",type);

    drawStyle->pointSize = pointSizeDefault;
    type = "Default";
    addDrawStyle("VisCSCWireDigiTwig/Spare/Wire",drawStyle,type);
    addDrawStyleAlias("VisCSCStripDigiTwig/Spare/Strip","VisCSCWireDigiTwig/Spare/Wire",type);

    drawStyle->pointSize.setValue(3.0F);
    type = "Default";
    addDrawStyle("VisCSCWireDigiTwig/RZ",drawStyle,type);
    addDrawStyleAlias("VisCSCStripDigiTwig/RZ","VisCSCWireDigiTwig/RZ",type);

    drawStyle->lineWidth.setValue(2.0F); 
    drawStyle->pointSize = pointSizeDefault;
    type = "Default";
    addDrawStyle("VisCSCSegmentTwig/*/Segment",drawStyle,type);

    drawStyle->lineWidth.setValue(1.0F); // 0=>use default width
    drawStyle->linePattern.setValue(0x3333);
    type = "Default";
    addDrawStyle("VisCSCSegmentTwig/*/SegmentEnvelope",drawStyle,type);

    drawStyle->lineWidth = lineWidthDefault;
    drawStyle->pointSize = pointSizeDefault;
    drawStyle->linePattern = linePatternDefault;

    type = "Default";
    addDrawStyle("*",drawStyle,type);
 
    drawStyle->unref();
    
}

void
IgTwigConfigElement::initVisibility ()
{
    addVisibility("*",false,"None");
    addVisibility("*",true,"All");
    
    std::string type("Default");
    addVisibility("VisDetIvTwig/*",false,type);

    /*std::string prefix("/Objects/CMS Event and Detector/");
    addVisibility(prefix+"Muon/Barrel/Drift Tubes",false,type);
    addVisibility(prefix+"Muon/Barrel/Drift Tubes/Drift Tubes Superlayers",false,type);
    addVisibility(prefix+"Muon/Barrel/Drift Tubes/Drift Tubes Superlayers/Drift Tubes Superlayers (details)",false,type);
    addVisibility(prefix+"Muon/Barrel/RPCs",false,type);
    addVisibility(prefix+"Muon/Barrel/Absorber",false,type);
    addVisibility(prefix+"Muon/Endcap/CSCs",false,type);
    addVisibility(prefix+"Muon/Endcap/RPCs",false,type);
    addVisibility(prefix+"Muon/Endcap/Absorber",false,type);
    addVisibility(prefix+"ECAL/Barrel",false,type);
    addVisibility(prefix+"ECAL/Forward",false,type);
    addVisibility(prefix+"ECAL/Preshower",false,type);
    addVisibility(prefix+"HCAL/Barrel",false,type);
    addVisibility(prefix+"HCAL/Barrel/Barrel Layers",false,type);
    addVisibility(prefix+"HCAL/Barrel/Towers",false,type);
    addVisibility(prefix+"HCAL/Endcap",false,type);
    addVisibility(prefix+"HCAL/Forward",false,type);
    addVisibility(prefix+"HCAL/Outer",false,type);
    addVisibility(prefix+"Tracker/Tracker",false,type);
    addVisibility(prefix+"Beampipe",false,type);
    addVisibility(prefix+"Magnet",false,type);
    */
}

void
IgTwigConfigElement::replaceMaterial (SoNode* node, const std::string& name,
                                      const std::string& type) const
{
    SoMaterial* newNode = material(name,type);
    if (newNode)
    {
        newNode->ref();
	replaceMaterial_(node,newNode);
	newNode->unref();
    }
}


void
IgTwigConfigElement::replaceDrawStyle (SoNode* node, const std::string& name,
                                       const std::string& type) const
{
    SoDrawStyle* newNode = drawStyle(name,type);
    if (newNode)
    {
        newNode->ref();
        replaceDrawStyle_(node,newNode);
        newNode->unref();
    }
}

void
IgTwigConfigElement::replaceMaterial_ (SoNode* oldNode, const SoMaterial* newNode) const
{
    if (oldNode->isOfType(SoMaterial::getClassTypeId ()))
    {
	SoMaterial* oldMaterial = dynamic_cast<SoMaterial*>(oldNode);
        oldMaterial->shininess = newNode->shininess;
        oldMaterial->transparency = newNode->transparency;
        oldMaterial->ambientColor = newNode->ambientColor;
        oldMaterial->diffuseColor = newNode->diffuseColor;
        oldMaterial->specularColor = newNode->specularColor;
        oldMaterial->emissiveColor = newNode->emissiveColor;
	return;
    }
        
   if (oldNode->isOfType(SoGroup::getClassTypeId ()))
   {
       SoGroup* group = dynamic_cast<SoGroup*>(oldNode);
       int children = group->getNumChildren ();
       for(int i = 0; i < children; i++)
           replaceMaterial_ (group->getChild (i), newNode);
   }
}

void
IgTwigConfigElement::replaceDrawStyle_ (SoNode* oldNode, const SoDrawStyle* newNode) const
{
    if (oldNode->isOfType(SoDrawStyle::getClassTypeId ()))
    {
	SoDrawStyle* oldDrawStyle = dynamic_cast<SoDrawStyle*>(oldNode);
	int oldstyle = oldDrawStyle->style.getValue ();
	if (oldstyle == newNode->style.getValue ())
	{
	    switch (oldstyle)
	    {
	      case SoDrawStyle::LINES:
	          oldDrawStyle->lineWidth = newNode->lineWidth;
	          oldDrawStyle->linePattern = newNode->linePattern;
	          break;
	      case SoDrawStyle::POINTS:
	          oldDrawStyle->pointSize = newNode->pointSize;
	          break;
	      default:
	          break;
	    }
	}
	return;
    }

    if (oldNode->isOfType(SoGroup::getClassTypeId ()))
    {
        SoGroup* group = dynamic_cast<SoGroup*>(oldNode);
        int children = group->getNumChildren ();
        for(int i = 0; i < children; i++)
            replaceDrawStyle_ (group->getChild (i), newNode);
    }
}


void
IgTwigConfigElement::addMaterialAlias  (const std::string& alias, const std::string& to,
                                        const std::string& type)
{ ADD_SO_TWIG_CONFIG_ALIAS(m_material,Material); }

void
IgTwigConfigElement::addDrawStyleAlias (const std::string& alias, const std::string& to,
                                        const std::string& type)
{ ADD_SO_TWIG_CONFIG_ALIAS(m_drawStyle,DrawStyle); }

SoMaterial*
IgTwigConfigElement::findMaterial  (const std::string& name,const std::string& type) const
{
    FIND_TWIG_CONFIG_OBJECT(m_material,Material);
    return 0;
}

SoDrawStyle*
IgTwigConfigElement::findDrawStyle (const std::string& name, const std::string& type) const
{
    FIND_TWIG_CONFIG_OBJECT(m_drawStyle,DrawStyle);
    return 0;
}

bool
IgTwigConfigElement::findVisibility (const std::string& name, const std::string& type) const
{
    FIND_TWIG_CONFIG_OBJECT(m_visibility,Visibility);
    return false;
}
