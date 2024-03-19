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

    masher::RobloxMesh* mesh = new masher::RobloxMesh(buffer.str().c_str());

    if (!mesh->isLoaded() || masher::GetLastError() != masher::MASHER_OK)
    {
        std::cerr << "Failed to load mesh!" << std::endl;
        return 1;
    }

    std::string versionStrs[] = {"1.00", "1.01", "2.00", "3.00", "3.01", "4.00", "4.01", "5.00", "6.00", "7.00"};
    std::cout << "Successfully loaded mesh v" << versionStrs[mesh->getVersion()] << "!" << std::endl;
    std::cout << "Loaded " << mesh->vertices->size() << " vertices and " << mesh->faces->size() << " faces" << std::endl;
    std::cout << "Attempting to write to file '" + clone.string() + "'..." << std::endl;

    std::ofstream output(clone, std::ios::binary);
    output << mesh->write();
    output.close();

    std::cout << "Successfully wrote to file '" + clone.string() + "'!" << std::endl;

    return 0;
}