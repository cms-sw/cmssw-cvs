#ifndef EcalSimAlgos_EcalCorrelatedNoiseMatrix_h
#define EcalSimAlgos_EcalCorrelatedNoiseMatrix_h

/*
 *
 * $Id: EcalCorrelatedNoiseMatrix.h,v 1.2 2007/12/03 11:25:14 crovelli Exp $
 *
 */

#include "DataFormats/Math/interface/Error.h"
#include "CalibFormats/CaloObjects/interface/CaloSamples.h"

typedef math::ErrorD<CaloSamples::MAXSAMPLES>::type EcalCorrMatrix;

class EcalCorrelatedNoiseMatrix
{

 public:

  explicit EcalCorrelatedNoiseMatrix(const EcalCorrMatrix & matrix);

  ~EcalCorrelatedNoiseMatrix() {};

  void setMatrix(const EcalCorrMatrix & matrix) { theMatrix = matrix; }

  void getMatrix(EcalCorrMatrix & matrix) { matrix = theMatrix; }

private:

  EcalCorrMatrix theMatrix;
  unsigned int theSize; 
};

#endif
