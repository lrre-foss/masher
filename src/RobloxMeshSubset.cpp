#include "masher/RobloxMeshSubset.hpp"

namespace masher {

RobloxMesh* RobloxMeshSubset::mesh()
{
    RobloxMesh* mesh = new RobloxMesh(this);

    if (!mesh->hasLoaded())
    {
        delete mesh;
        return nullptr;
    }

    return mesh;
}

std::deque<RobloxMeshBone>* RobloxMeshSubset::getBones()
{
    return bones;
}

void RobloxMeshSubset::setBones(std::deque<RobloxMeshBone>* bones)
{
    if (bones->size() >= 26)
    {
        masher::SetLastError(MASHER_ERROR_ILLEGAL_OPERATION);
        throw std::invalid_argument("RobloxMeshSubset::setBones: bones.size() must be less than 26");
    }

    this->bones = bones;
}

} // namespace masher
