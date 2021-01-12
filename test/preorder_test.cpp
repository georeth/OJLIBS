#include <gtest/gtest.h>
#include <random>
#include <ojlibs/misc/preorder.hpp>
using namespace std;
using namespace ojlibs;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;
uni dist_v(-10000000, 10000000);

TEST(BASIC, ctor) {
    preorder_solver_general<3> solver;
}

template <int D>
void test_general_func(int test_size, bool check = true) {
    typedef preorder_solver_general<D> solver_t;
    solver_t solver;
    typedef typename solver_t::coord_t coord_t;
    vector<coord_t> Q, U;

    for (int i = 0; i < test_size; ++i) {
        coord_t coord;
        for (int j = 0; j < D; ++j)
            coord[j] = dist_v(gen);
        Q.push_back(coord);
        solver.add_query(coord, i);
    }
    for (int i = 0; i < test_size; ++i) {
        coord_t coord;
        for (int j = 0; j < D; ++j)
            coord[j] = dist_v(gen);
        U.push_back(coord);
        solver.add_update(coord);
    }

    solver.discretize();
    solver.solve();

    if (!check)
        return;

    vector<int64_t> actual(test_size);
    vector<int64_t> expected(test_size);

    for (int i = 0; i < test_size; ++i) { // query
        for (int j = 0; j < test_size; ++j) { // update
            bool ok = true;

            for (int d = 0; d < D; ++d)
                if (Q[i][d] < U[j][d]) {
                    ok = false;
                    break;
                }

            if (ok) ++expected[i];
        }
    }

    for (auto q : solver.qs) {
        if (q.is_q) {
            actual[q.id] = q.ans;
        }
    }

    for (int i = 0; i < test_size; ++i) { // query
        ASSERT_EQ(actual[i], expected[i]);
    }
}

TEST(CORRECTNESS, general_2D) {
    test_general_func<2>(1000);
}
TEST(CORRECTNESS, general_3D) {
    test_general_func<3>(1000);
}
TEST(CORRECTNESS, general_6D) {
    test_general_func<6>(1000);
}

TEST(BENCH, general_2D) {
    test_general_func<2>(100000 / 2, false);
}
TEST(BENCH, general_3D) {
    test_general_func<3>(100000 / 2, false);
}
TEST(BENCH, general_6D) {
    test_general_func<6>(1000, false);
}
