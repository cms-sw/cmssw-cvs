#ifndef VisCuTkModuleMap_H
#define VisCuTkModuleMap_H

#include "VisReco/VisCustomTracker/interface/VisCuTkModule.h"

#include <map>
class GeomDetUnit;

class VisCuTkModuleMap {
 public:
  static std::map<const GeomDetUnit * , VisCuTkModule *> moduleMap;
 };
#endif
