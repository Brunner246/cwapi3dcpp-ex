add_library(composite INTERFACE)

#include(GenerateExportHeader)
#generate_export_header(composite
#        BASE_NAME COMPOSITE
#        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/cwapi3d/composite/composite_export.h
#)

target_include_directories(composite
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(composite INTERFACE cxx_std_20)

# Link to geometry3d to ensure its include directories are available
target_link_libraries(composite INTERFACE geometry3d)

# Create alias for FetchContent usage
add_library(CwAPI3D::composite ALIAS composite)

# Installation
install(TARGETS composite
        EXPORT CwAPI3DExTargets
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Note: composite is header-only and doesn't generate an export header
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/cwapi3d/composite/composite_export.h
#         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cwapi3d/composite
# )
