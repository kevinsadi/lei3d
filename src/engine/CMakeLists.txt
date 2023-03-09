cmake_minimum_required(VERSION 3.14)
project(kek3d)

add_subdirectory(lib)

add_custom_target(copy_data ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)   

target_include_directories(imgui PUBLIC include)
target_include_directories(spdlog PUBLIC include)

set(sourceFiles
    main.cpp
    Shader.cpp
)
set(headerFiles
    include/engine/Shader.hpp
)
add_executable(kek3d ${sourceFiles} ${headerFiles})
add_dependencies(kek3d copy_data)

find_package(OpenGL REQUIRED)

target_include_directories(glfw PUBLIC include)
target_include_directories(glad PUBLIC include)
target_include_directories(imgui PUBLIC include)
target_include_directories(spdlog PUBLIC include)

target_include_directories(kek3d PRIVATE include)
target_link_libraries(kek3d PRIVATE glad glfw imgui glm cgltf stb_image spdlog)
