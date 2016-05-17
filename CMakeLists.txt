cmake_minimum_required (VERSION 2.8) 
project (OJLIBS) 

enable_testing ()
include_directories(inc)
add_subdirectory (src) 
add_subdirectory (test) 
