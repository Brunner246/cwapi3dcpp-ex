project(geometry3d)

add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME}
        PRIVATE
        src/Point3D.cpp
        src/Vec3D.cpp
        src/Plane3D.cpp
        src/CoordinateFrame3D.cpp
        src/BoundingBox.cpp
)



include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
        BASE_NAME GEOMETRY3D
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/cwapi3d/geometry/geometry3d_export.h
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

add_library(CwAPI3D::geometry3d ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        OUTPUT_NAME "cwapi3d_geometry3d"
)

install(TARGETS ${PROJECT_NAME}
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
