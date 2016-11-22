#ifndef COND_DBCommon_ClassInfoLoader_H
#define COND_DBCommon_ClassInfoLoader_H

/**
 * loads plugin corresponding to token....
 */

#include<boost/shared_ptr.hpp>
#include<string>
#include "RVersion.h"

#if ROOT_VERSION_CODE < ROOT_VERSION(5,19,0)
namespace ROOT{
  namespace Reflex {
    class Type;
  }
}
typedef ROOT::Reflex::Type TypeH;
#else
namespace Reflex {
  class Type;
}
typedef Reflex::Type TypeH;
#endif
namespace cond {

  class ClassInfo;

  // decode token
  std::string classID(std::string const & token);

  // decode token (container==classname...)
  std::string container(std::string const & token);


  // find and return
  boost::shared_ptr<ClassInfo> classInfo(std::string const & token);

  // load plugin (delete classInfo)
  bool loadClassByToken(std::string const & token);

  TypeH reflexTypeByToken(std::string const & token);

  std::string classNameForTypeId( const std::type_info& typeInfo );

  template <typename T> std::string classNameForPointer( T* pointer ){
    if(!pointer) return classNameForTypeId( typeid(T) );
    return classNameForTypeId( typeid(*pointer) );
  }

}


#endif // COND_DBCommon_ClassInfoLoader_H
