#ifndef TEST_TYPE_HPP_XWSIK01R
#define TEST_TYPE_HPP_XWSIK01R
#include <cstdio>

namespace ojlibs { // TO_BE_REMOVED
struct test_type {
    int v;
    test_type(int v = 0) : v(v) {
        printf("test_type: normal ctor [%d] %p\n", v, (void *)this);
    }
    test_type(const test_type &that) {
        v = that.v;
        printf("test_type: copy ctor [%d] %p\n", v, (void *)this);
    }
    test_type(test_type &&that) {
        v = that.v;
        that.v = -1;
        printf("test_type: move ctor [%d] %p <- %p\n", v, (void *)this, (void *)&that);
    }
    ~test_type() {
        printf("test_type: dtor [%d] %p\n", v, (void *)this);
    }
    void f() { printf("test_type::f\n"); }
    virtual void vf() { printf("test_type::vf\n"); }
};
struct test_derived_type : public test_type {
    int v;
    test_derived_type(int v = 0) : v(v) {
        printf("test_derived_type: normal ctor [%d] %p\n", v, (void *)this);
    }
    test_derived_type(const test_type &that) {
        v = that.v;
        printf("test_derived_type: copy ctor [%d] %p\n", v, (void *)this);
    }
    test_derived_type(test_type &&that) {
        v = that.v;
        that.v = -1;
        printf("test_derived_type: move ctor [%d] %p <- %p\n", v, (void *)this, (void *)&that);
    }
    ~test_derived_type() {
        printf("test_derived_type: dtor [%d] %p\n", v, (void *)this);
    }
    void f() { printf("test_derived_type: f\n"); }
    virtual void vf() { printf("test_derived_type: :vf\n"); }
};
    
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: TEST_TYPE_HPP_XWSIK01R */
