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

#ifndef MASHER_LIB_EXPORT
    #define MASHER_LIB_EXPORT
#endif

#include "masher/RobloxMesh.hpp"

namespace masher {

Error lastError = MASHER_OK;

Error GetLastError()
{
    Error error = lastError;
    lastError = MASHER_OK;
    return error;
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

        std::string version = std::string(data).substr(8, 4);

        if (version == "1.00") this->version = ROBLOX_MESH_V1_00;
        else if (version == "1.01") this->version = ROBLOX_MESH_V1_01;
        else if (version == "2.00") this->version = ROBLOX_MESH_V2_00;
        else if (version == "3.00") this->version = ROBLOX_MESH_V3_00;
        else if (version == "3.01") this->version = ROBLOX_MESH_V3_01;
        else if (version == "4.00") this->version = ROBLOX_MESH_V4_00;
        else if (version == "4.01") this->version = ROBLOX_MESH_V4_01;
        else if (version == "5.00") this->version = ROBLOX_MESH_V5_00;
        else if (version == "6.00") this->version = ROBLOX_MESH_V6_00;
        else if (version == "7.00") this->version = ROBLOX_MESH_V7_00;
        else
        {
            lastError = MASHER_ERROR_INVALID_VERSION;
            return false;
        }
    }

    std::istringstream stream(data);

    // Ignore version header
    std::string dummy;
    std::getline(stream, dummy);

    // RGBA data didn't exist until v2.00. However, RGBA tinting wasn't made a default property of all vertices until v4.00.
    // Thus, we have check if it has RGBA data in loadV2/loadV3 by seeing if the mesh header's sizeof_Vertex == 40 and not 36.
    // If it is 40, we turn this on, and we add RobloxMeshVertexRGBAs instead of RobloxMeshVertexs to this->vertices.
    if (this->version > ROBLOX_MESH_V3_00)
        this->areVerticesRgba = true;

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
            lastError = MASHER_ERROR_UNSUPPORTED;
            return false;
        default:
            return false;
    }
}

std::string RobloxMesh::write(RobloxMeshVersion version)
{
    if (!hasLoaded)
    {
        lastError = MASHER_ERROR_ILLEGAL_OPERATION;
        return "";
    }

    std::ostringstream stream(std::ios::binary);

    version = version == ROBLOX_MESH_UNKNOWN ? this->version : version;

    switch (version)
    {
        case ROBLOX_MESH_V1_00:
        case ROBLOX_MESH_V1_01:
            writeV1(stream);
            break;
        case ROBLOX_MESH_V2_00:
            writeV2(stream);
            break;
        case ROBLOX_MESH_V3_00:
        case ROBLOX_MESH_V3_01:
            writeV3(stream);
            break;
        case ROBLOX_MESH_V4_00:
        case ROBLOX_MESH_V4_01:
        case ROBLOX_MESH_V5_00:
            writeV4(stream);
            break;
        case ROBLOX_MESH_V6_00:
        case ROBLOX_MESH_V7_00:
            lastError = MASHER_ERROR_UNSUPPORTED;
            return "";
    }

    return stream.str();
}

bool RobloxMesh::loadV1(std::istringstream& stream)
{
    float scale = this->version == ROBLOX_MESH_V1_00 ? 0.5f : 1.0f;

    this->vertices = new std::vector<RobloxMeshVertex>();
    this->faces = new std::vector<RobloxMeshFace>();

    int numFaces;
    stream >> numFaces;

    if (stream.fail())
    {
        lastError = MASHER_ERROR_STREAM_FAILURE;
        return false;
    }

    // OK, whatever, no faces, we'll load you so long as you're not corrupt
    if (numFaces <= 0)
    {
        // See if any data is left in the stream; if so, you're corrupt
        if (stream.rdbuf()->in_avail() > 0)
        {
            lastError = MASHER_ERROR_V1_MALFORMED_FACE_COUNT;
            return false;
        }

        return true;
    }

    for (int i = 0; i < numFaces; i++)
    {
        RobloxMeshFace face;

        // 3 vertices per face
        for (int j = 0; j < 3; j++)
        {
            RobloxMeshVertex vertex;
            for (int k = 0; k < 3; k++)
            {
                std::string xStr, yStr, zStr;
                float x, y, z;

                try
                {
                    // read until we find a non-numeric character (so we can read exponential stuff)
                    while (std::isdigit(stream.peek()) == 0 && stream.peek() != '-' && stream.peek() != '+')
                    {
                        stream.ignore();
                    }

                    std::getline(stream, xStr, ',');
                    std::getline(stream, yStr, ',');
                    std::getline(stream, zStr, ']');

                    x = std::stof(xStr);
                    y = std::stof(yStr);
                    z = std::stof(zStr);
                }
                catch (std::exception)
                {
                    lastError = MASHER_ERROR_V1_MALFORMED_VECTOR3;
                    return false;
                }

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
    lastError = MASHER_ERROR_UNSUPPORTED;

    return false;
}

void RobloxMesh::writeV2(std::ostringstream& stream)
{
    lastError = MASHER_ERROR_UNSUPPORTED;
}

bool RobloxMesh::loadV3(std::istringstream& stream)
{
    lastError = MASHER_ERROR_UNSUPPORTED;

    return false;
}

void RobloxMesh::writeV3(std::ostringstream& stream)
{
    lastError = MASHER_ERROR_UNSUPPORTED;
}

bool RobloxMesh::loadV4(std::istringstream& stream)
{
    lastError = MASHER_ERROR_UNSUPPORTED;

    return false;
}

void RobloxMesh::writeV4(std::ostringstream& stream)
{
    lastError = MASHER_ERROR_UNSUPPORTED;
}

} // namespace masher
