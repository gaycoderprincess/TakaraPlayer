# Takara Player

Simple bubbly music player written in C++ with DirectX 11, ImGui and BASS.

Supports WAV, MP3, FLAC and OGG files.

## Building

Building is done on an Arch Linux system with CLion and vcpkg being used for the build process. 

Before you begin, clone nya-common to a folder next to this one, so it can be found.

Required packages: `mingw-w64-gcc vcpkg`

To install all dependencies, use:
```console
vcpkg install taglib:x64-mingw-static
```

Once installed, copy files from `~/.vcpkg/vcpkg/installed/x64-mingw-static/`:

- `include` dir to `nya-common/3rdparty`
- `lib` dir to `nya-common/lib64`

You should be able to build the project now in CLion.

After building, make sure to also get BASS from https://www.un4seen.com/bass.html
