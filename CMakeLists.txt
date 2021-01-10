cmake_minimum_required (VERSION 3.1)
project (owo)

find_package(PkgConfig)
pkg_check_modules(yaml-cpp yaml-cpp REQUIRED)

add_subdirectory(vendor/verbly)
add_subdirectory(vendor/libtwittercpp)

include_directories(
  vendor/verbly/lib
  vendor/libtwittercpp/src
  ${yaml-cpp_INCLUDE_DIRS})

add_executable(owo owo.cpp)
set_property(TARGET owo PROPERTY CXX_STANDARD 11)
set_property(TARGET owo PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(owo verbly twitter++ ${yaml-cpp_LIBRARIES})
