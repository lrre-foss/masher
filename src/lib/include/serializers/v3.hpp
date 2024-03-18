#pragma once

#include "serializer.hpp"

struct MeshHeader
{
    uint16_t sizeof_MeshHeader;
    uint8_t sizeof_Vertex;
    uint8_t sizeof_Face;
    uint16_t sizeof_LOD;

    uint16_t numLODs;
    uint32_t numVerts;
    uint32_t numFaces;
};

const char* convertToV3(const aiScene* scene);
const aiScene* loadFromV3(const char* mesh);