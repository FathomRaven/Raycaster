cmake_minimum_required(VERSION 3.10)

# Define project name and language
project(Raycaster CXX)

# Add all source files in src/ to the sources list
file(GLOB SOURCES "src/*.cpp")

# Add all header files in include/ to the headers list
file(GLOB HEADERS "include/*.hpp")

# Add executable target named "Raycaster" using the sources list
add_executable(Raycaster ${SOURCES} ${HEADERS})

set_target_properties(Raycaster PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "bin"
)

find_package(SDL2 REQUIRED)
target_link_libraries(Raycaster PRIVATE SDL2::SDL2)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/bin/Raycaster
    DEPENDS Raycaster
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Set target include directories
target_include_directories(Raycaster PRIVATE include)