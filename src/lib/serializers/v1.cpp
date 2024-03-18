#include <v1.hpp>

const char* convertToV1(const aiScene* scene, const char* version)
{
    return nullptr;
}

const aiScene* loadFromV1(const char* mesh, const char* version)
{
    return nullptr;
}

const char* convertToV1_00(const aiScene* scene)
{
    return convertToV1(scene, "1.00");
}

const char* convertToV1_01(const aiScene* scene)
{
    return convertToV1(scene, "1.01");
}

const aiScene* loadFromV1_00(const char* mesh)
{
    return loadFromV1(mesh, "1.00");
}

const aiScene* loadFromV1_01(const char* mesh)
{
    return loadFromV1(mesh, "1.01");
}