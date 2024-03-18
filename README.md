# masher

[![License](https://img.shields.io/github/license/lrre-foss/masher)](https://github.com/lrre-foss/masher/blob/trunk/LICENSE)
[![Star](https://img.shields.io/github/stars/lrre-foss/masher?style=social)](https://github.com/lrre-foss/masher/stargazers)

> [!IMPORTANT]
> This project is currently an active work-in-progress. We will not be accepting contributions at this time.

Roblox mesh loader

It can convert over 40+ different 3D file formats to the following Roblox mesh file formats (and vice versa):

- Version 1.00
- Version 1.01
- Version 2.00
- Version 3.00
- Version 3.01
- Version 4.00
- Version 4.01
- Version 5.00

Version 6.00 and version 7.00 are currently unsupported at this time.

## Building

masher is available both as a CLI app as well as a standalone library for embedding in your own applications. On all platforms, you can use CMake to run the following commands to build masher binaries from source:

```sh
mkdir cmake && cd cmake
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build . --config RelWithDebInfo
```

## License

masher is licensed under the [MIT license](https://github.com/lrre-foss/masher/blob/trunk/LICENSE). A copy of it has been included with masher.

masher uses [assimp](https://github.com/assimp/assimp) to process non-Roblox mesh formats. masher's Roblox mesh loader is based upon [MaximumADHD's Roblox Mesh Format documentation](https://devforum.roblox.com/t/roblox-mesh-format/326114).
