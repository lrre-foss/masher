# masher

[![License](https://img.shields.io/github/license/lrre-foss/masher)](https://github.com/lrre-foss/masher/blob/trunk/LICENSE)
[![Star](https://img.shields.io/github/stars/lrre-foss/masher?style=social)](https://github.com/lrre-foss/masher/stargazers)

Roblox mesh loader

masher currently supports loading and writing to the following mesh versions:

- Version 1.00
- Version 1.01

Versions 6.00 and 7.00 are intentionally left unsupported since it has yet to officially release.

## Building

masher is available both as a CLI app as well as a standalone library for embedding in your own applications. You may turn off the `MASHER_BUILD_CLI` option to toggle building the CLI application by default. On all platforms, you can use CMake to run the following commands to build masher binaries from source:

```sh
mkdir cmake && cd cmake
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build . --config RelWithDebInfo
```

### Usage

As of right now, you may clone a mesh file using the mesher CLI by running `./masher.cli <path_to_mesh_file>`. You may also embed masher in your own project by simply including the `masher.hpp` header file and linking against masher. Below is a quick example of loading and converting a version 5.00 mesh file to version 4.00 using the masher library.

```cpp
#include <masher/RobloxMesh.hpp>

masher::RobloxMesh* mesh = new masher::RobloxMesh(file->getData());

if (!mesh->isLoaded() || (error = masher::GetLoaderError()) != masher::MASHER_OK) {
    printf("Failed to load mesh! Error code: %d\n", (int)error);
    return;
}

int numFaces = mesh->faces->size();
int numVertices = mesh->vertices->size();

printf("Successfully loaded %d faces and %d vertices!\n", numFaces, numVertices);

if (mesh->getVersion() > masher::ROBLOX_MESH_V1_00) {
    file->write(mesh->write(masher::ROBLOX_MESH_V1_01));
    printf("Successfully converted mesh to v1.01!\n");
}
```

## License

masher is licensed under the [MIT license](https://github.com/lrre-foss/masher/blob/trunk/LICENSE). A copy of it has been included with masher.

masher was written using [MaximumADHD's Roblox Mesh Format documentation](https://devforum.roblox.com/t/roblox-mesh-format/326114) as a reference for implementing the Roblox mesh file format.