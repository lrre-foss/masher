#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <masher/RobloxMesh.hpp>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./masher.cli <file>" << std::endl;
        return 1;
    }

    std::filesystem::path original(argv[1]);
    std::filesystem::path clone = original.parent_path() / (original.stem().string() + ".mesh.clone");

    std::ifstream file(original, std::ios::binary);

    if (!file)
    {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    RobloxMesh* mesh = new RobloxMesh(buffer.str().c_str());

    if (!mesh->isLoaded())
    {
        std::cerr << "Failed to load mesh!" << std::endl;
        return 1;
    }

    std::cout << "Successfully loaded mesh!" << std::endl;

    std::cout << "Version: ";
    if (mesh->version == ROBLOX_MESH_V1_00)
        std::cout << "1.00" << std::endl;
    else if (mesh->version == ROBLOX_MESH_V1_01)
        std::cout << "1.01" << std::endl;
    else if (mesh->version == ROBLOX_MESH_V2_00)
        std::cout << "2.00" << std::endl;
    else if (mesh->version == ROBLOX_MESH_V3_00)
        std::cout << "3.00" << std::endl;
    else if (mesh->version == ROBLOX_MESH_V4_00)
        std::cout << "4.00" << std::endl;
    else if (mesh->version == ROBLOX_MESH_V5_00)
        std::cout << "5.00" << std::endl;

    std::cout << "Loaded " << mesh->vertices->size() << " vertices and " << mesh->faces->size() << " faces" << std::endl;

    std::cout << "Attempting to write to file '" + clone.string() + "'..." << std::endl;

    std::ofstream output(clone, std::ios::binary);
    output << mesh->write();
    output.close();

    std::cout << "Successfully wrote to file '" + clone.string() + "'!" << std::endl;

    return 0;
}