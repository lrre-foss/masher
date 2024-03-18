#pragma once

#include "serializer.hpp"

const char* convertToV1(const aiScene* scene, const char* version);
const aiScene* loadFromV1(const char* mesh, const char* version);

const char* convertToV1_00(const aiScene* scene);
const aiScene* loadFromV1_00(const char* mesh);

const char* convertToV1_01(const aiScene* scene);
const aiScene* loadFromV1_01(const char* mesh);