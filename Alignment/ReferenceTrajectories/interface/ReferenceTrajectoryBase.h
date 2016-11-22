#ifndef Alignment_ReferenceTrajectories_ReferenceTrajectoryBase_H
#define Alignment_ReferenceTrajectories_ReferenceTrajectoryBase_H

/**
 * Author     : Gero Flucke (based on code for ORCA by Edmund Widl)
 * date       : 2006/09/17
 * last update: $Date: 2009/12/17 12:28:06 $
 * by         : $Author: flucke $
 *
 * Base class for reference 'trajectories' of single- or multiparticles
 * stated.
 * Inheriting classes have to calculate all relevant quantities accessed
 * through member functions of this base class:
 *
 * The local measured x/y coordinates on all crossed detectors as vector:
 *
 * m = (x1, y1, x2, y2, ..., xN, yN) [transposed vector shown]
 *
 * their covariance matrix (possibly containing correlations between hits
 * due to material effects which are not taken into account by the ideal
 * trajectory parametrisation, cf. below),
 *
 * similarly the local x/y cordinates of the reference trajectory with covariance,
 *
 * the parameters of the (ideal) 'trajectory' 
 * (e.g. 5 values for a single track or 9 for a two-track-state with vertex constraint),
 *
 * the derivatives of the local coordinates of the reference trajectory
 * with respect to the initial 'trajectory'-parameters, 
 * e.g. for n parameters 'p' and N hits with 'x/y' coordinates:
 * 
 *  D = ( dx1/dp1, dx1/dp2, ..., dx1/dpn,
 *
 *        dy1/dp1, dy1/dp2, ..., dy1/dpn,
 *
 *        dx2/dp1, dx2/dp2, ..., dx2/dpn,
 *
 *        dy2/dp1, dy2/dp2, ..., dy2/dpn,
 *
 *           .        .             .
 *
 *           .        .             .
 *
 *        dxN/dp1, dxN/dp2, ..., dxN/dpn,
 *
 *        dyN/dp1, dyN/dp2, ..., dyN/dpn )
 *
 * and finally the TrajectoryStateOnSurface's of the reference trajectory.
 * 
 * Take care to check validity of the calculation (isValid()) before using the results.
 *
 *.............................................................................
 *
 * 090730 C. Kleinwort: 'Break Points' introduced for better description of multiple 
 *                      scattering (correlations)
 *
 * For each detector layer (in the hit collection) two ortogonal scattering angles are
 * introduced as new local track parameters ("break points"). To constrain those to the
 * expected mean (=0.) and variance (~1/p^2 X/X0) corresponding measurements are added.
 * Invalid hits may be used to produce break points too (UseInvalidHits = True).
 *
 * Break points have been implemented for: ReferenceTrajectory, BzeroReferenceTrajectory,
 *    DualReferenceTrajectory and DualBzeroReferenceTrajectory
 * For 'Dual' trajectories they make no sense as only the correlations between the hits  
 * in a track half are accounted for and not those between the halves! 
 *
 * Break Points are selected by TrajectoryFactory.MaterialEffects = "BreakPoints"
 *
 * 090909 C. Kleinwort: 'Broken Lines' introduced for description of multiple scattering
 *      (V. Blobel, Nuclear Instruments and Methods A, 566 (2006), pp. 14-17)
 * Fine Broken Lines are selected by TrajectoryFactory.MaterialEffects = "BrokenLinesFine"
 *      (exact derivatives)
 * Coarse Broken Lines are selected by TrajectoryFactory.MaterialEffects = "BrokenLines[Coarse]"
 *      (approximate derivatives, closeby hits (ds<1cm) combined)
 *
 * 091127 C. Kleinwort: 
 *   1) 'Broken Lines' extended to PCA,
 *      required for beamspot constraint and TwoBodyDecayTrajectory,
 *      selected with "BrokenLines[Coarse]Pca" or "BrokenLinesFinePca"
 *   2) For coarse Broken Lines linear interpolation is used for combined hits
 *   3) TwoBodyDecayTrajectory implemented for break points and Broken Lines
 */

#include "DataFormats/GeometrySurface/interface/ReferenceCounted.h"

// for AlgebraicVector, -Matrix and -SymMatrix:
#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryParametrization/interface/LocalTrajectoryError.h"

#include <vector>

class ReferenceTrajectoryBase : public ReferenceCounted
{

public:

  typedef ReferenceCountingPointer<ReferenceTrajectoryBase> ReferenceTrajectoryPtr;

  enum MaterialEffects { none, multipleScattering, energyLoss, combined, breakPoints, brokenLinesCoarse, brokenLinesFine, brokenLinesCoarsePca, brokenLinesFinePca };

  virtual ~ReferenceTrajectoryBase() {}

  bool isValid() { return theValidityFlag; }

  /** Returns the measurements in local coordinates.
   */
  const AlgebraicVector& measurements() const { return theMeasurements; }

  /** Returns the full covariance matrix of the measurements. ORCA-equivalent: covariance()
   */
  const AlgebraicSymMatrix& measurementErrors() const { return theMeasurementsCov; }
  
  /** Returns the local coordinates of the reference trajectory.
      ORCA-equivalent: referenceTrack()
   */
  const AlgebraicVector& trajectoryPositions() const { return theTrajectoryPositions; }

  /** Returns the covariance matrix of the reference trajectory.
   */
  const AlgebraicSymMatrix& trajectoryPositionErrors() const { return theTrajectoryPositionCov; }

  /** Returns the derivatives of the local coordinates of the reference
   *  trajectory (i.e. trajectoryPositions) w.r.t. the initial 'trajectory'-parameters.
   */
  const AlgebraicMatrix& derivatives() const { return theDerivatives; }

  /** Returns the transformation of tracjectory to curvilinear parameters
   */
  const AlgebraicMatrix& trajectoryToCurv() const { return theInnerTrajectoryToCurvilinear; }
  /** Returns the transformation of local to tracjectory parameters
   */
  const AlgebraicMatrix& localToTrajectory() const { return theInnerLocalToTrajectory; }    

  /** Returns the set of 'track'-parameters.
   */  
  const AlgebraicVector& parameters() const { return theParameters; }

  /** Returns true if the covariance matrix of the 'track'-parameters is set.
   */
  inline bool parameterErrorsAvailable() const { return theParamCovFlag; }

  /** Set the covariance matrix of the 'track'-parameters.
   */
  inline void setParameterErrors( const AlgebraicSymMatrix& error ) { theParameterCov = error; theParamCovFlag = true; }
  
  /** Returns the covariance matrix of the 'track'-parameters.
   */
  inline const AlgebraicSymMatrix& parameterErrors() const { return theParameterCov; }

  /** Returns the Tsos at the surfaces of the hits, parallel to recHits()
   */
  const std::vector<TrajectoryStateOnSurface>& trajectoryStates() const { return theTsosVec; }

  /** Returns the TransientTrackingRecHits (as ConstRecHitPointer's), 
      order might depend on concrete implementation of inheriting class
   */
  const TransientTrackingRecHit::ConstRecHitContainer& recHits() const { return theRecHits; }

  inline unsigned int numberOfHits() const { return theNumberOfHits; }
  inline unsigned int numberOfPar() const { return theNumberOfPars; }
  inline unsigned int numberOfMsMeas() const { return theNumberOfMsMeas; }  
  inline unsigned int numberOfMsPar() const { return theNumberOfMsPars; }  
  inline unsigned int numberOfHitMeas() const { return theNumberOfHits * nMeasPerHit; } 
     
  virtual ReferenceTrajectoryBase* clone() const = 0;

protected:

  explicit ReferenceTrajectoryBase(unsigned int nPar, unsigned int nHits, unsigned int nMsPar, unsigned int nMsMeas);

  unsigned int numberOfUsedRecHits(const TransientTrackingRecHit::ConstRecHitContainer &recHits) const;
  bool useRecHit(const TransientTrackingRecHit::ConstRecHitPointer& hitPtr) const;

  bool theValidityFlag;
  bool theParamCovFlag;

  unsigned int theNumberOfHits;   // number of (measurements from) hits
  unsigned int theNumberOfPars;   // number of (track) parameters
  unsigned int theNumberOfMsMeas; // number of measurements for multiple scattering
  unsigned int theNumberOfMsPars; // number of parameters   for multiple scattering 
      
  std::vector<TrajectoryStateOnSurface> theTsosVec;
  TransientTrackingRecHit::ConstRecHitContainer theRecHits;

  AlgebraicVector     theMeasurements;
  AlgebraicSymMatrix  theMeasurementsCov;

  AlgebraicVector     theTrajectoryPositions;
  AlgebraicSymMatrix  theTrajectoryPositionCov;

  AlgebraicVector     theParameters;
  AlgebraicSymMatrix  theParameterCov;

  AlgebraicMatrix     theDerivatives;

// CHK for beamspot   transformation trajectory parameter to curvilinear at refTSos
  AlgebraicMatrix     theInnerTrajectoryToCurvilinear;  
// CHK for TwoBodyD.  transformation local to trajectory parameter at refTsos
  AlgebraicMatrix     theInnerLocalToTrajectory;  
    
  static const unsigned int nMeasPerHit = 2;
};

#endif // REFERENCE_TRAJECTORY_BASE_H

