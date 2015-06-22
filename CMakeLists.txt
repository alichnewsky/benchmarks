cmake_minimum_required( VERSION 3.0 )

project(foobar)

# external / third party cmake setup a la grpc.
# provider "module' means submodule under third_party
# provider "package" means cmake's find_package

set(project_GOOGLE_TEST_PROVIDER "module" CACHE STRING "provider of the google test library" )
set_property( CACHE project_GOOGLE_TEST_PROVIDER PROPERTY STRINGS "module" "package" )

set(project_GOOGLE_BENCHMARK_PROVIDER "module" CACHE STRING "provider of the google benchmark library" )
set_property( CACHE project_GOOGLE_BENCHMARK_PROVIDER PROPERTY STRINGS "module" "package" )

include( cmake/gtest.cmake )
include( cmake/benchmark.cmake)

add_subdirectory( src )
