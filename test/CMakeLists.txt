add_executable(hello_test hello_test.cpp)
target_link_libraries(hello_test ojlibs)

add_test(HelloTest hello_test)
