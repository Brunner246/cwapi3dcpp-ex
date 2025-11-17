find_package(GTest CONFIG REQUIRED)

add_executable(geometry3d_tests
    test_Point3D.cpp
    test_Vec3D.cpp
    test_CoordinateFrame3D.cpp
    test_KDTree.cpp
    test_BoundingBox.cpp
)


target_link_libraries(geometry3d_tests PRIVATE 
    geometry3d
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(geometry3d_tests)
