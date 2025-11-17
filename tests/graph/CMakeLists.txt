find_package(GTest CONFIG REQUIRED)

add_executable(graph_tests
    graph_tests.cpp
    graph_algorithms_tests.cpp
    relation_graph_tests.cpp
)

target_link_libraries(graph_tests
    PRIVATE
        graph
        GTest::gtest
        GTest::gtest_main
)

# Discover tests for CTest
include(GoogleTest)
gtest_discover_tests(graph_tests)

