![Kek3d logo](screenshots/Logo12-29-2022.png)
# Kek3d

Kek3d is a graphics engine I'm developing using OpenGL and C++. It's express purpose is to be used to animate the process of Stable Diffusion on Terrain and to implement a interior decorating game.

Kek3d has originally been created using Visual Studio 2022 on Windows. To compile, please use CMake to create a build for whatever system you are currently running on. 

## File Structure
```
kek3d/
├─ data/                   # assets
│  ├─ shaders/             # all the shaders for the engine
│  ├─ textures/            # any necessary textures
├─ lib/                    # Cmake pulls in any necessary requirements
├─ screenshots/            # screenshots for the readme
├─ src/                    # main meat of the code
│  ├─ engine/              # Engine source is here
│  │  ├─ main.cpp          
│  │  ├─ Shader.cpp
│  ├─ lib/                 # More necessary requirements for engine
│  ├─ include/
│  │  ├─ engine/
│  │  │  ├─ Shader.hpp/    # Any necessary header files for main source
├─ README.md
```


## Build

### Windows
1) Download [CMake](https://cmake.org/download/). Ensure to set Cmake available on path to all users.
2) Open CMake GUI tool. Select source code to be the root directory of this cloned repo
3) Create a new folder called 'build'. Set this to be where we build our binaries
4) Press generate
5) Open the generated sln file with Visual Studio 2022
6) Under the solution explorer, find kek3d. Right click and press build
7) Right click kek3d again, set it as the startup project. 
8) Run the project.

### Mac
1) Navigate to the project folder
2) Make a build folder `mkdir build`  
3) Enter the folder `cd build`
4) Compile your project `cmake ..`
5) Call `make`
6) cd `/src/engine`
7) `./kek3d`

### Linux
1) Navigate to the project folder
2) Make a build folder `mkdir build`  
3) Enter the folder `cd build`
4) Compile your project `cmake ..`
5) Call `make`
6) cd `/src/engine`
7) `./kek3d`

## Requirements
Graphics card that support OpenGL 4.6

* GLFW library - https://www.glfw.org
* GLAD library - https://glad.dav1d.de/
* GLM library - https://glm.g-truc.net
* stb image library - https://github.com/nothings/stb/blob/master/stb_image.h

## Screenshots

https://user-images.githubusercontent.com/34428034/210031032-c1a0070b-4849-45ed-adfe-096b33d1d307.mp4

