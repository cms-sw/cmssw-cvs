/*
#include "CondCore/DBCommon/interface/ClassInfoLoader.h"
#include "CondCore/DBCommon/interface/ClassID.h"
#include "CondCore/DBCommon/interface/Exception.h"

#include "StorageSvc/DbReflex.h"

#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/PluginCapabilities.h"

namespace cond {

  // decode token
  std::string classID(std::string const & token) {
    static std::string const clid("CLID=");
    std::string::size_type s = token.find(clid) + clid.size();
    std::string::size_type e = token.find(']',s);
    return token.substr(s,e-s);

  }

  // decode token
  std::string containerName(std::string const & token) {
    static std::string const clid("CNT=");
    std::string::size_type s = token.find(clid) + clid.size();
    std::string::size_type e = token.find(']',s);
    return token.substr(s,e-s);

  }

  // find and return
  boost::shared_ptr<ClassInfo> classInfo(std::string const & token) {
    static std::string const prefix = cond::idCategories::dictIDCategory + "/";
    std::string pluginName = prefix + classID(token);
    return boost::shared_ptr<ClassInfo>(ClassInfoFactory::get()->create(pluginName));
  }
  
  // load plugin (delete classInfo)
  bool loadClassByToken(std::string const & token) {
     boost::shared_ptr<ClassInfo> ci = classInfo(token);
     // will never return false as pluginMgr throw!
     return ci.get()!=0;
  }


  namespace {
    std::string const errmsg("Unable to load class for token ");
    std::string const orimsg(". Original error msg was ");

  }

  TypeH reflexTypeByToken(std::string const & token) {
    const pool::Guid guid(cond::classID(token));
    {
      // look if already loaded
      TypeH type = pool::DbReflex::forGuid(guid);
      if (type) return type;
    }
    // try with name of container...
    try {
      static std::string const prefix("LCGReflex/");
      edmplugin::PluginCapabilities::get()->load(prefix + containerName(token));
      TypeH type = pool::DbReflex::forGuid(guid);
      if (type) return type;
    } catch(...){}
    // try our plugin
    try {
      // plugin mgr will throw fist: still
      if (!cond::loadClassByToken(token)) 
	throw cond::Exception(errmsg+token);
    }
    catch (cms::Exception const & e) {
      throw cond::Exception(errmsg+token+orimsg+e.what());
    }
    TypeH type = pool::DbReflex::forGuid(guid);
    if (!type) throw cond::Exception(errmsg+token +". Problem with DbReflex");
    return type;
  }

  std::string classNameForTypeId( const std::type_info& typeInfo ){
    TypeH reflexType = TypeH::ByTypeInfo( typeInfo );
    return reflexType.Name();
  }

}
*/
