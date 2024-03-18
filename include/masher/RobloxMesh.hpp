#pragma once

#include <cstdint>
#include <cassert>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#if defined(_WIN32)
    #ifdef MASHER_LIB_EXPORT
        #define MASHER_LIB_API __declspec(dllexport)
    #else
        #define MASHER_LIB_API __declspec(dllimport)
    #endif
#elif defined(__GNUC__) && __GNUC__ >= 4
    #define MASHER_LIB_API __attribute__ ((visibility("default")))
#else
    #define MASHER_LIB_API
#endif

enum RobloxMeshVersion
{
    ROBLOX_MESH_V1_00 = 1,
    ROBLOX_MESH_V1_01 = 2,
    ROBLOX_MESH_V2_00 = 3,
    ROBLOX_MESH_V3_00 = 4,
    ROBLOX_MESH_V4_00 = 5,
    ROBLOX_MESH_V5_00 = 6,
};

struct RobloxMeshHeaderV2
{
    uint16_t sizeof_MeshHeader;
    uint8_t  sizeof_Vertex;
    uint8_t  sizeof_Face;

    uint32_t numVerts;
    uint32_t numFaces;
};

struct RobloxMeshHeaderV3
{
    uint16_t sizeof_MeshHeader;
    uint8_t  sizeof_Vertex;
    uint8_t  sizeof_Face;
    uint16_t sizeof_LOD;

    uint16_t numLODs;
    uint32_t numVerts;
    uint32_t numFaces;
};

struct RobloxMeshHeaderV4
{
    uint16_t sizeof_MeshHeader;
    uint16_t lodType;

    uint32_t numVerts;
    uint32_t numFaces;

    uint16_t numLODs;
    uint16_t numBones;

    uint32_t sizeof_boneNamesBuffer;
    uint16_t numSubsets;

    uint8_t  numHighQualityLODs;
    uint8_t  unused;
};

struct RobloxMeshHeaderV5
{
    uint16_t sizeof_MeshHeader;
    uint16_t lodType;

    uint32_t numVerts;
    uint32_t numFaces;

    uint16_t numLODs;
    uint16_t numBones;

    uint32_t sizeof_boneNamesBuffer;
    uint16_t numSubsets;

    uint8_t  numHighQualityLODs;
    uint8_t  unused;

    uint32_t facsDataFormat;
    uint32_t facsDataSize;
};

// Introduced in Version 2.00
struct RobloxMeshVertex
{
    float   px, py, pz;      // Position
    float   nx, ny, nz;      // Normal Vector
    float   tu, tv, tw;      // UV Texture Coordinates

    int8_t  tx, ty, tz, ts;  // Tangent Vector & Bi-Normal Direction
    uint8_t r, g, b, a;      // RGBA Color Tinting
};

// Introduced in Version 2.00
struct RobloxMeshFace
{
    uint32_t a; // 1st Vertex Index
    uint32_t b; // 2nd Vertex Index
    uint32_t c; // 3rd Vertex Index
};

// Introduced in Version 4.00
enum RobloxMeshLodType
{
    NONE          = 0,
    UNKNOWN       = 1,
    ROBLOX        = 2,
    MESHOPTIMIZER = 3,
};

// Introduced in Version 4.00
struct RobloxMeshEnvelope
{
    uint8_t bones[4];
    uint8_t weights[4];
};

// Introduced in Version 4.00
struct RobloxMeshBone
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
struct RobloxMeshSubset
{
    uint32_t facesBegin;
    uint32_t facesLength;

    uint32_t vertsBegin;
    uint32_t vertsLength;

    uint32_t numBoneIndices;
    uint16_t boneIndices[26];
};

// Introduced in Version 5.00
struct RobloxMeshFacsData
{
    uint32_t sizeof_faceBoneNamesBuffer;
    uint32_t sizeof_faceControlNamesBuffer;
    uint64_t sizeof_quantizedTransformsBuffer;
    
    uint32_t sizeof_twoPoseCorrectivesBuffer;
    uint32_t sizeof_threePoseCorrectivesBuffer;

    uint8_t* faceBoneNamesBuffer;        // sizeof(faceBoneNamesBuffer) == sizeof_faceBoneNamesBuffer
    uint8_t* faceControlNamesBuffer;     // sizeof(faceControlNamesBuffer) == sizeof_faceControlNamesBuffer
    uint8_t* quantizedTransformsBuffer;  // sizeof(quantizedTransformsBuffer) == sizeof_quantizedTransformsBuffer

    uint8_t* twoPoseCorrectivesBuffer;   // sizeof(twoPoseCorrectivesBuffer) == sizeof_twoPoseCorrectivesBuffer
    uint8_t* threePoseCorrectivesBuffer; // sizeof(threePoseCorrectivesBuffer) == sizeof_threePoseCorrectivesBuffer
};

// Introduced in Version 5.00
struct RobloxMeshQuantizedMatrix
{
    uint16_t version;
    uint32_t rows, cols;

    // version == 1
    float**    v1_matrix;

    // version == 2
    float      v2_min, v2_max;
    uint16_t** v2_matrix;
};

// Introduced in Version 5.00
struct RobloxMeshQuantizedTransforms
{
    RobloxMeshQuantizedMatrix px;
    RobloxMeshQuantizedMatrix py;
    RobloxMeshQuantizedMatrix pz;

    RobloxMeshQuantizedMatrix rx;
    RobloxMeshQuantizedMatrix ry;
    RobloxMeshQuantizedMatrix rz;
};

// Introduced in Version 5.00
struct RobloxMeshTwoPoseCorrective
{
    uint16_t controlIndex0;
    uint16_t controlIndex1;
};

// Introduced in Version 5.00
struct RobloxMeshThreePoseCorrective
{
    uint16_t controlIndex0;
    uint16_t controlIndex1;
    uint16_t controlIndex2;
};

struct MASHER_LIB_API RobloxMesh {
    std::vector<RobloxMeshVertex>* vertices;
    std::vector<RobloxMeshFace>*   faces;
    std::vector<uint32_t>*         lods;
    std::vector<RobloxMeshBone>*   bones;
    std::vector<uint8_t>*          nameTable;
    std::vector<RobloxMeshSubset>* subsets;
    std::vector<uint8_t>*          facsDataBuffer;

    RobloxMeshVersion version;

    bool isLoaded() { return loaded; }

    RobloxMesh(const char* data);
    RobloxMesh(const char* data, RobloxMeshVersion version);

    std::string write();

private:
    bool loaded;
    bool load(const char* data, bool detect = false);

    // v1.00, v1.01
    bool loadV1(std::istringstream& stream);
    void writeV1(std::ostringstream& stream);

    // v2.00
    bool loadV2(std::istringstream& stream);
    void writeV2(std::ostringstream& stream);

    // v3.00
    bool loadV3(std::istringstream& stream);
    void writeV3(std::ostringstream& stream);

    // v4.00, v5.00
    bool loadV4(std::istringstream& stream);
    void writeV4(std::ostringstream& stream);
};