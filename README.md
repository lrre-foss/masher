# masher

[![License](https://img.shields.io/github/license/lrre-foss/masher)](https://github.com/lrre-foss/masher/blob/trunk/LICENSE)
[![Star](https://img.shields.io/github/stars/lrre-foss/masher?style=social)](https://github.com/lrre-foss/masher/stargazers)

Roblox mesh loader

masher currently supports the following mesh versions:

- Version 1.00
- Version 1.01
- Version 2.00 (WIP)
- Version 3.00 (WIP)
- Version 3.01 (WIP)
- Version 4.00 (WIP)
- Version 4.01 (WIP)
- Version 5.00 (WIP)

Versions 6.00 and 7.00 are intentionally left unsupported due to the fact that there is no Roblox mesh file available that has been encoded in these formats; thus, there can be no documentation on how it works. 

## Building

masher is available both as a CLI app as well as a standalone library for embedding in your own applications. On all platforms, you can use CMake to run the following commands to build masher binaries from source:

```sh
mkdir cmake && cd cmake
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build . --config RelWithDebInfo
```

### Usage

Below is an example of loading converting a version 5.00 mesh file to version 4.00 using masher.

```cpp
#include <masher/RobloxMesh.hpp>

masher::RobloxMesh* mesh = new masher::RobloxMesh(data, masher::VERSION_5_00);

if (!mesh->isLoaded()) {
    printf("Failed to load mesh!\n");
    return;
}

int faces = mesh->faces->size();
int vertices = mesh->vertices->size();
int bones = mesh->bones->size();

printf("Successfully loaded %d faces, %d vertices, and %d bones!\n", faces, vertices, bones);

if (mesh->hasFacsData()) {
    printf("Mesh has facial animation data! Will be lost during conversion.\n");
}

// Convert!
mesh->version = masher::VERSION_4_00;
std::string data = mesh->write();

printf("Successfully converted v5.00 mesh to v4.00!\n");
```

## License

masher is licensed under the [MIT license](https://github.com/lrre-foss/masher/blob/trunk/LICENSE). A copy of it has been included with masher.

masher was written using [MaximumADHD's Roblox Mesh Format documentation](https://devforum.roblox.com/t/roblox-mesh-format/326114) as a reference for reading and writing Roblox mesh file data.
