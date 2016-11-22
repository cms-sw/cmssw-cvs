//
// Original Author:  Fedor Ratnikov Oct 31, 2007
// $Id: SimpleL5FlavorCorrector.h,v 1.5 2008/08/28 20:11:13 kkousour Exp $
//
// Jet flavor dependent corrections
//
#ifndef SimpleL5FlavorCorrector_h
#define SimpleL5FlavorCorrector_h

#include <string>

class SimpleJetCorrectorParameters;

class SimpleL5FlavorCorrector {
 public:
  SimpleL5FlavorCorrector ();
  SimpleL5FlavorCorrector (const std::string& fDataFile, const std::string& fSection);
  virtual ~SimpleL5FlavorCorrector ();

  virtual double correctionXYZT (double fPx, double fPy, double fPz, double fE) const;
  virtual double correctionPtEta (double fPt, double fEta) const;
  virtual double correctionEtEtaPhiP (double fEt, double fEta, double fPhi, double fP) const;

 private:
  SimpleL5FlavorCorrector (const SimpleL5FlavorCorrector&);
  SimpleL5FlavorCorrector& operator= (const SimpleL5FlavorCorrector&);
  double correctionBandPtEta (unsigned fBand, double fPt, double fEta) const;
  SimpleJetCorrectorParameters* mParameters;
};

#endif
