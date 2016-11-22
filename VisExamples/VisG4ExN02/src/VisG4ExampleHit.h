#ifndef IG_G4_DEMO_IG_G4DEMO_HIT_H
# define IG_G4_DEMO_IG_G4DEMO_HIT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "VisExamples/VisG4ExN02/interface/config.h"
# include <G4ThreeVector.hh>
# include <G4THitsCollection.hh>
# include <G4Allocator.hh>
# include <G4VHit.hh>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_G4_DEMO_API VisG4ExampleHit : public G4VHit
{
public:
    VisG4ExampleHit (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    inline void *	operator new (size_t sz);
    inline void		operator delete (void *p);

    virtual void	Draw (void);
    virtual void	Print (void);

    int			trackId (void) const;
    void		setTrackId (int n);

    int			chamber (void) const;
    void		setChamber (int n);

    double		energy (void) const;
    void		setEnergy (double energy);

    G4ThreeVector	position (void) const;
    void		setPosition (G4ThreeVector pos);

private:
    int			m_trackId;
    int			m_chamber;
    double		m_energy;
    G4ThreeVector	m_position;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

typedef G4THitsCollection<VisG4ExampleHit> VisG4ExampleHitsCollection;
extern  G4Allocator<VisG4ExampleHit>       VisG4ExampleHitAllocator;

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline int
VisG4ExampleHit::trackId (void) const
{ return m_trackId; }

inline void
VisG4ExampleHit::setTrackId (int n)
{ m_trackId = n; }

//////////////////////////////////////////////////////////////////////
inline int
VisG4ExampleHit::chamber (void) const
{ return m_chamber; }

inline void
VisG4ExampleHit::setChamber (int n)
{ m_chamber = n; }

//////////////////////////////////////////////////////////////////////
inline double
VisG4ExampleHit::energy (void) const
{ return m_energy; }

inline void
VisG4ExampleHit::setEnergy (double energy)
{ m_energy = energy; }

//////////////////////////////////////////////////////////////////////
inline G4ThreeVector
VisG4ExampleHit::position (void) const
{ return m_position; }

inline void
VisG4ExampleHit::setPosition (G4ThreeVector pos)
{ m_position = pos; }

//////////////////////////////////////////////////////////////////////

inline void *
VisG4ExampleHit::operator new (size_t)
{ return VisG4ExampleHitAllocator.MallocSingle (); }

inline void
VisG4ExampleHit::operator delete (void *hit)
{ VisG4ExampleHitAllocator.FreeSingle ((VisG4ExampleHit *) hit); }

#endif // IG_G4_DEMO_IG_G4DEMO_HIT_H
