cmake_minimum_required(VERSION 3.5)

project(masher LANGUAGES CXX)

find_package(assimp CONFIG REQUIRED)

add_library(lib SHARED
    src/lib/include/serializers/v1.hpp
    src/lib/include/serializers/v2.hpp
    src/lib/include/serializers/v3.hpp
    src/lib/include/serializers/v4.hpp
    src/lib/include/serializers/v5.hpp
    src/lib/include/masher.hpp

    src/lib/serializers/v1.cpp
    src/lib/serializers/v2.cpp
    src/lib/serializers/v3.cpp
    src/lib/serializers/v4.cpp
    src/lib/serializers/v5.cpp
    src/lib/masher.cpp
)
add_executable(cli src/cli/main.cpp)

set_target_properties(lib PROPERTIES OUTPUT_NAME masher)
set_target_properties(cli PROPERTIES OUTPUT_NAME masher.cli)

target_include_directories(lib PRIVATE ${ASSIMP_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src/lib/include)
target_include_directories(cli PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/include)

target_link_libraries(lib PRIVATE ${ASSIMP_LIBRARIES})
target_link_libraries(cli PRIVATE masher)