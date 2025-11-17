cmake_minimum_required(VERSION 3.20)

project(cwapi3dcpp-ex
    VERSION 0.1.0
    DESCRIPTION "Extension library for CwAPI3D providing geometry and composite object structures"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_TESTING "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

# Include directories
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Add subprojects
add_subdirectory(geometry3d)
add_subdirectory(composite)

# Testing
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation
install(EXPORT CwAPI3DExTargets
    FILE CwAPI3DExTargets.cmake
    NAMESPACE CwAPI3D::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CwAPI3DEx
)

# Create config file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CwAPI3DExConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/CwAPI3DExConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CwAPI3DEx
)

# Create version file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/CwAPI3DExConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install config files
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/CwAPI3DExConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/CwAPI3DExConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CwAPI3DEx
)
