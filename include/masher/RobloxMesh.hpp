#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>

#include "masher.h"

namespace masher {

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

struct RobloxMeshVertex
{
    float   px, py, pz; // Position
    float   nx, ny, nz; // Normal Vector
    float   tu, tv, tw; // UV Texture Coordinates
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
    const char* name;

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

enum RobloxMeshQuantizedMatrixVersion : uint16_t
{
    UNKNOWN = 0,
    RAW,       /* 1 - Floating point values are stored raw */
    QUANTIZED, /* 2 - Floating point values are quantized  */
};

struct RobloxMeshQuantizedMatrix
{
    RobloxMeshQuantizedMatrixVersion version;
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

struct RobloxMeshTwoPoseCorrective
{
    RobloxMeshBone* control0;
    RobloxMeshBone* control1;
};

struct RobloxMeshThreePoseCorrective
{
    RobloxMeshBone* control0;
    RobloxMeshBone* control1;
    RobloxMeshBone* control2;
};

class MASHER_LIB_API RobloxMesh
{
public:
    RobloxMeshVersion getVersion() { return version; }
    bool hasLoaded() { return isLoaded; }

    RobloxMeshVertex* vertices;
    RobloxMeshFace* faces;
    RobloxMeshBone* bones;
    RobloxMesh* lodMeshes;

    // Facial animation data
    RobloxMeshQuantizedMatrix* quantizedMatrices;
    RobloxMeshTwoPoseCorrective* twoPoseCorrectives;
    RobloxMeshThreePoseCorrective* threePoseCorrectives;

    bool holdsTexWData()   { return isTexWDataPresent;   }
    bool holdsRgbaData()   { return isRgbaDataPresent;   }
    bool holdsLodData()    { return isLodDataPresent;    }
    bool holdsBoneData()   { return isBoneDataPresent;   }
    bool holdsSubsetData() { return isSubsetDataPresent; }
    bool holdsFacsData()   { return isFacsDataPresent;   }

    RobloxMesh(const char* data);
    RobloxMesh(const char* data, RobloxMeshVersion version);

    std::string write(RobloxMeshVersion version = ROBLOX_MESH_UNKNOWN);

private:
    RobloxMeshVersion version = ROBLOX_MESH_UNKNOWN;
    bool isLoaded = false;

    bool isTexWDataPresent   = false; // removed in v2.00
    bool isRgbaDataPresent   = false; // added in v2.00
    bool isLodDataPresent    = false; // added in v3.00
    bool isBoneDataPresent   = false; // added in v4.00
    bool isSubsetDataPresent = false; // added in v4.00
    bool isFacsDataPresent   = false; // added in v5.00

    bool load(const char* data, bool detect = false);

    // v1.00, v1.01
    bool loadText(std::istringstream& stream);
    void writeText(std::ostringstream& stream);

    // v2.00+
    bool loadBinary(std::istringstream& stream);
    void writeBinary(std::ostringstream& stream);
};

} // namespace masher