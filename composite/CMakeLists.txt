add_library(composite
    src/Component.cpp
    src/ComponentVisitor.cpp
)

# Generate export header for Windows DLL support
include(GenerateExportHeader)
generate_export_header(composite
    BASE_NAME COMPOSITE
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/cwapi3d/composite/composite_export.h
)

target_include_directories(composite
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(composite PUBLIC cxx_std_20)

# Set library properties
set_target_properties(composite PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME "cwapi3d_composite"
)

# Installation
install(TARGETS composite
    EXPORT CwAPI3DExTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/cwapi3d/composite/composite_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cwapi3d/composite
)
