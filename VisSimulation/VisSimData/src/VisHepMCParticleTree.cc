//<<<<<< INCLUDES                                                       >>>>>>

#include "VisSimulation/VisSimData/interface/VisHepMCParticleTree.h"
#include <HepMC/GenParticle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <sstream>
#include <vector>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

HepMCParticleTree::HepMCParticleTree (std::vector <HepMC::GenParticle* > particles)
{    
    std::vector<HepMCParticleTree* > mapping (particles.size ());    
    std::vector<HepMCParticleTree* >::iterator itdt = mapping.begin ();

    for (std::vector<HepMC::GenParticle* >::iterator iter = particles.begin ();
	   iter != particles.end (); 
	   iter++ )
    {
        (*itdt) = new HepMCParticleTree (*iter);
	itdt ++;
    }
    
    for (std::vector<HepMCParticleTree* >::iterator idt = mapping.begin (); idt != mapping.end (); idt++)
    {
//         if ((*idt)->m_particle->vertex ().hasParent ())
// 	{
//   	    mapping[(*idt)->m_particle->vertex ().mother ().id () ]->m_daughters.push_back (*idt);
// 	} 
// 	else 
// 	{
// 	    m_daughters.push_back (*idt);
// 	}
    }
}

HepMCParticleTree::HepMCParticleTree (HepMC::GenParticle * particle) 
    : m_particle (particle)
{}

HepMCParticleTree::~HepMCParticleTree (void)
{
    if (m_daughters.size () != 0)
	for (std::vector<HepMCParticleTree* >::iterator idt = m_daughters.begin (); 
	     idt != m_daughters.end ();
	     idt++)
	{
	    delete (*idt);
	}
}

//float 
//HepMCParticleTree::recursiveMakeSeparatorTree (SoSeparator *separator, float x//, float *nextY, float z)
//{
//    const float delta = 1.0;
    //    IgSoSimpleTrajectory *track = new IgSoSimpleTrajectory;
//    SbVec3f pnt;
//    float mx = x;
//    float yBar = 0;
//    float y;
//    x += delta;

//    int size;

//    if ((size = m_daughters.size ()) != 0)
//    {
//        vector<IgSoSimpleTrajectory* > trajs (size);
	
//	vector<VisSimTracksTree* >::iterator it = m_daughters.begin (); 
//        for (vector<IgSoSimpleTrajectory* >::iterator trajIt = trajs.begin ();// trajIt != trajs.end (); trajIt++)
//	{
//	    y = (*it)->recursiveMakeSeparatorTree (separator, x, nextY, z);
//	    yBar += y; 
//	    (*trajIt) = new IgSoSimpleTrajectory;
//	    pnt.setValue (x, y, z);
//	    (*trajIt )->controlPoints.set1Value (0, pnt);
//	    (*trajIt )->lineWidth = 2.0;  // You can set the trajectory paramete//rs here, like linePattern, depending on the SimTracks data.
//	    it++;
	    //	    trajIt++;
	    //	    *nextY += delta;
//	}
//	yBar = yBar / size;
//	for (vector<IgSoSimpleTrajectory* >::iterator trajIt2 = trajs.begin (); //trajIt2 != trajs.end (); trajIt2++)
//	{
//	    pnt.setValue (mx, yBar, z);
//	    (*trajIt2 )->controlPoints.set1Value (1, pnt);
//	    separator->addChild (*trajIt2);
//	}	
//   } 
//   else
//    {
//        yBar = *nextY;
//	(*nextY) += delta;
//    }
//    return yBar;
//}

//void 
//VisSimTracksTree::makeSeparatorTree (SoSeparator *separator, 
//				     const float mx, 
//				     const float my, 
//				     const float mz)
//{
//   const float delta = 1.0;
//    float nextY;
//    float y = my;
//    float z = mz;
//    float x;
//    x = mx + delta;
//    nextY = y; 
    
//    if (m_daughters.size () != 0)
//   {
//        for (vector<VisSimTracksTree *>::iterator it = m_daughters.begin (); i//t != m_daughters.end (); it++)
//	{
//	    SbVec3f pnt;
//           if (!(*it))
//	    {
//		exit (0);
//            }  
//            y = (*it)->recursiveMakeSeparatorTree (separator, x, &nextY, z);

//	    IgSoSimpleTrajectory *traj = new IgSoSimpleTrajectory;
//	    pnt.setValue (x, y, z); 
//	    traj->controlPoints.set1Value (0, pnt);
//	    pnt.setValue (mx, y, z);
//	    traj->controlPoints.set1Value (1, pnt);
//	    traj->lineWidth = 2.0;
//	    separator->addChild (traj);
//	}
//   } 
//}
