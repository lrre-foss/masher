#pragma once

#include <vector>

#include "RobloxMesh.hpp"

namespace masher {

class RobloxMesh;
struct RobloxMeshVertex;
struct RobloxMeshFace;
struct RobloxMeshBone;

// Meshes are sometimes broken into subsets to get around some graphics cards
// not supporting >26 bones per mesh; thus, RobloxMeshSubsest just takes as many
// bones as possible (but <26) and assigns them to a mesh with corresponding
// verts/faces.
class RobloxMeshSubset
{
public:
    RobloxMesh* mesh();

    std::vector<RobloxMeshVertex>* vertices;
    std::vector<RobloxMeshFace>* faces;

    std::vector<RobloxMeshBone>* getBones();
    void setBones(std::vector<RobloxMeshBone>* bones);

private:
    std::vector<RobloxMeshBone>* bones;
};

} // namespace masher
