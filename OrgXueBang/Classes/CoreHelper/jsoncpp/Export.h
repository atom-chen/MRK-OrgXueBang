#ifndef __EXPORT_COMMON__
#define __EXPORT_COMMON__

#if defined(_WIN32)
    #if defined(_EXPORT_DLL_)
    #define JSON_API     __declspec(dllexport)
    #elif defined(IGNORE_EXPORT)
    #define JSON_API
    #else 		/* use a DLL library */
    #define JSON_API     __declspec(dllimport)
    #endif
#else
    #if defined(_SHARED_)
    #define JSON_API     __attribute__((visibility("default")))
    #elif defined(IGNORE_EXPORT)
    #define JSON_API
    #else
    #define JSON_API
    #endif
#endif 

#endif	// end of __EXPORT_COMMON__
