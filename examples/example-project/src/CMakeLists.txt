project(example_lib)

add_library(${PROJECT_NAME}
        SHARED
        library.cc
)

message(STATUS "Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

include(FetchContent)
FetchContent_Declare(
        cwapi3dcpp
        GIT_REPOSITORY https://github.com/cwapi3d/cwapi3dcpp.git
        GIT_TAG main
)

FetchContent_Declare(
        CwAPI3DEx
        GIT_REPOSITORY https://github.com/Brunner246/cwapi3dcpp-ex.git
        GIT_TAG main
)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(cwapi3dcpp CwAPI3DEx)
FetchContent_GetProperties(cwapi3dcpp SOURCE_DIR cwapi3dcpp_source_dir)
FetchContent_GetProperties(CwAPI3DEx SOURCE_DIR CwAPI3DEx_source_dir)

message(STATUS "cwapi3dcpp source dir: ${cwapi3dcpp_source_dir}")
message(STATUS "CwAPI3DEx source dir: ${CwAPI3DEx_source_dir}")

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${cwapi3dcpp_source_dir}/include
        ${CwAPI3DEx_source_dir}/geometry3d/include
        ${CwAPI3DEx_source_dir}/composite/include
        ${CwAPI3DEx_source_dir}/utility/include

)


target_link_libraries(${PROJECT_NAME}
        PRIVATE
        CwAPI3D::geometry3d
        CwAPI3D::composite
        CwAPI3D::utility
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        CWAPI3D_PLUGIN_NAME=L"Cadwork Test Plugin"
        CWAPI3D_AUTHOR_NAME=L"Your Name"
        CWAPI3D_AUTHOR_EMAIL=L"your_name@something.swiss"
)
