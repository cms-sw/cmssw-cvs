#ifndef COND_DBCommon_ConnectionConfiguration_h
#define COND_DBCommon_ConnectionConfiguration_h
#include "RelationalAccess/IMonitoring.h"
//
// Package:     DBCommon
// Class  :     Connection
//
/**\class ConnectionConfiguration ConnectionConfiguration.h CondCore/DBCommon/interface/ConnectionConfiguration.h
   Description: interface to query and set the connection configuration parameters. The parameters will take effect only after the DBSession is opened.
*/
//
// Author:      Zhen Xie
//
namespace cond{
  class ConnectionConfiguration{
  public:
    ConnectionConfiguration();
    ~ConnectionConfiguration();
    
    /**
     * Enables the sharing of the same physical connection among more clients
     */
    void enableConnectionSharing();
    
    /**
     * Disables the sharing of the same physical connection
     * among more clients.
     */
    void disableConnectionSharing();
    
    /**
     * Returns true if connection sharing is enabled
     */
    bool isConnectionSharingEnabled() const;
    
    /**
     * Sets the period of connection retrials (time interval between two retrials).
     */
    void setConnectionRetrialPeriod( int timeInSeconds );
    
    /**
     * Returns the rate of connection retrials (time interval between two retrials).
     */
    int connectionRetrialPeriod() const;
    
    /**
     * Sets the time out for the connection retrials before the connection
     * service fails over to the next available replica or quits.
     */
    void setConnectionRetrialTimeOut( int timeOutInSeconds );

    /**
     * Returns the time out for the connection retrials before the connection
     * service fails over to the next available replica or quits.
     */
    int connectionRetrialTimeOut() const;

    /**
     * Sets the connection time out in seconds.
     */
    void setConnectionTimeOut( int timeOutInSeconds );

    /**
     * Retrieves the connection time out in seconds.
     */
    int connectionTimeOut();

    /**
     * Sets idle connection pool cleanup period. Has effect only if 
     * PoolAutomaticCleanUp is disabled
     */
    void setIdleConnectionCleanupPeriod( int timeInSeconds );
    
    /**
     * Returns period in second for manual cleanup idle connections 
     */
    int idleConnectionCleanupPeriod() const;
    
    /**
     * Enables the re-use of Update connections for Read-Only sessions
     */
    void enableReadOnlySessionOnUpdateConnections();

    /**
     * Disables the re-use of Update connections for Read-Only sessions
     */
    void disableReadOnlySessionOnUpdateConnections();
    
    /**
     * Returns true if the re-use of Update connections for Read-Only sessions is enabled 
     */
    bool isReadOnlySessionOnUpdateConnectionsEnabled();
    
    /**
     * Activate the parallel thread for idle pool cleaning up
     */
    void enablePoolAutomaticCleanUp();
    
    /**
     * Disable the parallel thread for idle pool cleaning up
     */
    void disablePoolAutomaticCleanUp();

    /**
     * Returns true if the parallel thread for idle pool cleaning up is enabled
     */
    bool isPoolAutomaticCleanUpEnabled() const;

    // set the monitoring level
    void setMonitorLevel(coral::monitor::Level level);
    // monitoring level
    coral::monitor::Level monitorLevel() const;
  private:
    bool m_enableConSharing;
    int m_connectionRetrialPeriod;
    int m_connectionRetrialTimeOut;
    int m_connectionTimeOut;
    int m_idleconnectionCleanupPeriod;
    bool m_enableCommonConnection;
    bool m_enablePoolAutomaticCleanUp; 
    coral::monitor::Level m_monitorLevel;
  };
}
#endif
