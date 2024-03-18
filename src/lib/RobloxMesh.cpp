/**
 * https://devforum.roblox.com/t/roblox-mesh-format/326114
 * 
 * V1 is simple position, normal, and UV Vector3s
 * V2 is the same represented in binary format
 * V3 adds support for LOD
 * V4 is a full overhaul adding support for skeletal data and mesh deformation
 * V5 is a major revision to V4 with additional FACS data at the end
 */

#ifndef MASHER_LIB_EXPORT
    #define MASHER_LIB_EXPORT
#endif

#include <masher/RobloxMesh.hpp>

RobloxMesh::RobloxMesh(const char* data, RobloxMeshVersion version) : version(version), loaded(false) {
    bool success = false;

    if (version == ROBLOX_MESH_V1_00 || version == ROBLOX_MESH_V1_01) {
        success = loadV1(data);
    } else if (version == ROBLOX_MESH_V2_00) {
        success = loadV2(data);
    } else if (version == ROBLOX_MESH_V3_00) {
        success = loadV3(data);
    } else if (version == ROBLOX_MESH_V4_00) {
        success = loadV4(data);
    } else if (version == ROBLOX_MESH_V5_00) {
        success = loadV5(data);
    }
}

std::string RobloxMesh::write() {
    std::ostringstream stream(std::ios::binary);

    if (version == ROBLOX_MESH_V1_00 || version == ROBLOX_MESH_V1_01) {
        writeV1(stream);
    } else if (version == ROBLOX_MESH_V2_00) {
        writeV2(stream);
    } else if (version == ROBLOX_MESH_V3_00) {
        writeV3(stream);
    } else if (version == ROBLOX_MESH_V4_00) {
        writeV4(stream);
    } else if (version == ROBLOX_MESH_V5_00) {
        writeV5(stream);
    }

    return stream.str();
}

bool RobloxMesh::loadV1(const char* data) {
    return true;
}

bool RobloxMesh::loadV2(const char* data) {
    return true;
}

bool RobloxMesh::loadV3(const char* data) {
    return true;
}

bool RobloxMesh::loadV4(const char* data) {
    return true;
}

bool RobloxMesh::loadV5(const char* data) {
    return true;
}

void RobloxMesh::writeV1(std::ostringstream& stream) {

}

void RobloxMesh::writeV2(std::ostringstream& stream) {
}

void RobloxMesh::writeV3(std::ostringstream& stream) {

}

void RobloxMesh::writeV4(std::ostringstream& stream) {

}

void RobloxMesh::writeV5(std::ostringstream& stream) {

}