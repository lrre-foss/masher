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

RobloxMesh::RobloxMesh(const char* data) : loaded(false)
{
    loaded = load(data, true);
}

RobloxMesh::RobloxMesh(const char* data, RobloxMeshVersion version) : version(version), loaded(false)
{
    loaded = load(data, false);
}

bool RobloxMesh::load(const char* data, bool detect)
{
    if (detect)
    {
        if (strlen(data) < 13)
            return false;

        std::map<std::string, RobloxMeshVersion> versions = {
            { "version 1.00\n", ROBLOX_MESH_V1_00 },
            { "version 1.01\n", ROBLOX_MESH_V1_01 },
            { "version 2.00\n", ROBLOX_MESH_V2_00 },
            { "version 3.00\n", ROBLOX_MESH_V3_00 },
            { "version 4.00\n", ROBLOX_MESH_V4_00 },
            { "version 5.00\n", ROBLOX_MESH_V5_00 }
        };

        std::string version = std::string(data).substr(0, 13);

        if (versions.count(version) == 0)
            return false;
        
        this->version = versions[version];
    }

    std::map<RobloxMeshVersion, Loader> loaders = {
        { ROBLOX_MESH_V1_00, &RobloxMesh::loadV1 },
        { ROBLOX_MESH_V1_01, &RobloxMesh::loadV1 },
        { ROBLOX_MESH_V2_00, &RobloxMesh::loadV2 },
        { ROBLOX_MESH_V3_00, &RobloxMesh::loadV3 },
        { ROBLOX_MESH_V4_00, &RobloxMesh::loadV4 },
        { ROBLOX_MESH_V5_00, &RobloxMesh::loadV5 }
    };

    std::istringstream stream(data);
    stream.ignore(13); // "version x.xx\n"

    return (this->*loaders[this->version])(stream);
}

std::string RobloxMesh::write()
{
    std::ostringstream stream(std::ios::binary);

    std::map<RobloxMeshVersion, Writer> writers = {
        { ROBLOX_MESH_V1_00, &RobloxMesh::writeV1 },
        { ROBLOX_MESH_V1_01, &RobloxMesh::writeV1 },
        { ROBLOX_MESH_V2_00, &RobloxMesh::writeV2 },
        { ROBLOX_MESH_V3_00, &RobloxMesh::writeV3 },
        { ROBLOX_MESH_V4_00, &RobloxMesh::writeV4 },
        { ROBLOX_MESH_V5_00, &RobloxMesh::writeV5 }
    };

    (this->*writers[this->version])(stream);

    return stream.str();
}

bool RobloxMesh::loadV1(std::istringstream& stream)
{
    float scale = this->version == ROBLOX_MESH_V1_00 ? 0.5f : 1.0f;

    this->vertices = new std::vector<RobloxMeshVertex>();
    this->faces = new std::vector<RobloxMeshFace>();

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
                std::string vector3;
                stream >> vector3;
                vector3 = vector3.substr(1, vector3.size() - 2);

                std::istringstream iss(vector3);
                std::string val;

                std::getline(iss, val, ',');
                float x = std::stof(val);

                std::getline(iss, val, ',');
                float y = std::stof(val);

                std::getline(iss, val, ',');
                float z = std::stof(val);

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

bool RobloxMesh::loadV2(std::istringstream& stream)
{
    return true;
}

bool RobloxMesh::loadV3(std::istringstream& stream)
{
    return true;
}

bool RobloxMesh::loadV4(std::istringstream& stream)
{
    return true;
}

bool RobloxMesh::loadV5(std::istringstream& stream)
{
    return true;
}

void RobloxMesh::writeV1(std::ostringstream& stream)
{

}

void RobloxMesh::writeV2(std::ostringstream& stream)
{

}

void RobloxMesh::writeV3(std::ostringstream& stream)
{

}

void RobloxMesh::writeV4(std::ostringstream& stream)
{

}

void RobloxMesh::writeV5(std::ostringstream& stream)
{

}