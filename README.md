# NekoEngine Classic

## Description

Cross-platform game engine for older systems.

Supported platforms:
* Windows 2000 and newer
* Linux on x86_64 & arm64 (ppc64 should work too, but i can't test it)
* Mac OS X 10.4 ppc
* Xbox (not fully functional)

Supported rendering APIs:
* Direct3D8
* Direct3D9
* OpenGL 2.0+ (the 1.5 implementation in Mac OS X 10.4 has all the needed functionality)

This repository is a mirror which is not frequently updated.

## Build instructions

### Windows

#### Prerequisites
* Visual C++ 6
* Microsoft Platform SDK February 2003 (latest Platform SDK to support VC6)
* DirectX 9.0b SDK or DirectX 8.1 SDK

#### Build
1. Clone the repository
2. Clone the data repository found at https://github.com/nalexandru/NekoEngineClassicData in SOURCE_DIR\bin\Data
3. Open NekoEngine.dsw
4. Build NekoEngine and one of the drivers

### *nix

#### Prerequisites
* gcc 4 (3 might work but it's not tested)
* X11 & OpenGL development files
* autoconf
* automake

#### Build
1. Clone the repository
2. Clone the data repository found at https://github.com/nalexandru/NekoEngineClassicData in SOURCE_DIR\build\Data
3. Run the following
```
autoreconf --install
cd build
../configure
make
mkdir -p Data/Shaders/OpenGL
cp ../Drivers/OpenGL/Shaders/*.* Data/Shaders/OpenGL/
```

#### Notes
* It has been tested only on Linux on:
	* arm64: Raspberry PI running the 64-bit version of Raspbian
	* x86_64: Ubuntu 20.10

### Mac OS X

#### Prerequisites
* Xcode 2.4

#### Build
1. Clone the repository
2. Clone the data repository found at https://github.com/nalexandru/NekoEngineData in SOURCE_DIR\build\Data
3. Build NekoEngine.xcodeproj

#### Notes
* It has only been tested on Tiger running on PowerPC machines (a G4 and a G5)

## 3rd Party libraries

* PhysFS - https://icculus.org/physfs/
* Lua - https://www.lua.org/
* cgltf - https://github.com/jkuhlmann/cgltf
* Jsmn - https://github.com/zserge/jsmn
* stb_image, stb_image_write, stb_vorbis - https://github.com/nothings/stb
* glad - https://glad.dav1d.de/
