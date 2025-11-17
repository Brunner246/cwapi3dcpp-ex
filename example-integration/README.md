# Example Integration Project

This directory contains a complete example of how to use CwAPI3D.Ex in your CMake project via FetchContent.

## Structure

```
example-integration/
├── CMakeLists.txt        # Main CMake configuration with FetchContent
├── src/
│   └── main.cpp          # Example application using geometry3d and composite
└── README.md            # This file
```

## How to Build

```bash
cd example-integration
cmake -B build
cmake --build build
./build/example_app   # On Linux/macOS
# or
.\build\Debug\example_app.exe   # On Windows
```

## What This Demonstrates

1. How to fetch CwAPI3D.Ex using FetchContent
2. How to configure build options (disable tests/examples)
3. How to link against geometry3d and composite libraries
4. Real-world usage of the library APIs

## CMakeLists.txt Breakdown

```cmake
# Minimum required CMake version
cmake_minimum_required(VERSION 3.20)

# Your project
project(ExampleApp VERSION 1.0.0)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch CwAPI3D.Ex
include(FetchContent)
FetchContent_Declare(
    CwAPI3DEx
    GIT_REPOSITORY https://github.com/YOUR_USERNAME/CwAPI3D.Ex.git
    GIT_TAG        main
)

# Don't build tests/examples from fetched library
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(CwAPI3DEx)

# Your executable
add_executable(example_app src/main.cpp)

# Link against CwAPI3D.Ex libraries
target_link_libraries(example_app PRIVATE 
    geometry3d
    composite
)
```

## Notes

- This example uses `GIT_TAG main` to always fetch the latest version
- For production code, use a specific version tag: `GIT_TAG v0.1.0`
- The libraries are automatically built and linked
- No manual installation required

