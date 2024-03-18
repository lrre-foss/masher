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

    uint32_t facsDataFormat;
    uint32_t facsDataSize;
};

const char* convertToV5(const aiScene* scene);
const aiScene* loadFromV5(const char* mesh);