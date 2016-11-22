#ifndef IGUANA_GL_BROWSERS_CONFIG_H
# define IGUANA_GL_BROWSERS_CONFIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/** @def IGUANA_GL_BROWSERS_API
  @brief A macro that controls how entities of this shared library are
         exported or imported on Windows platforms (the macro expands
         to nothing on all other platforms).  The definitions are
         exported if #IGUANA_GL_BROWSERS_BUILD_DLL is defined, imported
         if #IGUANA_GL_BROWSERS_BUILD_ARCHIVE is not defined, and left
	 alone if latter is defined (for an archive library build).  */

/** @def IGUANA_GL_BROWSERS_BUILD_DLL
  @brief Indicates that the header is included during the build of
         a shared library of this package, and all entities marked
	 with #IGUANA_GL_BROWSERS_API should be exported.  */

/** @def IGUANA_GL_BROWSERS_BUILD_ARCHIVE
  @brief Indicates that this library is or was built as an archive
	 library, not as a shared library.  Lack of this indicates
	 that the header is included during the use of a shared
	 library of this package, and all entities marked with
	 #IGUANA_GL_BROWSERS_API should be imported.  */

# ifndef IGUANA_GL_BROWSERS_API
#  ifdef _WIN32
#   if defined IGUANA_GL_BROWSERS_BUILD_DLL
#    define IGUANA_GL_BROWSERS_API __declspec(dllexport)
#   elif ! defined IGUANA_GL_BROWSERS_BUILD_ARCHIVE
#    define IGUANA_GL_BROWSERS_API __declspec(dllimport)
#   endif
#  endif
# endif

# ifndef IGUANA_GL_BROWSERS_API
#  define IGUANA_GL_BROWSERS_API
# endif

#endif // IGUANA_GL_BROWSERS_CONFIG_H
