#pragma once

#include "serializer.hpp"

struct MeshHeader
{
    unsigned short sizeof_MeshHeader;
    unsigned char sizeof_Vertex;
    unsigned char sizeof_Face;

    unsigned int numVerts;
    unsigned int numFaces;
};

const char* convertToV2(const aiScene* scene);
const aiScene* loadFromV2(const char* mesh);