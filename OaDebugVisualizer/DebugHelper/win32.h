#if !defined(WIN32_H_)
#define WIN32_H_

// vc14's "ucrt" (universal c runtime) has snprintf
#if _MSC_VER < 1900
#  ifndef snprintf
#    define snprintf _snprintf
#  endif
#endif

#ifndef strncasecmp
#  define strncasecmp strnicmp
#endif

#ifndef strcasecmp
#  define strcasecmp stricmp
#endif

#ifndef atoll
#  define atoll _atoi64
#endif

#ifndef strtoll
#  define strtoll _strtoi64
#endif

#ifndef strtoull
#  define strtoull _strtoui64
#endif

#pragma warning(disable : 4660 4661 4273)

// warning C4786: '<template>' : identifier was truncated to '255' characters in the debug information
#pragma warning(disable : 4786)

// warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable : 4800)

// enable warnings to enjoy more conformity with linux gcc
// warning C4062: enumerator '<case>' in switch of enum '<enum>' is not handled
#pragma warning(default : 4062)

#define _POSIX_PATH_MAX MAX_PATH
typedef signed int ssize_t;


/*
 * damit winsock.h NICHT automatisch eingebunden wird!
 * Es wird naemlich explizit winsock2.h verwendet
*/
#define _WINSOCKAPI_

#ifndef FULL_WIN32
# define NOGDI
# define NOUSER
# define NOMSG

# ifndef NODEF_OLE2_H_
#   define _OLE2_H_
# endif

# ifndef NODEF__RPC_H__
#   define __RPC_H__
# endif
#endif  // FULL_WIN32

// #define FOPEN_DEBUG
#if defined(FOPEN_DEBUG)
  #define fopen(a,b)	        FOPEN(a,b,__FILE__,__LINE__)  // befindet sich in FileSys.cxx  

  #ifdef __cplusplus
    extern "C" {
  #endif
  FILE *FOPEN(const char *filename, const char *mode,   const char *source, long lineNo);
  #ifdef __cplusplus
    }
  #endif
#endif

typedef char *caddr_t;

#define USE_STRUCT_FD_SET // TI 13943
typedef int socklen_t;

                               // weil statt extern defines EXTERN_BASICS .. verwendet werden,
                               // die unter UNIX wegdefined werden kann dieser Warning 
                               // ausgeschaltet werden.
#pragma warning(disable:4231)  // warning C4231: nonstandard extension used
                               //              : 'extern' before template explicit instantiation

#ifdef MAKEDLL_BCM
  #define DLLEXP_BCM  __declspec(dllexport)
#else
  #define DLLEXP_BCM  __declspec(dllimport)
#endif

#ifdef MAKE_DLL_BASICS
  #define DLLEXP_BASICS  __declspec(dllexport)
  #define EXTERN_BASICS
#else
  #define DLLEXP_BASICS  __declspec(dllimport)
  #define EXTERN_BASICS  extern
#endif

#ifdef MAKE_DLL_CONFIGS
  #define DLLEXP_CONFIGS  __declspec(dllexport)
  #define EXTERN_CONFIGS
#else
  #define DLLEXP_CONFIGS  __declspec(dllimport)
  #define EXTERN_CONFIGS  extern
#endif

#ifdef MAKE_DLL_DATAPOINT
  #define DLLEXP_DATAPOINT  __declspec(dllexport)
  #define EXTERN_DATAPOINT
#else
  #define DLLEXP_DATAPOINT  __declspec(dllimport)
  #define EXTERN_DATAPOINT  extern
#endif

#ifdef MAKE_DLL_MESSAGES
  #define DLLEXP_MESSAGES  __declspec(dllexport)
  #define EXTERN_MESSAGES
#else
  #define DLLEXP_MESSAGES  __declspec(dllimport)
  #define EXTERN_MESSAGES  extern
#endif

#ifdef MAKE_DLL_MANAGER
  #define DLLEXP_MANAGER  __declspec(dllexport)
  #define EXTERN_MANAGER
#else
  #define DLLEXP_MANAGER  __declspec(dllimport)
  #define EXTERN_MANAGER  extern
#endif

#ifdef MAKE_DLL_CTRL
  #define DLLEXP_CTRL  __declspec(dllexport)
  #define EXTERN_CTRL
#else
  #define DLLEXP_CTRL  __declspec(dllimport)
  #define EXTERN_CTRL  extern
#endif

#endif

// 21.07.15 esperrer IM117695: add new main for UTF8 command line
#define UTF8_MAIN                  \
  UTF8main(int argc, char **argv); \
                                                \
  int main(int argc, char *argv[])              \
  {                                             \
    int nArgs;                                  \
    LPWSTR *szArglist;                          \
                                                \
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);      \
    char **utf8Argv = new char*[nArgs];         \
    for (int n = 0; n < nArgs; n++)             \
    {                                           \
      CharString dest;                          \
      UTF8Converter::convertFromWchar(szArglist[n], dest);          \
      utf8Argv[n] = dest.dup();                 \
    }                                           \
                                                \
    UTF8main(nArgs, utf8Argv);                  \
  }                                             \
                                                \
int UTF8main
