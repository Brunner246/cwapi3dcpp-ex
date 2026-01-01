project(model)

add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME}
#        PUBLIC
#        include/cwapi3d/model/EntityId.h
#        include/cwapi3d/model/Element.h
#        include/cwapi3d/model/Floor.h
#        include/cwapi3d/model/Building.h
#        include/cwapi3d/model/BRepGeometry.h
#        include/cwapi3d/model/PropertySet.h
#        include/cwapi3d/model/ComponentRegistry.h
#        include/cwapi3d/model/FloorAssignment.h
#        include/cwapi3d/model/BuildingAssignment.h
#        include/cwapi3d/model/ModelContext.h
#        include/cwapi3d/model/AssignmentService.h

        PRIVATE
        src/EntityId.cpp
        src/Element.cpp
        src/BuildingStorey.cpp
        src/Building.cpp
        src/ComponentRegistry.cpp
        src/BuildingStoreyAssignment.cpp
        src/BuildingAssignment.cpp
        src/ElementAssignment.cpp
        src/ModelContext.cpp
        src/AssignmentService.cpp

)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC  /wd4251 /wd4275)
endif()

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
        BASE_NAME CWAPI3D_MODEL
        EXPORT_FILE_NAME include/cwapi3d/model/cwapi3d_model_export.h
)

install(TARGETS ${PROJECT_NAME}
        EXPORT CwAPI3DExTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/cwapi3d
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/cwapi3d/model/cwapi3d_model_export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cwapi3d/model
)

add_library(CwAPI3D::Model ALIAS ${PROJECT_NAME})
