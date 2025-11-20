project(utility)

add_library(${PROJECT_NAME} INTERFACE)

# Header-only helpers

target_include_directories(${PROJECT_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_library(CwAPI3D::utility ALIAS ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
        EXPORT CwAPI3DExTargets
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
