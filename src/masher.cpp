#include "masher/masher.h"

namespace masher {

static Error lastError = MASHER_OK;

Error GetLoaderError(bool wipeLastError)
{
    Error error = lastError;

    if (wipeLastError)
        lastError = MASHER_OK;

    return error;
}

void SetLoaderError(Error error)
{
    lastError = error;
}

} // namespace masher