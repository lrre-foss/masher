#ifndef MASHER_LIB_H
#define MASHER_LIB_H

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

#ifdef __cplusplus
namespace masher {
#endif

#ifdef __cplusplus
enum Error
#else
typedef enum
#endif;
{
    MASHER_OK = 0,
    MASHER_ERROR_INVALID_VERSION,         /* 1 - Invalid/unrecognized version string */
    MASHER_ERROR_STREAM_FAILURE,          /* 2 - General stream failure (usually due to malformed data) */
    MASHER_ERROR_V1_MALFORMED_FACE_COUNT, /* 3 - Malformed face count while reading v1.xx mesh */
    MASHER_ERROR_V1_MALFORMED_VECTOR3,    /* 4 - Malformed Vector3 data while reading v1.xx mesh */
    MASHER_ERROR_ILLEGAL_OPERATION,       /* 5 - Illegal operation (e.g. trying to write an unloaded mesh) */
    MASHER_ERROR_UNSUPPORTED,             /* 6 - Mesh format is currently unsupported at this time. v6.xx and v7.xx are left intentionally unsupported -- see README for more info. */
    MASHER_LAST
#ifndef __cplusplus
} MASHER_ERROR;
#else
};
#endif

#ifdef __cplusplus
Error lastError = MASHER_OK;
MASHER_LIB_API extern Error GetLoaderError(bool wipeLastError = true)
#else
MASHER_ERROR lastError = MASHER_OK;
MASHER_LIB_API extern MASHER_ERROR masherGetLoaderError(bool wipeLastError)
#endif
{
#ifdef __cplusplus
    Error error = lastError;
#else
    MASHER_ERROR error = lastError;
#endif

    if (wipeLastError)
        lastError = MASHER_OK;

    return error;
}

#ifdef __cplusplus
} // namespace masher
#endif

#endif