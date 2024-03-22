#include "masher/masher.h"

namespace masher {

static Error lastError = MASHER_OK;

Error GetLastError(bool wipeLastError)
{
    Error error = lastError;

    if (wipeLastError)
        lastError = MASHER_OK;

    return error;
}

void SetLastError(Error error)
{
    lastError = error;
}

} // namespace masher