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

std::vector<RobloxMeshBone>* RobloxMeshSubset::getBones()
{
    return bones;
}

void RobloxMeshSubset::setBones(std::vector<RobloxMeshBone>* bones)
{
    if (bones->size() >= 26)
        throw std::invalid_argument("RobloxMeshSubset::setBones: bones.size() must be less than 26");

    this->bones = bones;
}

} // namespace masher
