find_package(GTest CONFIG REQUIRED)

add_executable(test_model
        test_EntityId.cpp
        test_DomainEntities.cpp
        test_ComponentRegistry.cpp
        test_FloorAssignment.cpp
        test_ElementAssignment.cpp
        test_ModelContext.cpp
        test_AssignmentService.cpp
        test_ElementHierarchy.cpp
)

target_link_libraries(test_model
        PRIVATE
        CwAPI3D::Model
        GTest::gtest_main
)

add_test(NAME test_model COMMAND test_model)
