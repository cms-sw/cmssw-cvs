#ifndef VIS_G4_CORE_CONFIG_H
# define VIS_G4_CORE_CONFIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/** @def VIS_G4_CORE_API
  @brief A macro that controls how entities of this shared library are
         exported or imported on Windows platforms (the macro expands
         to nothing on all other platforms).  The definitions are
         exported if #VIS_G4_CORE_BUILD_DLL is defined, imported
         if #VIS_G4_CORE_BUILD_ARCHIVE is not defined, and left
	 alone if latter is defined (for an archive library build).  */

/** @def VIS_G4_CORE_BUILD_DLL
  @brief Indicates that the header is included during the build of
         a shared library of this package, and all entities marked
	 with #VIS_G4_CORE_API should be exported.  */

/** @def VIS_G4_CORE_BUILD_ARCHIVE
  @brief Indicates that this library is or was built as an archive
	 library, not as a shared library.  Lack of this indicates
	 that the header is included during the use of a shared
	 library of this package, and all entities marked with
	 #VIS_G4_CORE_API should be imported.  */

# ifndef VIS_G4_CORE_API
#  ifdef _WIN32
#   if defined VIS_G4_CORE_BUILD_DLL
#    define VIS_G4_CORE_API __declspec(dllexport)
#   elif ! defined VIS_G4_CORE_BUILD_ARCHIVE
#    define VIS_G4_CORE_API __declspec(dllimport)
#   endif
#  endif
# endif

# ifndef VIS_G4_CORE_API
#  define VIS_G4_CORE_API
# endif

// FIXME: Disgustingly ugly hack for Geant4 which is standard-broken
#if defined __SUNPRO_CC
namespace std {}


// FIXME: Sigh.  Now work around the problems created by the above...
# define exception math_exception
# include <math.h>
# undef exception
#endif

#endif // VIS_G4_CORE_CONFIG_H
