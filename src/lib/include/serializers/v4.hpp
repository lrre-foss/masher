#pragma once

#include "serializer.hpp"

struct MeshHeader
{
    uint16_t sizeof_MeshHeader;
    uint16_t lodType;

    uint32_t numVerts;
    uint32_t numFaces;

    uint16_t numLODs;
    uint16_t numBones;

    uint32_t sizeof_boneNamesBuffer;
    uint16_t numSubsets;

    uint8_t numHighQualityLODs;
    uint8_t unused;
};

const char* convertToV4(const aiScene* scene);
const aiScene* loadFromV4(const char* mesh);