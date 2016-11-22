#ifndef IG_G4_DEMO_CONFIG_H
# define IG_G4_DEMO_CONFIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <classlib/sysapi/system.h>
# include "VisGeant4/VisG4Core/interface/config.h" // Get disgusting G4 hacks

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/** @def IG_G4_DEMO_API
  @brief A macro that controls how entities of this shared library are
         exported or imported on Windows platforms (the macro expands
         to nothing on all other platforms).  The definitions are
         exported if #IG_G4_DEMO_BUILD_DLL is defined, imported
         if #IG_G4_DEMO_BUILD_ARCHIVE is not defined, and left
	 alone if latter is defined (for an archive library build).  */

/** @def IG_G4_DEMO_BUILD_DLL
  @brief Indicates that the header is included during the build of
         a shared library of this package, and all entities marked
	 with #IG_G4_DEMO_API should be exported.  */

/** @def IG_G4_DEMO_BUILD_ARCHIVE
  @brief Indicates that this library is or was built as an archive
	 library, not as a shared library.  Lack of this indicates
	 that the header is included during the use of a shared
	 library of this package, and all entities marked with
	 #IG_G4_DEMO_API should be imported.  */

# ifndef IG_G4_DEMO_API
#  ifdef _WIN32
#   if defined IG_G4_DEMO_BUILD_DLL
#    define IG_G4_DEMO_API __declspec(dllexport)
#   elif ! defined IG_G4_DEMO_BUILD_ARCHIVE
#    define IG_G4_DEMO_API __declspec(dllimport)
#   endif
#  endif
# endif

# ifndef IG_G4_DEMO_API
#  define IG_G4_DEMO_API
# endif

#endif // IG_G4_DEMO_CONFIG_H
