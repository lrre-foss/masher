cmake_minimum_required(VERSION 3.5)

project(masher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(lib SHARED src/lib/RobloxMesh.cpp)
add_executable(cli src/cli/main.cpp)

target_include_directories(lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cli PRIVATE lib)

set_target_properties(lib PROPERTIES OUTPUT_NAME masher)
set_target_properties(cli PROPERTIES OUTPUT_NAME masher.cli)