#ifndef IG_G4_DEMO_IG_G4_DEMO_DETECTOR_H
# define IG_G4_DEMO_IG_G4_DEMO_DETECTOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisExamples/VisG4ExN02/interface/config.h"
# include <G4VUserDetectorConstruction.hh>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class VisG4ExampleField;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Box;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Sample Geant4 detector, based on Geant4 example N02.  (FIXME: Use
    Andrea's BarrelSaggin detector.)  */
class IG_G4_DEMO_API VisG4ExampleDetector : public G4VUserDetectorConstruction
{
public:
    VisG4ExampleDetector (void);
    ~VisG4ExampleDetector (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual G4VPhysicalVolume *	Construct (void);

    double			fullLength (void) const;

private:
    G4Box			*m_worldSol;	//< Envelope solid
    G4LogicalVolume		*m_worldLV;	//< Envelope logical volume
    G4VPhysicalVolume		*m_worldPV;	//< Envelope physical volume

    G4Box			*m_targetSol;	//< Target solid
    G4LogicalVolume		*m_targetLV;	//< Target logical volume
    G4VPhysicalVolume		*m_targetPV;	//< Target physical volume

    G4Box			*m_trackerSol;	//< Tracker solid
    G4LogicalVolume		*m_trackerLV;	//< Tracker logical volume
    G4VPhysicalVolume		*m_trackerPV;	//< Tracker physical volume

    G4Box			*m_chamberSol;	//< Chamber solid
    G4LogicalVolume		*m_chamberLV;	//< Chamber logical volume
    G4VPhysicalVolume		*m_chamberPV;	//< Chamber physical volume

    G4Material			*m_matPb;	//< Target material
    G4Material			*m_matXenon;	//< Chamber material

    VisG4ExampleField		*m_field;	//< Magnetic field

    static const double		WORLD_SIZE;	//< Full world volume length
    static const double		TARGET_SIZE;	//< Full target length
    static const double		TRACKER_SIZE;	//< Full tracker length
    static const int		N_CHAMBERS;	//< Number of tracker chambers
    static const double		CHAMBER_SIZE;	//< Width of a chamber
    static const double		CHAMBER_SPACING;//< Space between chambers
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline double
VisG4ExampleDetector::fullLength (void) const
{ return WORLD_SIZE; }

#endif // IG_G4_DEMO_IG_G4_DEMO_DETECTOR_H
