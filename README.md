![lei3d logo](screenshots/lei.png)
# lei3d

lei3d is a graphics engine I'm developing using OpenGL and C++. It's express purpose has been to model 3D terrain generation through GANs and as the properietary engine for the game SkyLei.

lei3d has originally been created using Visual Studio 2022 on Windows. It has been tested on other platforms, to compile, please follow the instructions below to use CMake to create a build for whatever system you are currently running on. 

## File Structure
```
lei3d/
├─ data/                   # assets
│  ├─ shaders/                 # all the shaders for the engine
│  ├─ textures/                # any necessary textures
├─ lib/                    # Cmake pulls in any necessary requirements
├─ screenshots/            # screenshots for the readme
├─ src/                    # main meat of the code
│  ├─ include/                 # All the header files are here            
│  │  ├─ engine/ 
│  │  │  ├─ Engine.hpp/            # Initialization, Loading, and main render loop logic
│  │  │  ├─ FlyCamera.hpp/         # Flight Camera, soon will have FPS camera
│  │  │  ├─ Shader.hpp/            # Shader code
│  │  ├─ pcg/ 
│  │  │  ├─ PCGHelpers.hpp/        # Helpers to create PCG meshes
│  ├─ engine/              # All the source files are here
│  │  ├─ Engine.cpp          
│  │  ├─ FlyCamera.cpp
│  │  ├─ main.cpp   
│  │  ├─ PCGHelpers.cpp        
│  │  ├─ Shader.cpp
│  ├─ lib/                 # More necessary requirements for engine

├─ README.md
```

## Build

### Windows
1) Download [CMake](https://cmake.org/download/). Ensure to set Cmake available on path to all users.
2) Open CMake GUI tool. Select source code to be the root directory of this cloned repo
3) Create a new folder called 'build'. Set this to be where we build our binaries
4) Press generate
5) Open the generated sln file with Visual Studio 2022
6) Under the solution explorer, find lei3d. Right click and press build
7) Right click lei3d again, set it as the startup project. 
8) Run the project.

### Mac
1) Navigate to the project folder
2) Make a build folder `mkdir build`  
3) Enter the folder `cd build`
4) Compile your project `cmake ..`
5) Call `make`
6) cd `/src/engine`
7) `./lei3d`

### Linux
1) Navigate to the project folder
2) Make a build folder `mkdir build`  
3) Enter the folder `cd build`
4) Compile your project `cmake ..`
5) Call `make`
6) cd `/src/engine`
7) `./lei3d`

## Requirements
Graphics card that support OpenGL 4.6

* GLFW library - https://www.glfw.org
* GLAD library - https://glad.dav1d.de/
* GLM library - https://glm.g-truc.net
* stb image library - https://github.com/nothings/stb/blob/master/stb_image.h

# SkyLei - Surf The Sky Islands
SkyLei is a first person 3D surfing game heavily inspired by the airstrafing mechanic born out of the Quake Engine. 

In a world that is shattered to pieces and taken to islands in the sky, SkyLei follows the exhilarating plight of Leilani Leaf through a colorless, apocalyptic world. As a part of the SkyLeis, a group of vigilante freedom fighters, Leilani’s objective is to fly around the world and bring color back to the environment. Primary gameplay consists of surfing through the world as quickly as possible using lei's embued with magical powers, and successfully bringing color and life back to each level of the world!

## Controls:
WASD - movement 
look around the scene by moving mouse
tab to unlock cursor to interact with the gui


## Screenshots


https://github.com/kevinsadi/lei3d/assets/34428034/aa5ea3b3-0151-4f35-8f73-911fa94d8c9a


https://github.com/kevinsadi/lei3d/assets/34428034/8bb4486a-66a1-429d-a42a-c4a8afaf8f15





