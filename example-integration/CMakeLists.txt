cmake_minimum_required(VERSION 3.20)
project(ExampleApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch CwAPI3D.Ex using FetchContent
include(FetchContent)

FetchContent_Declare(
    CwAPI3DEx
    GIT_REPOSITORY https://github.com/YOUR_USERNAME/CwAPI3D.Ex.git
    GIT_TAG        main  # Change to specific version tag for production
    GIT_SHALLOW    TRUE  # Faster download
)

# Configure options for the fetched library
set(BUILD_TESTING OFF CACHE BOOL "Don't build CwAPI3D.Ex tests" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Don't build CwAPI3D.Ex examples" FORCE)

# Make the library available
FetchContent_MakeAvailable(CwAPI3DEx)

# Create your executable
add_executable(example_app src/main.cpp)

# Link against CwAPI3D.Ex libraries
target_link_libraries(example_app PRIVATE
    geometry3d
    composite
)

# Optional: Enable warnings
if(MSVC)
    target_compile_options(example_app PRIVATE /W4)
else()
    target_compile_options(example_app PRIVATE -Wall -Wextra -Wpedantic)
endif()

