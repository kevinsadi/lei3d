include(FetchContent)

#-------------------------------------------------------------------------------------

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.10.0
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

message("Fetching spdlog")
FetchContent_MakeAvailable(spdlog)

#-------------------------------------------------------------------------------------

FetchContent_Declare(
    tracy
    GIT_REPOSITORY  https://github.com/wolfpld/tracy.git
    GIT_TAG         master
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
)

set(TRACY_ENABLE ON CACHE BOOL "Enable profiling")
#set(TRACY_NO_SYSTEM_TRACING ON CACHE BOOL "Disable System Tracing")
set(TRACY_ONLY_IPV4 ON CACHE BOOL "IPv4 only")
option(TRACY_ENABLE "Enable profiling" ON)
#option(TRACY_NO_SYSTEM_TRACING "Disable System Tracing" ON)
option(TRACY_ONLY_IPV4 "IPv4 only" ON)
message("Fetching tracy")
FetchContent_MakeAvailable(tracy)

#-------------------------------------------------------------------------------------

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG        v1.88
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    message("Fetching imgui")
    FetchContent_Populate(imgui)

    add_library(imgui
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)

    target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${glfw_SOURCE_DIR}/include)

    target_link_libraries(imgui PRIVATE glfw)
endif ()

#-------------------------------------------------------------------------------------

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG        master
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

message("Fetching glm")
FetchContent_MakeAvailable(glm)
target_compile_definitions(glm INTERFACE GLM_FORCE_SILENT_WARNINGS)

#-------------------------------------------------------------------------------------

FetchContent_Declare(
    stb_image
    GIT_REPOSITORY  https://github.com/nothings/stb.git
    GIT_TAG         master
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
)
FetchContent_GetProperties(stb_image)
if(NOT stb_image_POPULATED)
    FetchContent_Populate(stb_image)
    message("Fetching stb_image")

    add_library(stb_image INTERFACE ${stb_image_SOURCE_DIR}/stb_image.h)
    target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR})
endif()

#-------------------------------------------------------------------------------------

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG        v5.2.5
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "" FORCE)

FetchContent_GetProperties(assimp)
if(NOT assimp_POPULATED)

    FetchContent_Populate(assimp)
    message("Fetching assimp")

    add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR}) # THIS DOESN'T FEEL RIGHT

    FetchContent_MakeAvailable(assimp)
endif()

#-------------------------------------------------------------------------------------

# Fetch Bullet3 (it's gonna be a bit a different than the other libraries bc bullet3 is weird)
# https://github.com/potatoengine/libbullet3-cmake -> thank you to Sean Middleditch

FetchContent_Declare(
  bullet
  GIT_REPOSITORY https://github.com/bulletphysics/bullet3
  GIT_TAG        ebe1916b90acae8b13cd8c6b637d8327cdc64e94 # 3.1.7
)

FetchContent_GetProperties(bullet)
if(NOT bullet_POPULATED)
    FetchContent_Populate(bullet)
    message("Fetching bullet3")

    # Bullet is broken into sub-"libraries"; we'll automate handling of these
    set(BULLET_SUBMODULES
        Bullet3Collision
        Bullet3Common
        Bullet3Dynamics
        Bullet3Geometry
        BulletDynamics
        BulletCollision
        BulletInverseDynamics
        BulletSoftBody
        LinearMath
    )

    # Find all relevant sources in upstream repo
    # We don't use the bc*All.cpp files, as they
    # fail to build, while the individual sources
    # build correctly
    set(BULLET3_SOURCES
        "${bullet_SOURCE_DIR}/src/btBulletCollisionCommon.h"
        "${bullet_SOURCE_DIR}/src/btBulletDynamicsCommon.h"
    )
    foreach(SUB ${BULLET_SUBMODULES})
        file(GLOB_RECURSE SUBMODULE_SOURCES
            "${bullet_SOURCE_DIR}/src/${SUB}/*.h"
            "${bullet_SOURCE_DIR}/src/${SUB}/*.cpp"
        )
        list(APPEND BULLET3_SOURCES ${SUBMODULE_SOURCES})
    endforeach()

    # CPack support
    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    include(CPack)

    # Build the main libbullet3 library
    set(BULLET3_LIBRARY_TYPE "STATIC")
    add_library(libbullet3 ${BULLET3_LIBRARY_TYPE} ${BULLET3_SOURCES})
    set_target_properties(libbullet3 PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS 1
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION}
    )
    target_include_directories(libbullet3
        SYSTEM INTERFACE
            $<BUILD_INTERFACE:${bullet_SOURCE_DIR}/src>
            $<INSTALL_INTERFACE:include/Bullet3>
        PRIVATE
            ${bullet_SOURCE_DIR}/src
    )

    # Silence the many warnings in the libbullet source code;
    # we're not the maintainers of the code, so these warnings
    # aren't meaningful to us; they just clog up build output
    # when building Bullet as a subdirectory
    target_compile_options(libbullet3 PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/w>
        $<$<CXX_COMPILER_ID:GNU,Clang,Intel>:-w>
    )

    # Installation
    install(
        TARGETS libbullet3
        DESTINATION lib
        EXPORT Bullet3Targets
    )

    install(FILES
        "${bullet_SOURCE_DIR}/src/btBulletCollisionCommon.h"
        "${bullet_SOURCE_DIR}/src/btBulletDynamicsCommon.h"
        DESTINATION "include/Bullet3"
    )
    foreach(SUB ${BULLET_SUBMODULES})
        install(
            DIRECTORY "${bullet_SOURCE_DIR}/src/${SUB}"
            DESTINATION "include/Bullet3"
            FILES_MATCHING PATTERN "*.h"
        )
    endforeach()

    install(
        EXPORT Bullet3Targets
        FILE Bullet3Targets.cmake
        DESTINATION "lib/cmake/Bullet3"
    )

    install(
        EXPORT Bullet3Targets
        FILE Bullet3Targets.cmake
        DESTINATION "lib/cmake/Bullet3"
    )

    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/Bullet3Config.cmake"
        INSTALL_DESTINATION "lib/cmake/Bullet3"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/Bullet3ConfigVersion.cmake"
        VERSION "${libbullet3_VERSION_MAJOR}.${libbullet3_VERSION_MINOR}"
        COMPATIBILITY AnyNewerVersion
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/Bullet3Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/Bullet3ConfigVersion.cmake"
        DESTINATION lib/cmake/Bullet3
    )

    export(EXPORT Bullet3Targets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/Bullet3Targets.cmake"
    )
endif()