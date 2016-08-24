add_executable(hello_test hello_test.cpp)

add_executable(list_test list_test.cpp)
target_link_libraries(list_test gtest_main gtest)

add_executable(rbtree_test rbtree_test.cpp)
target_link_libraries(rbtree_test gtest_main gtest)

add_executable(segtree_test segtree_test.cpp)
target_link_libraries(segtree_test gtest_main gtest pthread)

add_executable(memorization_test memorization_test.cpp)
target_link_libraries(memorization_test gtest_main gtest)

add_executable(arith_test arith_test.cpp)
target_link_libraries(arith_test gtest_main gtest gmp)

add_test(HelloTest hello_test)
add_test(ListTest list_test)
add_test(RbtreeTest rbtree_test)
add_test(MemorizationTest memorization_test)
add_test(SegTest segtree_test)
add_test(ArithTest arith_test)
