#ifndef COND_DBCommon_SessionConfiguration_h
#define COND_DBCommon_SessionConfiguration_h
//
// Package:    CondCore/DBCommon
// Class:      SessionConfiguration
//
/**\class SessionConfiguration SessionConfiguration.h CondCore/DBCommon/interface/SessionConfiguration.h
 Description: set cofiguration parameters of the session
*/
//
// Author:      Zhen Xie
//
#include "CondCore/DBCommon/interface/AuthenticationMethod.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include <string>
namespace cond{
  class ConnectionConfiguration;
  class SessionConfiguration{
  public:
    SessionConfiguration();
    ~SessionConfiguration();
    ConnectionConfiguration* connectionConfiguration();
    void setAuthenticationMethod( cond::AuthenticationMethod m );
    void setAuthenticationPath( const std::string& p );
    void setBlobStreamer( const std::string& name );
    void setMessageLevel( cond::MessageLevel l );
    void startSQLMonitoring();
    cond::AuthenticationMethod authenticationMethod() const;
    bool hasBlobStreamService() const;
    std::string blobStreamerName() const;
    cond::MessageLevel messageLevel() const;
    std::string authName() const;
    bool isSQLMonitoringOn() const;
  private:
    cond::AuthenticationMethod m_authMethod;
    std::string m_authPath;
    bool m_hasBlobstreamer;
    std::string m_blobstreamerName;
    cond::MessageLevel m_messageLevel;
    cond::ConnectionConfiguration* m_conConfig;
    bool m_isSQLMonitoringOn;
  };
}
#endif
