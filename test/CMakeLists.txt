include(CTest)
link_libraries(gtest_main gtest gmp pthread)

set(test_cases # list all tests here
	# hello_test
	2sat_test
	# data_structure
	# list_test
	# slist_test
	# rbtree_test
	sized_splay_test
	segtree_test
	lazy_segtree_test
	implicit_segtree_test
	segtree2d_test
	bitree_test
	disjoint_set_test
	sparse_table_test
	graph_test
	# algorithm
	string_test
	suffix_array_test
	dinic_hugarian_test
	# todo : push_relabel_test
	# todo : kmp_test
	trie_ac_test
	prufer_test
	mst_test
	# other
	memoization_test
	arith_test
	power_test
	linear_test
	matrix_test
	shorthand_test
	prime_list_test
	# geometry
	geometry_test
	# inversion
	mobius_inversion_test
	subset_inversion_test
	# test_type_test
	# counting_test
	hash_test


	## Other tests
	digit_dc_test
)

function(ojlibs_add_test filename)
	add_executable(${filename} ${filename}.cpp)
	add_test(${filename} ${filename})
endfunction(ojlibs_add_test)

foreach(test ${test_cases})
	ojlibs_add_test(${test})
endforeach(test)

target_compile_definitions(shorthand_test PRIVATE -DLOCAL_JUDGE)
