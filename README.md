# ReMania-GameBox

## Description

This project is an open source reimplementation of the GameBox game engine used in games by Nadeo such as TrackMania Nations Forever

## How to build

### Prerequisites

Clone the repository using this command:
```git clone https://github.com/ProtoByter/ReMania-GameBox --recurse-submodules```

On Debian based distributions you should be able to install the required packages with:<br>
```sudo apt install libsdl2-dev libsdl2-image-dev libglew-dev cmake```

On Arch based distributions you should run:<br>
```sudo pacman -S sdl2 glew cmake```<br>

On other distributions these libraries should be in their respective package repos. If not, just clone with submodules

### Building

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
and inside the `build/dist` directory there should be four shared libraries which you can then link to in your project.