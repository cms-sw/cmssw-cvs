//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/GLBrowsers/interface/IgPlaneDirection.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


/** This class is basically a SbVec3f with the additional functionality of
	emitting a signal as soon as its values are changed */
IgPlaneDirection::IgPlaneDirection () : m_vector (SbVec3f (1.0, 0.0, 0.0))
{}

IgPlaneDirection::IgPlaneDirection (const SbVec3f &vec) : m_vector (vec)
{}

IgPlaneDirection::IgPlaneDirection (const float x, const float y, const float z)
{
	IgPlaneDirection (SbVec3f (x,y,z));
}

bool
IgPlaneDirection::isDirty ()
{
	// is never dirty because we are updating the values immediately
	return false;
}

void
IgPlaneDirection::setValue (float x, float y, float z)
{
	setValue (SbVec3f (x,y,z));
}

void
IgPlaneDirection::setValue (const SbVec3f &vec)
{
	m_vector = vec;
	emit dirty ();
}

SbVec3f
IgPlaneDirection::value ()
{
	return SbVec3f (m_vector[0], m_vector[1], m_vector[2]);
}
