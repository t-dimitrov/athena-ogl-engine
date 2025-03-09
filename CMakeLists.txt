cmake_minimum_required(VERSION 3.3)
project("Athena OpenGL Engine")

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
    src/Main.cpp
)

set(HEADER_FILES

)

# Create executable
add_executable(Athena
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

# Include directories
target_include_directories(Athena PRIVATE
    src/
)

# Copy 'assets' folder to output folder
add_custom_target(copy_assets_folder ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets
    ${PROJECT_BINARY_DIR}/assets
    COMMENT "Copying 'assets' folder into binary directory"
)

add_dependencies(Athena copy_assets_folder)
