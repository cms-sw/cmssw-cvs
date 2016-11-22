#ifndef IGUANA_MODELS_CONFIG_H
# define IGUANA_MODELS_CONFIG_H

//<<<<<< INCLUDES                                                       >>>>>>
//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/** @def IGUANA_MODELS_API
  @brief A macro that controls how entities of this shared library are
         exported or imported on Windows platforms (the macro expands
         to nothing on all other platforms).  The definitions are
         exported if #IGUANA_MODELS_BUILD_DLL is defined, imported
         if #IGUANA_MODELS_BUILD_ARCHIVE is not defined, and left
         alone if latter is defined (for an archive library build).  */

/** @def IGUANA_MODELS_BUILD_DLL
  @brief Indicates that the header is included during the build of
         a shared library of this package, and all entities marked
	 with #IGUANA_MODELS_API should be exported.  */

/** @def IGUANA_MODELS_BUILD_ARCHIVE
  @brief Indicates that this library is or was built as an archive
         library, not as a shared library.  Lack of this indicates
         that the header is included during the use of a shared
         library of this package, and all entities marked with
         #IGUANA_MODELS_API should be imported.  */

# ifndef IGUANA_MODELS_API
#  ifdef _WIN32
#    if defined IGUANA_MODELS_BUILD_DLL
#      define IGUANA_MODELS_API __declspec(dllexport)
#    elif ! defined IGUANA_MODELS_BUILD_ARCHIVE
#      define IGUANA_MODELS_API __declspec(dllimport)
#    endif
#  endif
# endif

# ifndef IGUANA_MODELS_API
#  define IGUANA_MODELS_API
# endif

#endif // IGUANA_MODELS_CONFIG_H
