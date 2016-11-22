#ifndef IGUANA_GL_BROWSERS_IG_TWIG_CONFIG_ELEMENT_H
# define IGUANA_GL_BROWSERS_IG_TWIG_CONFIG_ELEMENT_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/config.h"
#include "Iguana/Framework/interface/IgExtension.h"
#include <string>
#include <vector>
#include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoMaterial;
class SoDrawStyle;
class SoNode;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_GL_BROWSERS_API IgTwigConfigElement : public IgExtension
{
    IG_DECLARE_STATE_ELEMENT (IgTwigConfigElement);
public:
    IgTwigConfigElement  (IgState* state);
    ~IgTwigConfigElement ();
    
    void	       setMaterialType   (const std::string& type="Default");
    void	       setDrawStyleType  (const std::string& type="Default");
    void	       setVisibilityType (const std::string& type="Default");
    
    const std::string& getMaterialType   () const;
    const std::string& getDrawStyleType  () const;
    const std::string& getVisibilityType () const;
    
    SoMaterial*  material     (const std::string& name,
                               const std::string& type="") const;
    SoDrawStyle* drawStyle    (const std::string& name,
                               const std::string& type="") const;
    bool         visibility   (const std::string& name,
                               const std::string& type="") const;
    
    void	addMaterial   (const std::string& name, SoMaterial*  material,
                               const std::string& type="");
    void	addDrawStyle  (const std::string& name, SoDrawStyle* style,
                               const std::string& type="");
    void	addVisibility (const std::string& name, bool visibility,
                               const std::string& type="");

    void        replaceMaterial  (SoNode* node, const std::string& name,
                                  const std::string& type="") const;
    void        replaceDrawStyle (SoNode* node, const std::string& name,
                                  const std::string& type="") const;
private:
    void	init ();
    void 	initMaterial ();
    void	initDrawStyle ();
    void	initVisibility ();
    
    void	 addMaterialAlias  (const std::string& alias, const std::string& to,
                                    const std::string& type="");
    void	 addDrawStyleAlias (const std::string& alias, const std::string& to,
                                    const std::string& type="");
    SoMaterial*	 findMaterial      (const std::string& name,
                                    const std::string& type="")const;
    SoDrawStyle* findDrawStyle     (const std::string& name,
                                    const std::string& type="")const;
    bool         findVisibility    (const std::string& name,
                                    const std::string& type="")const;
    void         replaceMaterial_  (SoNode* oldNode, const SoMaterial*  newNode) const;
    void         replaceDrawStyle_ (SoNode* oldNode, const SoDrawStyle* newNode) const;

    IgState*		m_state;
    std::string		m_materialType;
    std::string		m_drawStyleType;
    std::string		m_visibilityType;

    typedef std::pair<std::string, SoMaterial*>  MaterialInfo;
    typedef std::pair<std::string, SoDrawStyle*> DrawStyleInfo;
    typedef std::pair<std::string, bool>         VisibilityInfo;
    typedef std::vector<MaterialInfo>            Material;
    typedef std::vector<DrawStyleInfo>           DrawStyle;
    typedef std::vector<VisibilityInfo>          Visibility;
    typedef std::map<std::string,Material>       MaterialMap;
    typedef std::map<std::string,DrawStyle>      DrawStyleMap;
    typedef std::map<std::string,Visibility>     VisibilityMap;
    
    MaterialMap		m_material;
    DrawStyleMap	m_drawStyle;
    VisibilityMap	m_visibility;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_GL_BROWSERS_IG_TWIG_CONFIG_ELEMENT_H
