cmake_minimum_required(VERSION 3.14)
project(lei3d)

add_subdirectory(lib)

add_custom_target(copy_data ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data)   

target_include_directories(imgui PUBLIC include)
target_include_directories(spdlog PUBLIC include)

file(GLOB_RECURSE sourceFiles
    "*.cpp"
)

file(GLOB_RECURSE headerFiles
    "*.hpp"
)

add_executable(lei3d ${sourceFiles} ${headerFiles})
add_dependencies(lei3d copy_data)

find_package(OpenGL REQUIRED)

include_directories(./)
target_include_directories(glfw PUBLIC include)
target_include_directories(glad PUBLIC include)
target_include_directories(imgui PUBLIC include)
target_include_directories(spdlog PUBLIC include)

target_include_directories(lei3d PRIVATE include)
target_link_libraries(lei3d PRIVATE glfw glad imgui glm stb_image spdlog assimp libbullet3)
