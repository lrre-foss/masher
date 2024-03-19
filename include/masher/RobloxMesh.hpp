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

namespace masher {

enum Error
{
                                            //
    ROBLOX_MESH_ERROR_NONE = 0,             // Default state of GetLastError()
                                            //
    ROBLOX_MESH_ERROR_UNSUPPORTED = 1,      // Unsupported version. v6.00 and v7.00 are intentionally left unsupported (see README).
                                            // v2.00-v5.00 are currently a work in progress and are not supported yet.
                                            //
    ROBLOX_MESH_ERROR_INVALID_VERSION = 2,  // Invalid version string
                                            //
    ROBLOX_MESH_ERROR_LOADER_EXCEPTION = 3, // General purpose loader exception
                                            //
    ROBLOX_MESH_ERROR_INVALID_SIZE = 4,     // Invalid size of mesh header
                                            //
    ROBLOX_MESH_ERROR_INVALID_LOD = 4,      // Invalid LOD type
                                            //
    ROBLOX_MESH_ERROR_INVALID_BONE = 5,     // Corrupt or invalid bone data (failed to parse)
                                            //
    ROBLOX_MESH_ERROR_INVALID_SUBSET = 6,   // Corrupt or invalid subset data (failed to parse)
                                            //
    ROBLOX_MESH_ERROR_INVALID_FACS = 7      // Corrupt or invalid FACS data (failed to parse)
                                            //
};

extern MASHER_LIB_API Error GetLastError();

enum RobloxMeshVersion
{
    ROBLOX_MESH_UNKNOWN = 0,
    ROBLOX_MESH_V1_00 = 1,
    ROBLOX_MESH_V1_01 = 2,

    // Unsupported/WIP below:
    ROBLOX_MESH_V2_00 = 3,
    ROBLOX_MESH_V3_00 = 4,
    ROBLOX_MESH_V3_01 = 5,
    ROBLOX_MESH_V4_00 = 6,
    ROBLOX_MESH_V4_01 = 7,
    ROBLOX_MESH_V5_00 = 8,
    ROBLOX_MESH_V6_00 = 9,
    ROBLOX_MESH_V7_00 = 10
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

struct RobloxMeshHeaderV5 : RobloxMeshHeaderV4
{
    uint32_t facsDataFormat;
    uint32_t facsDataSize;
};

// Introduced in Version 2.00
struct RobloxMeshVertex
{
    float   px, py, pz;      // Position
    float   nx, ny, nz;      // Normal Vector
    float   tu, tv;          // UV Texture Coordinates

    int8_t  tx, ty, tz, ts;  // Tangent Vector & Bi-Normal Direction
};

// Introduced sometime in Version 2.00
struct RobloxMeshVertexRGBA : RobloxMeshVertex
{
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

class MASHER_LIB_API RobloxMesh
{
public:
    std::vector<RobloxMeshVertex>* vertices;
    std::vector<RobloxMeshFace>*   faces;
    std::vector<uint32_t>*         lods;
    std::vector<RobloxMeshBone>*   bones;
    std::vector<uint8_t>*          boneNameTable;
    std::vector<RobloxMeshSubset>* subsets;
    std::vector<uint8_t>*          facs;

    RobloxMeshVersion version = ROBLOX_MESH_UNKNOWN;

    bool isLoaded() { return hasLoaded; }
    bool hasRgbaData() { return isRgbaDataPresent; }
    bool hasLodData() { return lods != nullptr; }
    bool hasBones() { return bones != nullptr || boneNameTable != nullptr || subsets != nullptr; }
    bool hasFacsData() { return facs != nullptr; }

    RobloxMesh(const char* data);
    RobloxMesh(const char* data, RobloxMeshVersion version);

    std::string write();

private:
    bool isRgbaDataPresent = false;

    bool hasLoaded;
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

} // namespace masher