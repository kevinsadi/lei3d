# lei3D / SkyLei
![skylei logo](screenshots/skylei.png)

## About
This is a dual repo to support the game SkyLei, and bespoke game engine Lei3D that it was built on.

If you wanted to know more about our process, and/or want to learn more about the engine, please check out:
* Our [design docs]((https://docs.google.com/document/d/1Ktms7R8Q_NFXl61THuaZg5ugdS3S1yB7YlfEAdG3e6E/edit))
* Our [technical docs](https://docs.google.com/document/d/1xWzLR5w1hAfVA2lAmUGGxLnd27bZAXDg0xyif3sXK6A/edit?usp=sharing)

## Features
![lei3d logo](screenshots/lei.png)
* Core
  * Build System for Windows, Mac, Linux - CMake
  * Physics, Quake-like air strafing, surfing, and bunny hopping. Rigid Body Collisions - Bullet3
  * GUI and dev tools - imgui
    * Level Editor
    * Frames Per Second (FPS) counter
    * Level Switching
    * Scene Edit/Play mode & Pausing/Resetting Levels
  * Player Controller / Input System
  * Entity Component System (ECS)
    * Includes naming for entities
    * Modify transform of entities from GUI
  * Forward Lighting 
    * Physically Based Material System
    * Directional Lights
    * Cascaded Shadows
    * Skybox Rendering
    * Stencil Masking
* Post Effects
  * Desaturation 
  * Screen Space Reflections (SSR)
  * Distance Based Fog

## Build

lei3d has originally been created using Visual Studio 2022 on Windows. We currently have two build targets, one contains the level editor and one is the game on it's own. It has been tested on other platforms, to compile, please follow the instructions below to use CMake to create a build for whatever system you are currently running on. You can consult alternative building instructions for other IDEs on the [technical docs](https://docs.google.com/document/d/1xWzLR5w1hAfVA2lAmUGGxLnd27bZAXDg0xyif3sXK6A/edit?usp=sharing).

### Windows
1) Download [CMake](https://cmake.org/download/). Ensure to set Cmake available on path to all users.
2) Open CMake GUI tool. Select source code to be the root directory of this cloned repo
3) Create a new folder called 'build'. Set this to be where we build our binaries
4) Press generate
5) Open the generated sln file with Visual Studio 2022
6) Under the solution explorer, find lei3d_lib. Right click and press build. This builds the library for lei3d.
7) Under the solution explorer, find either LeiEditor_exe for the game OR SkyLei_exe for the game. Right click and press build on the respective solution you want. 
8) Right click on the same solution you want to run again, set it as the startup project. 
9) Run the project (top left).

### Mac
1) Navigate to the project folder
2) Make a build folder `mkdir build`  
3) Enter the folder `cd build`
4) Compile your project `cmake ..`
5) Call `make`
6) cd `/src/engine`
7) `./LeiEditor_exe` for the editor OR `./SkyLei_exe` for the game

### Linux
1) Navigate to the project folder
2) Make a build folder `mkdir build`  
3) Enter the folder `cd build`
4) Compile your project `cmake ..`
5) Call `make`
6) cd `/src/engine`
7) `./LeiEditor_exe` for the editor OR `./SkyLei_exe` for the game

### Build Types
Depending on your needs with the engine, there are multiple ways you can build the engine. If you want to build a Debug build, include 
`cmake -DCMAKE_BUILD_TYPE=Debug` when you first run CMake. You can specify the same cmake parameter for Release builds. 

## requirements
Graphics card that support OpenGL 4.6

* GLFW library - https://www.glfw.org
* GLAD library - https://glad.dav1d.de/
* GLM library - https://glm.g-truc.net
* stb image library - https://github.com/nothings/stb/blob/master/stb_image.h

# SkyLei - Surf The Sky Islands

## Overview
[SkyLei](https://docs.google.com/document/d/1Ktms7R8Q_NFXl61THuaZg5ugdS3S1yB7YlfEAdG3e6E/edit) is a game that presents a unique twist in the world of speed running games by focusing on slowing down.

In a world that is shattered to pieces and taken to islands in the sky, SkyLei follows the exhilarating plight of Leilani Leaf through a colorless, once beautiful world. Primary gameplay consists of racing around and exploring a surreal-dreamlike world with physics-based movement. The levels of the world add new features that inspire new routes and discovery of the world. This adds challenge and enjoyment to players of any skill-level. At the very end of each level, Leilani brings color back to the world when she discovers the beauty of all that surrounds her. 

## Controls:
| Keys  | Action                        |
| ----- | ----------------------------- |
| WASD  | Movement                      |
| tab   | Open Debug GUI                |
| q     | Lower Camera in Fly Mode      |
| e     | Raise Camera in Fly Mode      |
| r     | Reset Scene                   |
| p     | Toggle Between Pause and Play |
| space | jump in play mode             |



## Showcase


https://github.com/kevinsadi/lei3d/assets/34428034/81e8b323-b533-4b35-bc26-2e06e315e536


https://github.com/kevinsadi/lei3d/assets/34428034/a4ee5ab8-b307-4df5-ac46-c90f608b7a95



https://github.com/kevinsadi/lei3d/assets/34428034/7f50b9ca-b884-4462-afe0-7b81eb598362


![image](https://github.com/kevinsadi/lei3d/assets/34428034/a5e09d77-9d18-467e-a7a4-339d13dfaf77)

![flower](https://github.com/kevinsadi/lei3d/assets/34428034/8f9e54b2-131e-4795-9572-e02b30ef7543)
