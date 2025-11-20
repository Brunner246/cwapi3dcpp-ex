add_library(composite INTERFACE)

target_include_directories(composite
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(composite INTERFACE cxx_std_20)

#target_link_libraries(composite INTERFACE geometry3d)

add_library(CwAPI3D::composite ALIAS composite)

# Installation
install(TARGETS composite
        EXPORT CwAPI3DExTargets
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

