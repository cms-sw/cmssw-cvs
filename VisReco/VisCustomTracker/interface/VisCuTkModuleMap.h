#ifndef VisCuTkModuleMap_H
#define VisCuTkModuleMap_H

#include "VisReco/VisCustomTracker/interface/VisCuTkModule.h"

#include <map>
class GeomDetUnit;
using namespace std;
class VisCuTkModuleMap {
 public:
  static map<const GeomDetUnit * , VisCuTkModule *> moduleMap;
 };
#endif
