cmake_minimum_required(VERSION 3.14)
project(lei3d)

find_package(OpenGL REQUIRED)

add_subdirectory(ext)
add_subdirectory(engine)
add_subdirectory(editor)
add_subdirectory(game)

add_custom_target(copy_data ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)

#-------------------------------------------------------------------------------------
# Editor

add_executable(LeiEditor_exe editor.cpp
        ${EDITOR_SOURCE})
add_dependencies(LeiEditor_exe copy_data)

target_include_directories(LeiEditor_exe PRIVATE
        editor
        engine
        ${IMGUI_INCLUDE}
        )
target_link_libraries(LeiEditor_exe PRIVATE leieditor_lib glfw glad imgui glm stb_image miniaudio spdlog assimp libbullet3)

#-------------------------------------------------------------------------------------
# Game

add_executable(SkyLei_exe game.cpp
        ${SKYLEI_SOURCE})
add_dependencies(SkyLei_exe copy_data)

target_include_directories(SkyLei_exe PRIVATE
        game
        engine
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
target_link_libraries(SkyLei_exe PRIVATE lei3d_lib glfw glad imgui glm stb_image miniaudio spdlog assimp libbullet3)
