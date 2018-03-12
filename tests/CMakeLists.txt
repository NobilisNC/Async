cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(tests VERSION 1.0.0 LANGUAGES CXX)

add_executable(
  main_tests
    main.cc
    general_tests.cc
    signal_tests.cc
    event_tests.cc
    box_tests.cc
    event_loop.cc
    sequence_loop_tests.cc
    pool_tests.cc
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)



target_link_libraries(
  main_tests

    PUBLIC
    Threads::Threads
      deps::catch2
      async
)

add_test(NAME tests::main COMMAND main_tests)
