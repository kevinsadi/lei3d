cmake_minimum_required(VERSION 3.14)
project(lei3d)

file(GLOB_RECURSE GAME_SOURCE
        "*.cpp"
        )

file(GLOB_RECURSE GAME_HEADER
        "*.hpp"
        )

add_library(skylei_lib
        ${GAME_HEADER}
        ${GAME_SOURCE}
        )

target_include_directories(skylei_lib PUBLIC
        .
        ../engine
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
    set_target_properties(skylei_lib PROPERTIES OUTPUT_NAME libskylei)
endif()
