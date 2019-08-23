cmake_minimum_required(VERSION 3.0.0)
project(print VERSION 0.1.0)

add_library(print INTERFACE)
target_include_directories(print INTERFACE ${CMAKE_CURRENT_LIST_DIR})

add_executable(print_test main.cpp)
target_link_libraries(print_test PRIVATE print)
target_compile_features(print_test PRIVATE cxx_std_17)

include(CTest)
enable_testing()
add_test(run_print_test print_test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
