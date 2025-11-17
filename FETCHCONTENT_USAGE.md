# Using cwapi3dcpp.ex with CMake FetchContent

This document explains how to integrate cwapi3dcpp.ex into your CMake project using FetchContent.

## Overview

cwapi3dcpp.ex is designed to work seamlessly with CMake's FetchContent module, allowing you to download and build it as
part of your project's configuration step without requiring manual installation.

## Basic Usage

Add the following to your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyProject)

include(FetchContent)

FetchContent_Declare(
        CwAPI3DEx
        GIT_REPOSITORY https://github.com/Brunner246/cwapi3dcpp.ex.git
        GIT_TAG main  # or a specific tag/commit
)

# Optional: Configure cwapi3dcpp.ex options before making it available
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)  # Don't build cwapi3dcpp.ex tests
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)  # Don't build cwapi3dcpp.ex examples

FetchContent_MakeAvailable(CwAPI3DEx)

# Now you can link against the libraries
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE
        geometry3d
        composite
)
```

## Using Specific Versions

### Using a Git Tag

```cmake
FetchContent_Declare(
        CwAPI3DEx
        GIT_REPOSITORY https://github.com/Brunner246/cwapi3dcpp.ex.git
        GIT_TAG v0.1.0  # Specific release tag
)
```

### Using a Git Commit Hash

```cmake
FetchContent_Declare(
        CwAPI3DEx
        GIT_REPOSITORY https://github.com/Brunner246/cwapi3dcpp.ex.git
        GIT_TAG abc123def456  # Specific commit hash
)
```

### Using a Git Branch

```cmake
FetchContent_Declare(
        CwAPI3DEx
        GIT_REPOSITORY https://github.com/Brunner246/cwapi3dcpp.ex.git
        GIT_TAG develop  # Track a specific branch
)
```

## Advanced Configuration

### With Custom Options

```cmake
include(FetchContent)

# Set options before fetching
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Skip tests" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Skip examples" FORCE)

FetchContent_Declare(
        CwAPI3DEx
        GIT_REPOSITORY https://github.com/Brunner246/cwapi3dcpp.ex.git
        GIT_TAG main
        GIT_SHALLOW TRUE  # Shallow clone for faster download
)

FetchContent_MakeAvailable(CwAPI3DEx)
```

### Using Only Specific Components

```cmake
include(FetchContent)

FetchContent_Declare(
        CwAPI3DEx
        GIT_REPOSITORY https://github.com/Brunner246/cwapi3dcpp.ex.git
        GIT_TAG main
)

FetchContent_MakeAvailable(CwAPI3DEx)

# Link only the geometry3d library
add_executable(my_geometry_app main.cpp)
target_link_libraries(my_geometry_app PRIVATE geometry3d)

# Or link only the composite library
add_executable(my_composite_app other_main.cpp)
target_link_libraries(my_composite_app PRIVATE composite)
```

## Complete Example Project

Here's a complete example of a project using cwapi3dcpp.ex:

**CMakeLists.txt:**

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyCADApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch cwapi3dcpp.ex
FetchContent_Declare(
        CwAPI3DEx
        GIT_REPOSITORY https://github.com/Brunner246/cwapi3dcpp.ex.git
        GIT_TAG v0.1.0
        GIT_SHALLOW TRUE
)

# Don't build tests and examples from the fetched library
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(CwAPI3DEx)

# Your application
add_executable(my_app
        src/main.cpp
        src/geometry_processor.cpp
)

target_link_libraries(my_app PRIVATE
        geometry3d
        composite
)

# Your application can now use all headers from cwapi3dcpp.ex
# #include "cwapi3d/geometry/Point3D.h"
# #include "cwapi3d/composite/Component.h"
```

**src/main.cpp:**

```cpp
#include <iostream>
#include "cwapi3d/geometry/Point3D.h"
#include "cwapi3d/geometry/Vec3D.h"
#include "cwapi3d/composite/Component.h"

int main() {
    using namespace cwapi3d::geometry;
    using namespace cwapi3d::composite;
    
    // Use geometry
    Point3D p1(1.0, 2.0, 3.0);
    Point3D p2(4.0, 5.0, 6.0);
    double dist = p1.distanceTo(p2);
    std::cout << "Distance: " << dist << std::endl;
    
    // Use composite
    auto root = std::make_unique<Composite>("Building");
    root->addChild(std::make_unique<Leaf>("Room1"));
    std::cout << "Children: " << root->childCount() << std::endl;
    
    return 0;
}
```

## Using with vcpkg Dependencies

If your project also uses vcpkg, you can combine FetchContent with vcpkg:

**CMakeLists.txt:**

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)

# vcpkg toolchain should be set via command line or CMakePresets.json
# cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

include(FetchContent)

FetchContent_Declare(
        CwAPI3DEx
        GIT_REPOSITORY https://github.com/Brunner246/cwapi3dcpp.ex.git
        GIT_TAG main
)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(CwAPI3DEx)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE geometry3d composite)
```

**vcpkg.json:**

```json
{
  "name": "my-project",
  "version": "1.0.0",
  "dependencies": [
    "gtest"
  ]
}
```

## Advantages of FetchContent

1. **No Manual Installation**: Users don't need to manually clone, build, and install cwapi3dcpp.ex
2. **Version Control**: Pin to specific commits, tags, or branches
3. **Reproducible Builds**: Everyone gets the same version automatically
4. **CI/CD Friendly**: Works seamlessly in automated build pipelines
5. **Cross-Platform**: Works on Windows, Linux, and macOS without changes

## Troubleshooting

### Issue: Slow downloads

**Solution**: Use `GIT_SHALLOW TRUE` for faster cloning:

```cmake
FetchContent_Declare(
        CwAPI3DEx
        GIT_REPOSITORY https://github.com/Brunner246/cwapi3dcpp.ex.git
        GIT_TAG main
        GIT_SHALLOW TRUE
)
```

### Issue: Option conflicts

**Solution**: Force options before `FetchContent_MakeAvailable`:

```cmake
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(CwAPI3DEx)
```

### Issue: Namespace collisions

**Solution**: cwapi3dcpp.ex uses proper namespaces (`cwapi3d::geometry`, `cwapi3d::composite`) and exported targets (
`geometry3d`, `composite`) to avoid conflicts.

## Alternative: Using find_package (for installed version)

If cwapi3dcpp.ex is already installed on the system, you can use `find_package` instead:

```cmake
find_package(CwAPI3DEx 0.1.0 REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE
        CwAPI3D::geometry3d
        CwAPI3D::composite
)
```

## Recommendations

1. **Use Git Tags**: For production code, always use specific version tags rather than branch names
2. **Disable Tests/Examples**: Set `BUILD_TESTING` and `BUILD_EXAMPLES` to `OFF` to speed up configuration
3. **Shallow Clones**: Use `GIT_SHALLOW TRUE` for faster downloads
4. **CMake Version**: Ensure you're using CMake 3.20 or higher
5. **Lock Versions**: Use `GIT_TAG` with commit hashes for fully reproducible builds

## License

When using cwapi3dcpp.ex via FetchContent, ensure you comply with the MIT license by including proper attribution in
your project.

