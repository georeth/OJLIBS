link_libraries(gtest_main gtest gmp pthread)

set(test_cases # list all tests here
	list_test
	rbtree_test
	memorization_test
	segtree_test
	disjoint_set_test
	arith_test
	linear_test
)

function(ojlibs_add_test filename)
	add_executable(${filename} ${filename}.cpp)
	add_test(${filename} ${filename})
endfunction(ojlibs_add_test)

foreach(test ${test_cases})
	ojlibs_add_test(${test})
endforeach(test)
