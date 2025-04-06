cmake_minimum_required(VERSION 3.29)
project("Athena OpenGL Engine")

set(CMAKE_CXX_STANDARD 17)

# External libraries
add_subdirectory(external/fmt)
add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)

set(ImGuiSource
    "external/imgui/imconfig.h"
    "external/imgui/imgui.cpp"
    "external/imgui/imgui.h"
    "external/imgui/imgui_demo.cpp"
    "external/imgui/imgui_draw.cpp"
    "external/imgui/imgui_internal.h"
    "external/imgui/imgui_tables.cpp"
    "external/imgui/imgui_widgets.cpp"
    "external/imgui/imstb_rectpack.h"
    "external/imgui/imstb_textedit.h"
    "external/imgui/imstb_truetype.h"
    "external/imgui/backends/imgui_impl_glfw.h"
    "external/imgui/backends/imgui_impl_glfw.cpp"
    "external/imgui/backends/imgui_impl_opengl3.h"
    "external/imgui/backends/imgui_impl_opengl3.cpp"
)

set(tinygltfSource
    "external/tinygltf/stb_image.h"
    "external/tinygltf/stb_image_write.h"
    "external/tinygltf/json.hpp"
    "external/tinygltf/tiny_gltf.h"
    "external/tinygltf/tiny_gltf.cc"
)

# Create executable
file(GLOB_RECURSE SOURCE_FILES
    "src/*.h"
    "src/*.cpp"
)
add_executable(Athena ${SOURCE_FILES} ${ImGuiSource} ${tinygltfSource})

# Link libraries
target_link_libraries(Athena PRIVATE
    fmt::fmt
    glad
    glfw
    glm
)

# Include directories
target_include_directories(Athena PRIVATE
    "src/"
    "external/imgui"
    "external/tinygltf"
)

# Check the build type and set preprocessor definitions
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
    target_compile_definitions(Athena PRIVATE DEBUG ATH_ENABLE_ASSERTS)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
    target_compile_definitions(Athena PRIVATE RELEASE)
endif()

# Copy 'assets' folder to output folder
add_custom_target(copy_assets_folder ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets/Shaders
    ${PROJECT_BINARY_DIR}/assets/Shaders
    COMMENT "Copying 'assets' folder into binary directory"
)

add_dependencies(Athena copy_assets_folder)
