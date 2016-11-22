#ifndef VIS_G4_VOLUMES_VIS_G4_TWIG_OPS_H
# define VIS_G4_VOLUMES_VIS_G4_TWIG_OPS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisGeant4/VisG4Volumes/interface/config.h"
# include <qobject.h>
# include <string>
# include <set>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisG4VolumeTwig;
class IgQtObjectMenu;
class IgQtObjectMenuMessage;
class G4Material;
class G4VisAttributes;
class G4LogicalVolume;
class G4VSolid;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Geant4-related twig context menu operations.  */
class VIS_G4_VOLUMES_API VisG4TwigOps : public QObject
{
    Q_OBJECT
public:
    static const int MENU_LOGICAL		= 3;
    static const int MENU_GET_MASS		= 4;

    static const int MENU_SHOW_CATEGORY		= 20;
    static const int MENU_SHOW_MATERIAL		= 21;

    static const int MENU_HIDE_CATEGORY		= 60;
    static const int MENU_HIDE_MATERIAL		= 61;

    static const int MENU_RENDER_SEP		= 80;
    static const int MENU_APPEARANCE		= 81;
    static const int MENU_RENDER		= 82;
    static const int MENU_RENDER_AS_IS		= 8200;
    static const int MENU_RENDER_SOLID		= 8201;
    static const int MENU_RENDER_WIREFRAME	= 8202;
    static const int MENU_RENDER_ALL		= 83;
    static const int MENU_COLOUR		= 84;
    static const int MENU_TRANSPARENCY		= 85;

    static const int CAT_SENSITIVE		= 2001;
    static const int CAT_INSENSITIVE		= 2002;
    static const int CAT_BEAMPIPE		= 2003;
    static const int CAT_CABLE			= 2004;
    static const int CAT_COOLING		= 2005;
    static const int CAT_MAGNET			= 2006;
    static const int CAT_SUPPORT		= 2007;
    static const int CAT_SENSITIVE_CONT		= 2008;
    static const int CAT_FILTER			= 2009;


    // implicit constructor
    // implicit destructor
    // implicit copy constructor
    // implicit assignment operator

    void		menuAction (IgQtObjectMenuMessage message);
    void		menuUpdate (void);

public slots:
    void		actionLogical (int id);
    void		actionGetMass (int id);
    void		actionShowCategory (int id);
    void		actionHideCategory (int id);
    void		actionShowMaterial (int id);
    void		actionRender (int id);
    void		actionRenderAll (int id);
    void		actionColour (int id);
    void		actionTransparency (int id);
    void 		actionApplyFilter (int id);    
    void 		actionAppearance (int id);    

private:
    double		getMass (G4LogicalVolume *volume, G4Material *material);
    double		estimateCubicVolume (G4VSolid *solid);
    
    void		doShowCategory (bool show, int id, VisG4VolumeTwig *twig);
    void		doShowMaterial (bool show, VisG4VolumeTwig *twig);
    void		doRender (int id, VisG4VolumeTwig *twig, bool recurse);
    void		doApplyFilter (bool show, int id, VisG4VolumeTwig *twig);    

    VisG4VolumeTwig		*m_twig;
    IgQtObjectMenu		*m_menu;
    std::set<std::string>	m_selected;
    std::set<G4VisAttributes *>	m_changed;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIS_G4_VOLUMES_VIS_G4_TWIG_OPS_H
