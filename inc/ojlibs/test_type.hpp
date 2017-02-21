#ifndef TEST_TYPE_HPP_XWSIK01R
#define TEST_TYPE_HPP_XWSIK01R
#include <cstdio>
namespace ojlibs { // TO_BE_REMOVED
struct test_type {
    int v;
    test_type(int v = 0) : v(v) {
        printf("normal ctor [%d] %p\n", v, this);
    }
    test_type(const test_type &that) {
        v = that.v;
        printf("copy ctor [%d] %p\n", v, this);
    }
    test_type(test_type &&that) {
        v = that.v;
        that.v = -1;
        printf("move ctor [%d] %p <- %p\n", v, this, &that);
    }
    ~test_type() {
        printf("dtor [%d] %p\n", v, this);
    }
};
    
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: TEST_TYPE_HPP_XWSIK01R */
