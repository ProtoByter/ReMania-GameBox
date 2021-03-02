# ReMania-GameBox

## Description

This project is an open source reimplementation of the GameBox game engine used in games by Nadeo such as TrackMania Nations Forever

## How to build

### Building on Linux

On linux you don't need to clone with submodules since all the libraries should be available through the package manager.

On Debian based distibutions you should be able to install the required packages with:<br>
```sudo apt install libsdl2-dev libglew-dev cmake```

On Arch based distributions you should run:<br>
```sudo pacman -S sdl2 glew cmake```

On other distributions these libraries should be in their respective package repos. If not, just clone with submodules

After installing the prerequisites follow one of these methods

### <b>Method 1</b> (In my opinion the better way, requires the use of a git repo and using CMake)
Include this project as a submodule in your own repository and just add this to your CMake file like this:
```
add_subdirectory(<GIT SUBMODULE DIR>)
```
and add `ReMania-GameBox` to the targets that require it, this should automatically build all three libraries and link to them

### <b>Method 2</b> (Not as good, in my opinion)


Run these commands:<br>
```
mkdir build
cd build
cmake ..
make -j
```
and inside the `build/dist` directory there should be three shared libraries which you can then link to in your project.

### Building on Windows

On Windows you do need to clone with submodules. The best way to do this is by running (you need the `git` tool installed):<br>
```git clone https://github.com/ProtoByter/ReMania-GameBox --recurse-submodules```

Now follow the instuctions for building on Linux, without the package manager stuff


