/**
 * https://devforum.roblox.com/t/roblox-mesh-format/326114
 * 
 * V1 is simple position, normal, and UV Vector3s
 * V2 is the same represented in binary format
 * V3 adds support for LOD
 * V4 is a full overhaul adding support for skeletal data and mesh deformation
 * V5 is a major revision to V4 with additional FACS data at the end
 * 
 * structs and enums are listed here in chronological order. Please try to keep it that way for simplicity.
 */

#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cstdint>
#include <cassert>
#include <string>
#include <sstream>
#include <vector>

// Introduced in Version 2.00
struct Vertex
{
    float px, py, pz;      // Position
    float nx, ny, nz;      // Normal Vector
    float tu, tv;          // UV Texture Coordinates

    int8_t tx, ty, tz, ts; // Tangent Vector & Bi-Normal Direction
    uint8_t r, g, b, a;    // RGBA Color Tinting
};

// Introduced in Version 2.00
struct Face
{
    uint32_t a; // 1st Vertex Index
    uint32_t b; // 2nd Vertex Index
    uint32_t c; // 3rd Vertex Index
};

// Introduced in Version 4.00
enum MeshLodType
{
    NONE = 0,
    UNKNOWN = 1,
    ROBLOX = 2,
    MESHOPTIMIZER = 3,
};

// Introduced in Version 4.00
struct Envelope
{
    uint8_t bones[4];
    uint8_t weights[4];
};

// Introduced in Version 4.00
struct Bone
{
    uint32_t boneNameIndex;

    uint16_t parentIndex;
    uint16_t lodParentIndex;

    float culling;

    float r00, r01, r02;
    float r10, r11, r12;
    float r20, r21, r22;

    float x, y, z;
};

// Introduced in Version 4.00
struct MeshSubset
{
    uint32_t facesBegin;
    uint32_t facesLength;

    uint32_t vertsBegin;
    uint32_t vertsLength;

    uint32_t numBoneIndices;
    uint16_t boneIndices[26];
};

// Introduced in Version 5.00
struct FacsData
{
    uint32_t sizeof_faceBoneNamesBuffer;
    uint32_t sizeof_faceControlNamesBuffer;
    uint64_t sizeof_quantizedTransformsBuffer;
    
    uint32_t sizeof_twoPoseCorrectivesBuffer;
    uint32_t sizeof_threePoseCorrectivesBuffer;

    std::vector<uint8_t> faceBoneNamesBuffer;        // sizeof(faceBoneNamesBuffer) == sizeof_faceBoneNamesBuffer
    std::vector<uint8_t> faceControlNamesBuffer;     // sizeof(faceControlNamesBuffer) == sizeof_faceControlNamesBuffer
    std::vector<uint8_t> quantizedTransformsBuffer;  // sizeof(quantizedTransformsBuffer) == sizeof_quantizedTransformsBuffer

    std::vector<uint8_t> twoPoseCorrectivesBuffer;   // sizeof(twoPoseCorrectivesBuffer) == sizeof_twoPoseCorrectivesBuffer
    std::vector<uint8_t> threePoseCorrectivesBuffer; // sizeof(threePoseCorrectivesBuffer) == sizeof_threePoseCorrectivesBuffer
};

// Introduced in Version 5.00
struct QuantizedMatrix
{
    uint16_t version;
    uint32_t rows, cols;

    // if version == 1 ...
    float** v1_matrix;

    // elseif version == 2 ...
    float v2_min, v2_max;    
    uint16_t** v2_matrix;
};

// Introduced in Version 5.00
struct QuantizedTransforms
{
    QuantizedMatrix px;
    QuantizedMatrix py;
    QuantizedMatrix pz;

    QuantizedMatrix rx;
    QuantizedMatrix ry;
    QuantizedMatrix rz;
};

// Introduced in Version 5.00
struct TwoPoseCorrective
{
    uint16_t controlIndex0;
    uint16_t controlIndex1;
};

// Introduced in Version 5.00
struct ThreePoseCorrective
{
    uint16_t controlIndex0;
    uint16_t controlIndex1;
    uint16_t controlIndex2;
};