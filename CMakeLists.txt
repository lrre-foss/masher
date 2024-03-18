cmake_minimum_required(VERSION 3.5)

option(MASHER_BUILD_CLI "Build CLI" ON)

project(masher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
add_library(lib SHARED src/lib/RobloxMesh.cpp)
target_include_directories(lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
set_target_properties(lib PROPERTIES OUTPUT_NAME masher)

if(MASHER_BUILD_CLI)
    add_executable(cli src/cli/main.cpp)
    target_link_libraries(cli PRIVATE lib)
    set_target_properties(cli PROPERTIES OUTPUT_NAME masher.cli)
endif()