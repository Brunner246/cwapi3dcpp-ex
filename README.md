# CwAPI3D.Ex

[![CI](https://github.com/Brunner246/cwapi3dcpp-ex/actions/workflows/ci.yml/badge.svg)](https://github.com/Brunner246/cwapi3dcpp-ex/actions/workflows/ci.yml)
[![Pulls](https://img.shields.io/github/issues-pr/Brunner246/cwapi3dcpp-ex)](https://github.com/Brunner246/cwapi3dcpp-ex/pulls)

Extension library for [CwAPI3D](https://github.com/cwapi3d/cwapi3dcpp) providing advanced 3D geometry utilities and composite object structures.

## Features

### Geometry3D Module
- **Point3D** - 3D point with distance calculations and arithmetic operations
- **Vec3D** - 3D vector with dot product, cross product, normalization
- **Plane3D** - Plane representation with point projection and distance calculations
- **CoordinateFrame3D** - Local/world coordinate transformations with orthonormal axes
- **AABB** - Axis-Aligned Bounding Box for efficient spatial queries
- **BoundingBox** - Oriented bounding box support
- **KDTree** - K-dimensional tree for nearest neighbor searches

### Composite Module
- **Component** - Abstract base class for composite pattern
- **Composite** - Container for child components with hierarchy management
- **Leaf** - Terminal component in the hierarchy
- **Visitor Pattern** - Traverse and operate on component hierarchies

## Requirements

- C++20 or later
- CMake 3.20 or later
- A modern C++ compiler (MSVC 2019+, GCC 11+, Clang 14+)

## Installation

### Option 1: CMake FetchContent (Recommended)

The easiest way to integrate CwAPI3D.Ex into your CMake project:

```cmake
include(FetchContent)

FetchContent_Declare(
    CwAPI3DEx
    GIT_REPOSITORY https://github.com/Brunner246/CwAPI3D.Ex.git
    GIT_TAG        v0.1.0  # or main for latest
)

# Optional: disable tests and examples
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(CwAPI3DEx)

# Link to your target
target_link_libraries(your_app PRIVATE geometry3d composite)
```

📖 See [FETCHCONTENT_USAGE.md](FETCHCONTENT_USAGE.md) for detailed FetchContent integration guide.

### Option 2: Building from Source

```bash
# Clone the repository
git clone https://github.com/Brunner246/cwapi3dcpp-ex.git
cd cwapi3d-ex

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run tests
cd build
ctest

# Install
cmake --install build
```

### Option 3: Using vcpkg

# TODO

Add to your `vcpkg.json`:

```json
{
  "dependencies": [
    "cwapi3dcpp-ex"
  ]
}
```

Or install directly:

```bash
vcpkg install cwapi3dcpp-ex
```

### Option 4: Using find_package (for installed version)

If you've installed cwapi3dcpp.ex via CMake or vcpkg:

```cmake
find_package(cwapi3dcpp-ex 0.1.0 REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE 
    CwAPI3D::geometry3d
    CwAPI3D::composite
)
```

## Usage Examples

### Geometry3D

```cpp
#include <cwapi3d/geometry/Point3D.h>
#include <cwapi3d/geometry/Vec3D.h>
#include <cwapi3d/geometry/CoordinateFrame3D.h>
#include <cwapi3d/geometry/KDTree.h>

using namespace cwapi3d::geometry;

// Basic geometry operations
Point3D p1(1.0, 2.0, 3.0);
Point3D p2(4.0, 5.0, 6.0);
double distance = p1.distanceTo(p2);

Vec3D v1 = Vec3D::unitX();
Vec3D v2 = Vec3D::unitY();
Vec3D cross = v1.cross(v2);  // Returns unit Z

// Coordinate frame transformations
CoordinateFrame3D frame(
    Point3D(10.0, 0.0, 0.0),
    Vec3D::unitX(),
    Vec3D::unitY(),
    Vec3D::unitZ()
);

Point3D worldPoint(15.0, 5.0, 3.0);
Point3D localPoint = frame.transformToLocal(worldPoint);

// KD-Tree for nearest neighbor search
KDTree<int> tree;
std::vector<std::pair<Point3D, int>> points = {
    {{0.0, 0.0, 0.0}, 1},
    {{1.0, 1.0, 1.0}, 2},
    {{2.0, 2.0, 2.0}, 3}
};
tree.build(points);

auto nearest = tree.findNearest(Point3D(0.5, 0.5, 0.5));
auto inRadius = tree.findInRadius(Point3D(1.0, 1.0, 1.0), 2.0);
```

### Composite Pattern

```cpp
#include <cwapi3d/composite/Component.h>
#include <cwapi3d/composite/ComponentVisitor.h>

using namespace cwapi3d::composite;

// Create a hierarchy
auto root = std::make_unique<Composite>("Building");

auto floor1 = std::make_unique<Composite>("Floor 1");
floor1->addChild(std::make_unique<Leaf>("Room 101"));
floor1->addChild(std::make_unique<Leaf>("Room 102"));

auto floor2 = std::make_unique<Composite>("Floor 2");
floor2->addChild(std::make_unique<Leaf>("Room 201"));

root->addChild(std::move(floor1));
root->addChild(std::move(floor2));

// Traverse the hierarchy
root->traverse([](const Component& comp) {
    std::cout << comp.name() << std::endl;
});

// Use visitor pattern
PrintVisitor printer(std::cout);
root->accept(printer);
```

## Integration with CwAPI3D

This library is designed to complement the [CwAPI3D](https://github.com/cwapi3d/cwapi3dcpp) interface. Use it to:

- Perform geometric calculations on CwAPI3D objects
- Build spatial indices for efficient element queries
- Create hierarchical object structures for complex assemblies
- Transform coordinates between different reference frames

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes with tests
4. Ensure all tests pass
5. Submit a pull request

## Design Principles

This project follows clean code and clean architecture principles:

- **SOLID Principles** - Single responsibility, open/closed, dependency inversion
- **Modern C++** - Uses C++17 features, smart pointers, move semantics
- **Type Safety** - Strong typing with minimal implicit conversions
- **Const Correctness** - Proper use of const and noexcept
- **Memory Safety** - RAII, no raw pointers in public interfaces
- **Testability** - Designed for unit testing and dependency injection

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Authors

- [@Brunner246](https://github.com/Brunner246)

## Acknowledgments

- [CwAPI3D](https://github.com/cwapi3d/cwapi3dcpp) - The Cadwork 3D API
- Inspired by modern C++ geometry libraries and design patterns

## Links

[//]: # (- [Documentation]&#40;https://github.com/cwapi3d/cwapi3d-ex/wiki&#41;)

[//]: # (- [Issue Tracker]&#40;https://github.com/cwapi3d/cwapi3d-ex/issues&#41;)
- [CwAPI3D Main Project](https://github.com/cwapi3d/cwapi3dcpp)
