#ifndef MASHER_LIB_EXPORT
    #define MASHER_LIB_EXPORT
#endif

#include <masher/RobloxMesh.hpp>

/**
 * https://devforum.roblox.com/t/roblox-mesh-format/326114
 * 
 * V1 is simple position, normal, and UV Vector3s
 * V2 is the same represented in binary format
 * V3 adds support for LOD
 * V4 is a full overhaul adding support for skeletal data and mesh deformation
 * V5 is a major revision to V4 with additional FACS data at the end
 */

RobloxMesh::RobloxMesh(const char* meshData, RobloxMeshVersion version)
    : version(version) {

    if (version == ROBLOX_MESH_V1_00 || version == ROBLOX_MESH_V1_01) {
        loadV1(meshData);
    } else if (version == ROBLOX_MESH_V2_00) {
        loadV2(meshData);
    } else if (version == ROBLOX_MESH_V3_00) {
        loadV3(meshData);
    } else if (version == ROBLOX_MESH_V4_00) {
        loadV4(meshData);
    } else if (version == ROBLOX_MESH_V5_00) {
        loadV5(meshData);
    }
}

std::string RobloxMesh::write() {
    std::ostringstream oss(std::ios::binary);

    if (version == ROBLOX_MESH_V1_00 || version == ROBLOX_MESH_V1_01) {
        writeV1(oss);
    } else if (version == ROBLOX_MESH_V2_00) {
        writeV2(oss);
    } else if (version == ROBLOX_MESH_V3_00) {
        writeV3(oss);
    } else if (version == ROBLOX_MESH_V4_00) {
        writeV4(oss);
    } else if (version == ROBLOX_MESH_V5_00) {
        writeV5(oss);
    }

    return oss.str();
}

void RobloxMesh::loadV1(const char* meshData) {
    // Load V1
}

void RobloxMesh::loadV2(const char* meshData) {
    // Load V2
}

void RobloxMesh::loadV3(const char* meshData) {
    // Load V3
}

void RobloxMesh::loadV4(const char* meshData) {
    // Load V4
}

void RobloxMesh::loadV5(const char* meshData) {
    // Load V5
}

void RobloxMesh::writeV1(std::ostringstream& oss) {
    // Write V1
}

void RobloxMesh::writeV2(std::ostringstream& oss) {
    // Write V2
}

void RobloxMesh::writeV3(std::ostringstream& oss) {
    // Write V3
}

void RobloxMesh::writeV4(std::ostringstream& oss) {
    // Write V4
}

void RobloxMesh::writeV5(std::ostringstream& oss) {
    // Write V5
}