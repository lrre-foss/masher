#pragma once

#if defined(_WIN32)
    #ifdef MASHER_LIB_EXPORT
        #define MASHER_LIB_API __declspec(dllexport)
    #else
        #define MASHER_LIB_API __declspec(dllimport)
    #endif
#elif defined(__GNUC__) && __GNUC__ >= 4
    #define MASHER_LIB_API __attribute__ ((visibility("default")))
#else
    #define MASHER_LIB_API
#endif

namespace masher {

enum Error
{
    MASHER_OK = 0,
    MASHER_ERROR_INVALID_VERSION,         /* 1 - Invalid/unrecognized version string */
    MASHER_ERROR_STREAM_FAILURE,          /* 2 - General stream failure (usually due to malformed data) */
    MASHER_ERROR_V1_MALFORMED_FACE_COUNT, /* 3 - Malformed face count while reading v1.xx mesh */
    MASHER_ERROR_V1_MALFORMED_VECTOR3,    /* 4 - Malformed Vector3 data while reading v1.xx mesh */
    MASHER_ERROR_ILLEGAL_OPERATION,       /* 5 - Illegal operation (e.g. trying to write an unloaded mesh) */
    MASHER_ERROR_UNSUPPORTED,             /* 6 - Mesh format is currently unsupported at this time. v6.xx and v7.xx are left intentionally unsupported -- see README for more info. */
    MASHER_LAST
};

extern MASHER_LIB_API Error GetLastError(bool wipeLastError = true);
extern MASHER_LIB_API void SetLastError(Error error);

} // namespace masher
