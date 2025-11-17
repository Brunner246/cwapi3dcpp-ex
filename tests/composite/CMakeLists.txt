find_package(GTest CONFIG REQUIRED)

add_executable(composite_tests
    test_Component.cpp
    test_GenericComponent.cpp
)

target_link_libraries(composite_tests PRIVATE 
    composite
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(composite_tests)
