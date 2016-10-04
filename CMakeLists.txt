cmake_minimum_required (VERSION 2.8) 
project (OJLIBS) 
set(CMAKE_CXX_STANDARD 11)

enable_testing ()
include_directories(./inc)
add_subdirectory (src) 
add_subdirectory (test) 
