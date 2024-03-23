#include "masher/RobloxMesh.hpp"

namespace masher {

RobloxMesh::RobloxMesh(const char* data)
{
    this->isLoaded = load(data);
}

RobloxMesh::RobloxMesh(const char* data, RobloxMeshVersion version)
{
    this->isLoaded = load(data, false);
}

RobloxMesh::RobloxMesh(RobloxMeshSubset* subset)
{
    // Set metadata
    this->isLoaded = true;
    this->version = ROBLOX_MESH_V4_00;
    this->isBoneDataPresent = true;

    this->vertices = subset->vertices;
    this->faces = subset->faces;
    this->bones = subset->getBones();
}

RobloxMesh::~RobloxMesh()
{
    // I don't know if I need to do this? but better safe than sorry
    delete this->vertices;
    delete this->faces;
    delete this->bones;
    delete this->subsets;
    delete this->lodMeshes;
    delete this->quantizedMatrices;
    delete this->twoPoseCorrectives;
    delete this->threePoseCorrectives;
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
            masher::SetLastError(MASHER_ERROR_UNSUPPORTED);
            return false;
        }
    }

    std::istringstream stream(data);

    // Ignore version header
    std::string dummy;
    std::getline(stream, dummy);

    return (this->version > ROBLOX_MESH_V2_00) ? loadBinary(stream) : loadText(stream);
}

std::string RobloxMesh::write(RobloxMeshVersion version)
{
    if (!this->isLoaded)
    {
        masher::SetLastError(MASHER_ERROR_ILLEGAL_OPERATION);
        return "";
    }

    std::ostringstream stream(std::ios::binary);

    version = version == ROBLOX_MESH_UNKNOWN ? this->version : version;

    if (this->version > ROBLOX_MESH_V2_00)
        writeBinary(stream);
    else
        writeText(stream);

    return stream.str();
}

bool RobloxMesh::loadBinary(std::istringstream& stream)
{
    masher::SetLastError(MASHER_ERROR_UNSUPPORTED);
    return false;
}

void RobloxMesh::writeBinary(std::ostringstream& stream)
{
    masher::SetLastError(MASHER_ERROR_UNSUPPORTED);
}

bool RobloxMesh::loadText(std::istringstream& stream)
{
    float scale = this->version == ROBLOX_MESH_V1_00 ? 0.5f : 1.0f;

    this->vertices = new std::deque<RobloxMeshVertex>();
    this->faces = new std::deque<RobloxMeshFace>();

    int numFaces;
    stream >> numFaces;

    if (stream.fail())
    {
        masher::SetLastError(MASHER_ERROR_STREAM_FAILURE);
        return false;
    }

    // OK, whatever, no faces, we'll load you so long as you're not corrupt
    if (numFaces <= 0)
    {
        // See if any data is left in the stream; if so, you're corrupt
        if (stream.rdbuf()->in_avail() > 0)
        {
            masher::SetLastError(MASHER_ERROR_V1_MALFORMED_FACE_COUNT);
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
            // create a new vertex directly in the deque
            // weird pointer issues occur if you don't use a deque since once you do
            // this->vertices->push_back(vertex), the pointer to the vertex is messed up
            // this sucks and I wish I can use vectors so I will fix this at some point in
            // the future. until then use the (eventual) c api ;)
            this->vertices->emplace_back();
            RobloxMeshVertex& vertex = this->vertices->back();

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
                    masher::SetLastError(MASHER_ERROR_V1_MALFORMED_VECTOR3);
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
                    if (z != 0.0f && !this->isTexWDataPresent)
                        this->isTexWDataPresent = true;

                    vertex.tu = x;
                    vertex.tv = 1.0f - y; // v1.xx quirk where tex_V is inverted
                    vertex.tw = z;
                }
            }

            if (j == 0)
                face.a = &this->vertices->at(vertices->size() - 1);
            else if (j == 1)
                face.b = &this->vertices->at(vertices->size() - 1);
            else
                face.c = &this->vertices->at(vertices->size() - 1);
        }

        this->faces->push_back(face);
    }

    return true;
}

void RobloxMesh::writeText(std::ostringstream& stream)
{
    float scale = this->version == ROBLOX_MESH_V1_00 ? 2.0f : 1.0f;

    stream << (this->version == ROBLOX_MESH_V1_00 ? "version 1.00" : "version 1.01") << std::endl;
    stream << this->faces->size() << std::endl;

    for (const RobloxMeshFace& face : *this->faces)
    {
        for (int i = 0; i < 3; i++)
        {
            const RobloxMeshVertex* vertex = (i == 0 ? face.a : (i == 1 ? face.b : face.c));

            stream << "[" << vertex->px * scale << "," << vertex->py * scale << "," << vertex->pz * scale << "]";
            stream << "[" << vertex->nx << "," << vertex->ny << "," << vertex->nz << "]";
            stream << "[" << vertex->tu << "," << 1.0f - vertex->tv << "," << vertex->tw << "]";
        }
    }
}

} // namespace masher
