// $Id: writeMaskToDB.cpp,v 1.23 2010/01/25 21:34:14 dellaric Exp $

/*!
  \file writeMaskFromDB.cpp
  \brief It reads errors masks from a file and updates database
  \author B. Gobbo
  \version $Revision: 1.23 $
  \date $Date: 2010/01/25 21:34:14 $
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <getopt.h>
#include <string.h>

#ifdef WITH_ECAL_COND_DB
#include "OnlineDB/EcalCondDB/interface/EcalCondDBInterface.h"
#include "OnlineDB/EcalCondDB/interface/RunIOV.h"
#endif

#include "DQM/EcalCommon/interface/EcalErrorMask.h"

void usage( char* cp ) {
  std::cout <<
"\n\
usage: " << cp << " [OPTIONS] file\n\n\
     -h, --help                   : print this help message \n\
     -s, --sid=SID                : data base sid \n\
     -u, --user-name=DB USER      : data base user name \n\
     -p, --password=DB PASSWORD   : data base password \n\
     -H, --host-name=HOST NAME    : data base server host name \n\
     -P, --port=PORT              : data base server port number \n\
     -l, --location=LOCATION      : location H4, 867-1, ...\n\
     -r, --run-number=RUN NUMBER  : run number \n\
     -t, --run-type=RUN TYPE      : run type (only needed for --self-iov)\n\
     -i, --self-iov               : self-made IOV \n\
     -d, --debug                  : debug \n\
     -v, --verify-syntax          : just verify file text syntax \n\n";

#ifndef WITH_ECAL_COND_DB
  std::cout << "WARNING: DB access is DISABLED\n\n";
#endif

}

#ifdef WITH_ECAL_COND_DB
void printTag( const RunTag* tag) {
  std::cout << "------------ RunTag:" << std::endl;
  std::cout << "GeneralTag:         " << tag->getGeneralTag() << std::endl;
  std::cout << "Location:           " << tag->getLocationDef().getLocation() << std::endl;
  std::cout << "Run Type:           " << tag->getRunTypeDef().getRunType() << std::endl;
  std::cout << "--------------------" << std::endl;
}

void printIOV( const RunIOV* iov) {
  std::cout << std::endl;
  RunTag tag = iov->getRunTag();
  printTag(&tag);
  std::cout << "------------ RunIOV:" << std::endl;
  std::cout << "Run Number:         " << iov->getRunNumber() << std::endl;
  std::cout << "Run Start:          " << iov->getRunStart().str() << std::endl;
  std::cout << "Run End:            " << iov->getRunEnd().str() << std::endl;
  std::cout << "--------------------" << std::endl;
  std::cout << std::endl;
}
#endif

int main( int argc, char **argv ) {

  char* cp;

  // --------------------------------------------------------------------------
  // If you like, you can set variables to some default here

  std::string sid      = "";
  std::string user     = "";
  std::string passwd   = "";
  std::string fileName = "";
  std::string hostName = "";
  int port             = 1521;
  std::string location = "";
  int runNb            = 0;
  std::string runType  = "";
  bool smi             = false;
  bool debug           = false;
  bool verifySyntax    = false;
  bool errors          = false;

  // ------------------

  if(( cp = (char*) strrchr( argv[0], '/' )) != NULL ) {
    ++cp;
  }
  else {
    cp = argv[0];
  }

  // Arguments and Options
  if( argc > 1 ) {
    int c;
    while(1) {
      int option_index;
      static struct option long_options[] = {
        { "help",          0, 0, 'h' },
        { "sid",           1, 0, 's' },
        { "user-name",     1, 0, 'u' },
        { "password",      1, 0, 'p' },
        { "host-name",     1, 0, 'H' },
        { "port",          1, 0, 'P' },
        { "location",      1, 0, 'l' },
        { "run-number",    1, 0, 'r' },
        { "run-type",      1, 0, 't' },
        { "self-iov",      0, 0, 'i' },
        { "debug",         0, 0, 'd' },
        { "verify-syntax", 0, 0, 'v' },
        { 0, 0, 0, 0 }
      };

      c = getopt_long( argc, argv, "hs:u:p:H:P:l:r:t:idv", long_options, &option_index );
      if( c == -1 ) break;

      switch( c ) {
      case 'h':
        usage(cp);
        return(0);
        break;
      case 's':
        sid = optarg;
        break;
      case 'u':
        user = optarg;
        break;
      case 'p':
        passwd = optarg;
        break;
      case 'H':
        hostName = optarg;
        break;
      case 'P':
        port = atoi(optarg);
        break;
      case 'l':
        location = optarg;
        break;
      case 'r':
        runNb = atoi(optarg);
        break;
      case 't':
        runType = optarg;
        break;
      case 'i':
        smi = true;
        break;
      case 'd':
        debug = true;
        break;
      case 'v':
        verifySyntax = true;
      default:
        break;
      }
    }
  }
  if( optind+1 == argc ) {
    fileName = argv[optind++];
  }
  else {
    usage(cp);
    return -1;
  }

  std::cout << std::endl;

  if( !verifySyntax ) {
    if( sid.size() == 0 ) {
      std::cout << "sid     : ";
      std::cin >> sid;
    }
    if( user.size() == 0 ) {
      std::cout << "username: ";
      std::cin >> user;
    }
    if( passwd.size() == 0 ) {
      std::cout << "password: ";
      std::cin >> passwd;
    }
    if( runNb == 0 ) {
      std::cout << "run Nb  : ";
      std::cin >> runNb;
    }
    if( location.size() == 0 ) {
      std::cout << "location: ";
      std::cin >> location;
    }
    if( smi && runType.size() == 0 ) {
      std::cout << "run type: ";
      std::cin >> runType;
    }

    std::cout << std::endl;
    std::cout << "sid       : " << sid << std::endl
              << "user      : " << user << std::endl
              << "password  : " << passwd << std::endl;
    if ( hostName != "" ) {
      std::cout << "hostname  : " << hostName << std::endl
                << "port      : " << port << std::endl;
    }
  }

  std::cout << "file      : " << fileName << std::endl;
  std::cout << std::endl;

  // OK, from here there's all what's needed...

  errors = false;

  try {
    if( verifySyntax ) {
      EcalErrorMask::readFile( fileName, debug, true );
    }
    else {
      EcalErrorMask::readFile( fileName, debug );
    }
  } catch( std::runtime_error &e ) {
    std::cerr << e.what() << std::endl;
    if( !verifySyntax ) {
      return -1;
    }
    else {
      errors = true;
    }
  }

#ifdef WITH_ECAL_COND_DB
  if( !verifySyntax ) {

    EcalCondDBInterface* eConn;

    eConn = 0;

    try {
      std::cout << "Opening DB connection with TNS_ADMIN ..." << std::endl;
      eConn = new EcalCondDBInterface( sid, user, passwd );
    } catch( std::runtime_error &e ) {
      std::cerr << e.what() << std::endl;
      if ( hostName != "" ) {
        try {
          std::cout << "Opening DB connection without TNS_ADMIN ..." << std::endl;
          eConn = new EcalCondDBInterface( hostName, sid, user, passwd, port );
        } catch( std::runtime_error &e ) {
          std::cerr << e.what() << std::endl;
          return -1;
        }
      }
    }

    if( smi ) {

      LocationDef locdef;
      locdef.setLocation( location );

      RunTypeDef rundef;
      rundef.setRunType( runType );

      RunTag runtag;
      runtag.setLocationDef( locdef );
      runtag.setRunTypeDef( rundef );
      runtag.setGeneralTag( runType );

      Tm startTm;
      startTm.setToCurrentGMTime();
      startTm.setToMicrosTime( startTm.microsTime() );

      RunIOV new_runiov;
      new_runiov.setRunNumber( runNb );
      new_runiov.setRunStart( startTm );
      new_runiov.setRunTag( runtag );

      printIOV(&new_runiov);

      std::string yesno;
      std::cout << "Inserting self-made IOV. Are you sure? [y/N] ";
      std::cin >> yesno;

      std::cout << std::endl;

      if( strcmp(yesno.c_str(), "y") == 0 || strcmp(yesno.c_str(), "Y") == 0 ) {

        if ( eConn ) {
          try {
            eConn->insertRunIOV(&new_runiov);
            std::cout << "done" << std::endl;
          } catch( std::runtime_error &e ) {
            std::cerr << e.what() << std::endl;
            return -1;
          }
        }

      }
    }

    RunIOV runiov;

    if ( eConn ) {
      try {
        runiov = eConn->fetchRunIOV( location, runNb );
      } catch( std::runtime_error &e ) {
        std::cerr << e.what() << std::endl;
        return -1;
      }
    }

    printIOV(&runiov);

    std::string yesno;
    std::cout << "Inserting masking table. Are you sure? [y/N] ";
    std::cin >> yesno;

    std::cout << std::endl;

    if( strcmp(yesno.c_str(), "y") == 0 || strcmp(yesno.c_str(), "Y") == 0 ) {

      if ( eConn ) {
        try {
          EcalErrorMask::writeDB( eConn, &runiov );
          std::cout << "done" << std::endl;
        } catch( std::runtime_error &e ) {
          std::cerr << e.what() << std::endl;
          return -1;
        }
      }

    }

    if ( eConn ) delete eConn;

  }
#else
  std::cout << std::endl;
  std::cout << "WARNING: DB access is DISABLED" << std::endl;
  std::cout << std::endl;
#endif

  return 0;

}

