#ifndef InputSort_H
#define InputSort_H

#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicTree.h"

/**
 * Helper class checking the
 * input of Kinematic Vertex Fitters
 * If some of particles provided have 
 * trees after them, makes sure that
 * only top tree particles are used in the fit.
 */

class InputSort{

public:

 InputSort(){}
 ~InputSort(){}
 
 pair<vector<RefCountedKinematicParticle>, vector<FreeTrajectoryState> > 
                          sort(vector<RefCountedKinematicParticle> particles) const;

 vector<RefCountedKinematicParticle> sort(vector<RefCountedKinematicTree> trees) const;
 
private:

};
#endif
