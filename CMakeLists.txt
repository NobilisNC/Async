cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(async VERSION 0.1.0 LANGUAGES CXX)

option(BUILD_TESTS "Build Tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

include(async.cmake)

if( BUILD_TESTS )
  include(deps/catch2.cmake)
  enable_testing()
  add_subdirectory(tests)
endif()


#if( BUILD_EXAMPLES )
  add_subdirectory(examples)
#endif()
