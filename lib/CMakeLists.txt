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

FetchContent_GetProperties(assimp)
if(NOT assimp_POPULATED)

    FetchContent_Populate(assimp)
    message("Fetching assimp")

    add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR}) # THIS DOESN'T FEEL RIGHT

    FetchContent_MakeAvailable(assimp)
endif()

#-------------------------------------------------------------------------------------

# Fetch Bullet3
FetchContent_Declare(
  bullet
  GIT_REPOSITORY https://github.com/bulletphysics/bullet3
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE
  GIT_TAG        master
)

set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(BUILD_PYBULLET OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)

FetchContent_GetProperties(bullet)
if(NOT bullet_POPULATED)
  FetchContent_Populate(bullet)
  add_subdirectory(${bullet_SOURCE_DIR} ${bullet_BINARY_DIR})
  target_include_directories(Bullet3Collision PUBLIC ${BULLET_PHYSICS_SOURCE_DIR}/src)
  target_include_directories(Bullet3Common PUBLIC ${BULLET_PHYSICS_SOURCE_DIR}/src)
  target_include_directories(Bullet3Dynamics PUBLIC ${BULLET_PHYSICS_SOURCE_DIR}/src)
  target_include_directories(Bullet3Geometry PUBLIC ${BULLET_PHYSICS_SOURCE_DIR}/src)
  target_include_directories(Bullet3OpenCL_clew PUBLIC ${BULLET_PHYSICS_SOURCE_DIR}/src)
  target_include_directories(Bullet2FileLoader PUBLIC ${BULLET_PHYSICS_SOURCE_DIR}/src)
  target_include_directories(BulletCollision PUBLIC ${BULLET_PHYSICS_SOURCE_DIR}/src)
  target_include_directories(BulletDynamics PUBLIC ${BULLET_PHYSICS_SOURCE_DIR}/src)
  target_include_directories(BulletInverseDynamics PUBLIC ${BULLET_PHYSICS_SOURCE_DIR}/src)
  target_include_directories(BulletSoftBody PUBLIC ${BULLET_PHYSICS_SOURCE_DIR}/src)
  target_include_directories(LinearMath PUBLIC ${BULLET_PHYSICS_SOURCE_DIR}/src)
endif()






