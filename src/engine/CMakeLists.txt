cmake_minimum_required(VERSION 3.14)
project(lei3d)

file(GLOB_RECURSE ENGINE_SOURCE
        "*.cpp"
        )

file(GLOB_RECURSE ENGINE_HEADER
        "*.hpp"
        )

# ----------------------------------------------------
# lib for editor

add_library(leieditor_lib
        ${ENGINE_HEADER}
        ${ENGINE_SOURCE}
        )

add_dependencies (leieditor_lib glad)

target_include_directories(leieditor_lib PUBLIC
        .
        ${GLFW_INCLUDE}
        ${GLAD_INCLUDE}
        ${IMGUI_INCLUDE}
        ${SPDLOG_INCLUDE}
        ${BULLET_INCLUDE}
        ${MINIAUDIO_INCLUDE}
        ${STB_INCLUDE}
        ${ASSIMP_INCLUDE}
        ${GLM_INCLUDE}
        )

target_compile_definitions(leieditor_lib PRIVATE -DEDITOR)

# ----------------------------------------------------
# lib for game

add_library(lei3d_lib
        ${ENGINE_HEADER}
        ${ENGINE_SOURCE}
        )

add_dependencies (lei3d_lib glad)

target_include_directories(lei3d_lib PUBLIC
        .
        ${GLFW_INCLUDE}
        ${GLAD_INCLUDE}
        ${IMGUI_INCLUDE}
        ${SPDLOG_INCLUDE}
        ${BULLET_INCLUDE}
        ${MINIAUDIO_INCLUDE}
        ${STB_INCLUDE}
        ${ASSIMP_INCLUDE}
        ${GLM_INCLUDE}
)

if (WIN32)
    # Avoid a name clash when building on Visual Studio
    set_target_properties(leieditor_lib PROPERTIES OUTPUT_NAME libleieditor)
    set_target_properties(lei3d_lib PROPERTIES OUTPUT_NAME liblei3d)
endif()
