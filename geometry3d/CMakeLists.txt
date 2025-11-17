add_library(geometry3d
    src/Point3D.cpp
    src/Vec3D.cpp
    src/Plane3D.cpp
    src/CoordinateFrame3D.cpp
    src/BoundingBox.cpp
)

# Generate export header for Windows DLL support
include(GenerateExportHeader)
generate_export_header(geometry3d
    BASE_NAME GEOMETRY3D
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/cwapi3d/geometry/geometry3d_export.h
)

target_include_directories(geometry3d
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(geometry3d PUBLIC cxx_std_20)

# Set library properties
set_target_properties(geometry3d PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME "cwapi3d_geometry3d"
)

# Installation
install(TARGETS geometry3d
    EXPORT CwAPI3DExTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/cwapi3d/geometry/geometry3d_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cwapi3d/geometry
)
