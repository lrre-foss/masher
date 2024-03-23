/**
 * https://devforum.roblox.com/t/roblox-mesh-format/326114
 * 
 * v1.00 is simple position, normal, and UV Vector3s
 * v1.01 fixes some bugs in V1.00
 * v2.00 is the same except represented in binary format
 * v3.00 adds support for LOD
 * v4.00 is a full overhaul adding support for skeletal data and mesh deformation
 * v5.00 is a slight revision to V4 with additional facial animation data at the end
 */

#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <deque>
#include "masher.h"
#include "RobloxMeshSubset.hpp"

namespace masher {

class RobloxMeshSubset;
struct RobloxMeshVertex;
struct RobloxMeshFace;
struct RobloxMeshBone;
struct RobloxMeshQuantizedMatrix;
struct RobloxMeshTwoPoseCorrective;
struct RobloxMeshThreePoseCorrective;

enum RobloxMeshVersion
{
    // Uses the v1.xx parser
    ROBLOX_MESH_V1_00 = 0,
    ROBLOX_MESH_V1_01,

    // Uses the v2.xx parser
    ROBLOX_MESH_V2_00,

    // Uses the v3.xx parser
    ROBLOX_MESH_V3_00,
    ROBLOX_MESH_V3_01,

    // Uses the v4.xx parser
    ROBLOX_MESH_V4_00,
    ROBLOX_MESH_V4_01,
    ROBLOX_MESH_V5_00,

    // Unsupported, but still recognized
    ROBLOX_MESH_V6_00,
    ROBLOX_MESH_V7_00,

    ROBLOX_MESH_UNKNOWN
};

// Used to determine which optimizer was used on the LOD meshes
enum RobloxMeshOptimizer
{
    OPTIMIZER_NONE = 0,
    OPTIMZIER_UNKNOWN, // Unused, but masher uses this if RobloxMesh->optimizer was never set
    OPTIMIZER_ROBLOX,
    OPTIMIZER_ZEUX
};

class MASHER_LIB_API RobloxMesh
{
public:
    RobloxMeshVersion getVersion() { return version; }
    bool hasLoaded() { return isLoaded; }
    bool isLodMesh() { return optimizer != OPTIMIZER_NONE; }

    RobloxMeshOptimizer optimizer = OPTIMIZER_NONE;

    std::deque<RobloxMeshVertex>* vertices;
    std::deque<RobloxMeshFace>* faces;
    std::deque<RobloxMeshBone>* bones;
    std::deque<RobloxMeshSubset>* subsets;
    std::deque<RobloxMesh>* lodMeshes;

    // FACS data
    std::deque<RobloxMeshQuantizedMatrix>* quantizedMatrices;
    std::deque<RobloxMeshTwoPoseCorrective>* twoPoseCorrectives;
    std::deque<RobloxMeshThreePoseCorrective>* threePoseCorrectives;

    bool holdsTexWData()   { return version > ROBLOX_MESH_V1_01 ? false : isTexWDataPresent; }
    bool holdsRgbaData()   { return version > ROBLOX_MESH_V2_00 ? true  : isRgbaDataPresent; }
    bool holdsLodData()    { return isLodDataPresent;    }
    bool holdsBoneData()   { return isBoneDataPresent;   }
    bool holdsSubsetData() { return isSubsetDataPresent; }
    bool holdsFacsData()   { return isFacsDataPresent;   }

    RobloxMesh(const char* data);
    RobloxMesh(const char* data, RobloxMeshVersion version);
    ~RobloxMesh();

    std::string write(RobloxMeshVersion version = ROBLOX_MESH_UNKNOWN);

private:
    RobloxMeshVersion version = ROBLOX_MESH_UNKNOWN;
    bool isLoaded = false;

    bool isTexWDataPresent   = false; // Removed in v2.00, always false >v1.01
    bool isRgbaDataPresent   = false; // Added in v2.00, always true >v3.00
    bool isLodDataPresent    = false; // Added in v3.00
    bool isBoneDataPresent   = false; // Added in v4.00
    bool isSubsetDataPresent = false; // Added in v4.00
    bool isFacsDataPresent   = false; // Added in v5.00

    RobloxMesh(RobloxMeshSubset* subset);

    bool load(const char* data, bool detect = true);

    // v2.00+
    bool loadBinary(std::istringstream& stream);
    void writeBinary(std::ostringstream& stream);

    // v1.00, v1.01
    bool loadText(std::istringstream& stream);
    void writeText(std::ostringstream& stream);

    friend class RobloxMeshSubset;
};

struct RobloxMeshVertex
{
    float   px, py, pz; // Position
    float   nx, ny, nz; // Normal Vector
    float   tu, tv, tw; // UV Texture Coordinates (tex_W is only for data retention with corrupt v1.xx meshes)
    float   tx, ty, tz; // Tangent Vector
    uint8_t r, g, b, a; // RGBA Color Tinting
};

struct RobloxMeshFace
{
    RobloxMeshVertex* a; // 1st Vertex
    RobloxMeshVertex* b; // 2nd Vertex
    RobloxMeshVertex* c; // 3rd Vertex
};

struct RobloxMeshBone
{
    std::string name;

    RobloxMeshBone* parent;
    RobloxMeshBone* lodParent;

    float culling;

    // The rotation of each face bone is applied in ZYX order (i.e. CFrame.Angles(rx, ry, rz))
    // The position of the face bone is a translation offset relative to the rotation (i.e. rotation * CFrame.new(position))
    bool isFaceBone = false;

    // Uses Roblox's rotation matrix
    // CFrames (Y-up, -Z forward)
    float r00, r01, r02;
    float r10, r11, r12;
    float r20, r21, r22;

    float x, y, z;
};

// FACS data
enum RobloxMeshQuantizedMatrixVersion : uint16_t
{
    MATRIX_UNKNOWN = 0,
    MATRIX_RAW,       /* 1 - Floating point values are stored raw */
    MATRIX_QUANTIZED, /* 2 - Floating point values are quantized  */
};

struct RobloxMeshQuantizedMatrix
{
    RobloxMeshQuantizedMatrixVersion version;

    uint32_t rows, cols;
    float** matrix;
};

struct RobloxMeshQuantizedTransforms
{
    RobloxMeshQuantizedMatrix px;
    RobloxMeshQuantizedMatrix py;
    RobloxMeshQuantizedMatrix pz;

    RobloxMeshQuantizedMatrix rx;
    RobloxMeshQuantizedMatrix ry;
    RobloxMeshQuantizedMatrix rz;
};

// A combination of 2-3 FACS poses (read: strings) that are used to make a more
// complex facial expression
struct RobloxMeshTwoPoseCorrective
{
    std::string control0;
    std::string control1;
};

struct RobloxMeshThreePoseCorrective
{
    std::string control0;
    std::string control1;
    std::string control2;
};

} // namespace masher
