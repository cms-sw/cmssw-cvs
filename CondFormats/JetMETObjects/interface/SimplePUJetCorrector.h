//
// Original Author:  Fedor Ratnikov Dec 27, 2006
// $Id: SimplePUJetCorrector.h,v 1.2.4.1 2009/12/11 12:56:27 kkousour Exp $
//
// MC Jet Corrector
//
#ifndef SimplePUJetCorrector_h
#define SimplePUJetCorrector_h

#include <map>
#include <string>

/// classes declaration
namespace PUnamespace{
  class ParametrizationPUJet;
  typedef std::map <double, ParametrizationPUJet*> ParametersMap;
}

class SimplePUJetCorrector {
 public:
  SimplePUJetCorrector ();
  SimplePUJetCorrector (const std::string& fDataFile);
  virtual ~SimplePUJetCorrector ();

  void init (const std::string& fDataFile);

  virtual double correctionXYZT (double fPx, double fPy, double fPz, double fE) const;
  virtual double correctionPtEtaPhiE (double fPt, double fEta, double fPhi, double fE) const;
  virtual double correctionEtEta (double fEt, double fEta) const;

 private:
  SimplePUJetCorrector (const SimplePUJetCorrector&);
  SimplePUJetCorrector& operator= (const SimplePUJetCorrector&);
  PUnamespace::ParametersMap* mParametrization;
};

#endif
