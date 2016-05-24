add_executable(hello_test hello_test.cpp)

add_executable(list_test list_test.cpp)
target_link_libraries(list_test gtest_main gtest pthread)

add_executable(rbtree_test rbtree_test.cpp)
target_link_libraries(rbtree_test gtest_main gtest pthread)

add_executable(segtree_test segtree_test.cpp)
target_link_libraries(segtree_test gtest_main gtest pthread)

add_test(HelloTest hello_test)
add_test(ListTest list_test)
add_test(RbtreeTest rbtree_test)
add_test(SegTest segtree_test)
