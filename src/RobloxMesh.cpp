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

#include "masher/RobloxMesh.hpp"

namespace masher {

RobloxMeshError lastError = ROBLOX_MESH_ERROR_NONE;

RobloxMeshError GetLastError()
{
    return lastError;
}

RobloxMesh::RobloxMesh(const char* data) : hasLoaded(false)
{
    hasLoaded = load(data, true);
}

RobloxMesh::RobloxMesh(const char* data, RobloxMeshVersion version) : version(version), hasLoaded(false)
{
    hasLoaded = load(data, false);
}

bool RobloxMesh::load(const char* data, bool detect)
{
    if (detect)
    {
        if (strlen(data) < 12)
            return false;

        std::string version = std::string(data).substr(0, 12);

        if (version == "version 1.00")
            this->version = ROBLOX_MESH_V1_00;
        else if (version == "version 1.01")
            this->version = ROBLOX_MESH_V1_01;
        else if (version == "version 2.00")
            this->version = ROBLOX_MESH_V2_00;
        else if (version == "version 3.00")
            this->version = ROBLOX_MESH_V3_00;
        else if (version == "version 3.01")
            this->version = ROBLOX_MESH_V3_01;
        else if (version == "version 4.00")
            this->version = ROBLOX_MESH_V4_00;
        else if (version == "version 4.01")
            this->version = ROBLOX_MESH_V4_01;
        else if (version == "version 5.00")
            this->version = ROBLOX_MESH_V5_00;
        else if (version == "version 6.00")
            this->version = ROBLOX_MESH_V6_00;
        else if (version == "version 7.00")
            this->version = ROBLOX_MESH_V7_00;
        else {
            lastError = ROBLOX_MESH_ERROR_INVALID_VERSION;
            return false;
        }
    }

    std::istringstream stream(data);

    // Ignore version header
    std::string dummy;
    std::getline(stream, dummy);

    switch (this->version)
    {
        case ROBLOX_MESH_V1_00:
        case ROBLOX_MESH_V1_01:
            return loadV1(stream);
        case ROBLOX_MESH_V2_00:
            return loadV2(stream);
        case ROBLOX_MESH_V3_00:
        case ROBLOX_MESH_V3_01:
            return loadV3(stream);
        case ROBLOX_MESH_V4_00:
        case ROBLOX_MESH_V4_01:
        case ROBLOX_MESH_V5_00:
            return loadV4(stream);
        case ROBLOX_MESH_V6_00:
        case ROBLOX_MESH_V7_00:
            lastError = ROBLOX_MESH_ERROR_UNSUPPORTED;
            return false;
        default:
            return false;
    }
}

std::string RobloxMesh::write()
{
    std::ostringstream stream(std::ios::binary);

    switch (this->version)
    {
        case ROBLOX_MESH_V1_00:
        case ROBLOX_MESH_V1_01:
            writeV1(stream);
            break;
        case ROBLOX_MESH_V2_00:
            writeV2(stream);
            break;
        case ROBLOX_MESH_V3_00:
            writeV3(stream);
            break;
        case ROBLOX_MESH_V4_00:
        case ROBLOX_MESH_V5_00:
            writeV4(stream);
            break;
    }

    return stream.str();
}

bool RobloxMesh::loadV1(std::istringstream& stream)
{
    float scale = this->version == ROBLOX_MESH_V1_00 ? 0.5f : 1.0f;

    this->vertices = new std::vector<RobloxMeshVertex>();
    this->faces = new std::vector<RobloxMeshFace>();

    try
    {
        int faces;
        stream >> faces;

        for (int i = 0; i < faces; i++)
        {
            RobloxMeshFace face;

            // 3 vertices per face
            for (int j = 0; j < 3; j++)
            {
                RobloxMeshVertex vertex;
                for (int k = 0; k < 3; k++)
                {
                    std::string xStr, yStr, zStr;

                    // read until we find a non-numeric character (so we can read exponential stuff)
                    while (std::isdigit(stream.peek()) == 0 && stream.peek() != '-' && stream.peek() != '+')
                    {
                        stream.ignore();
                    }

                    std::getline(stream, xStr, ',');
                    std::getline(stream, yStr, ',');
                    std::getline(stream, zStr, ']');

                    float x = std::stof(xStr);
                    float y = std::stof(yStr);
                    float z = std::stof(zStr);

                    if (k == 0)
                    {
                        vertex.px = x * scale;
                        vertex.py = y * scale;
                        vertex.pz = z * scale;
                    }
                    else if (k == 1)
                    {
                        vertex.nx = x;
                        vertex.ny = y;
                        vertex.nz = z;
                    }
                    else
                    {
                        vertex.tu = x;
                        vertex.tv = 1.0f - y; // v1.xx quirk where tex_V is inverted
                    }
                }

                this->vertices->push_back(vertex);

                if (j == 0)
                    face.a = static_cast<uint32_t>(vertices->size() - 1);
                else if (j == 1)
                    face.b = static_cast<uint32_t>(vertices->size() - 1);
                else
                    face.c = static_cast<uint32_t>(vertices->size() - 1);
            }

            this->faces->push_back(face);
        }
    }
    catch (std::exception& e)
    {
#ifdef _DEBUG
        throw e;
#endif

        return false;
    }

    return true;
}

void RobloxMesh::writeV1(std::ostringstream& stream)
{
    float scale = this->version == ROBLOX_MESH_V1_00 ? 2.0f : 1.0f;

    stream << (this->version == ROBLOX_MESH_V1_00 ? "version 1.00" : "version 1.01") << std::endl;
    stream << this->faces->size() << std::endl;

    for (const RobloxMeshFace& face : *this->faces)
    {
        for (int i = 0; i < 3; i++)
        {
            const RobloxMeshVertex& vertex = (*this->vertices)[i == 0 ? face.a : (i == 1 ? face.b : face.c)];

            stream << "[" << vertex.px * scale << "," << vertex.py * scale << "," << vertex.pz * scale << "]";
            stream << "[" << vertex.nx << "," << vertex.ny << "," << vertex.nz << "]";
            stream << "[" << vertex.tu << "," << 1.0f - vertex.tv << ",0]";
        }
    }
}

bool RobloxMesh::loadV2(std::istringstream& stream)
{
    return true;
}

void RobloxMesh::writeV2(std::ostringstream& stream)
{

}

bool RobloxMesh::loadV3(std::istringstream& stream)
{
    return true;
}

void RobloxMesh::writeV3(std::ostringstream& stream)
{

}

bool RobloxMesh::loadV4(std::istringstream& stream)
{
    return true;
}

void RobloxMesh::writeV4(std::ostringstream& stream)
{

}

} // namespace masher