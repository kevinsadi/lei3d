cmake_minimum_required(VERSION 3.14)
project(lei3d)

add_subdirectory(lib)

add_custom_target(copy_data ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)   

target_include_directories(imgui PUBLIC include)
target_include_directories(spdlog PUBLIC include)

# set(sourceFiles
#     main.cpp
#     Shader.cpp
#     PCGHelpers.cpp
#     FlyCamera.cpp
#     Mesh.cpp
#     Model.cpp
#     Engine.cpp
#     SkyBox.cpp
#     Physics.cpp
#     Entity.cpp
#     Log.cpp	
#     GLDebug.cpp
# )

# set(headerFiles
#     include/engine/Shader.hpp
#     include/engine/FlyCamera.hpp
#     include/engine/Engine.hpp
#     include/engine/Mesh.hpp
#     include/engine/Model.hpp
#     include/engine/SkyBox.hpp
#     include/engine/Entity.hpp
#     include/pcg/PCGHelpers.hpp
#     include/physics/Physics.hpp
#     include/util/Log.hpp
#     include/util/GLDebug.hpp
# )

file(GLOB sourceFiles
     "*.cpp"
)

file(GLOB headerFiles
    "include/*/*.hpp"
    "include/*/*/*.hpp"
    "include/*/*/*/*.hpp"
)

add_executable(lei3d ${sourceFiles} ${headerFiles})
add_dependencies(lei3d copy_data)

find_package(OpenGL REQUIRED)

target_include_directories(glfw PUBLIC include)
target_include_directories(glad PUBLIC include)
target_include_directories(imgui PUBLIC include)
target_include_directories(spdlog PUBLIC include)

target_include_directories(lei3d PRIVATE include)
target_link_libraries(lei3d PRIVATE glfw glad imgui glm stb_image spdlog assimp libbullet3)
