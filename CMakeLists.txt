cmake_minimum_required(VERSION 3.5)

option(MASHER_BUILD_CLI "Build CLI" ON)

project(masher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(masher SHARED src/masher.cpp src/RobloxMesh.cpp src/RobloxMeshSubset.cpp)
target_compile_definitions(masher PRIVATE MASHER_LIB_EXPORT)
target_include_directories(masher PUBLIC include)

if(MASHER_BUILD_CLI)
    add_executable(masher.cli src/main.cpp)
    target_link_libraries(masher.cli PRIVATE masher)
endif()