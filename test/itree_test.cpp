#include <algorithm>
#include "ojlibs/itree.hpp"
using namespace std;
using namespace ojlibs::data_structure;

struct node {
    int left;
    int leftval;
    int right;
    int rightval;
    int val;
};
node operator+(const node &left, const node &right) {
    node ret;
    ret.val = max(left.val, right.val);
    ret.left = left.left;
    ret.right = right.right;
    ret.leftval = left.leftval;
    ret.rightval = right.rightval;
    if (left.right == right.left) {
        ret.val = max(ret.val, left.rightval + right.leftval);
        if (left.left == right.left)
            ret.leftval += right.leftval;
        if (right.right == left.right)
            ret.rightval += left.rightval;
    } else {
    }
    return ret;
}

int main() {
    int T;
    int N, Q;
    itree<node> itree(0);
    return 0;
    while (scanf("%d%d", &N, &Q), N) {
        itree.resize_without_clear(N);
        for (int i = 0; i < N; ++i) {
            int v;
            node n;
            scanf("%d", &v);
            n.left = v;
            n.right = v;
            n.leftval = 1;
            n.rightval = 1;
            n.val = 1;
            itree[i] = n;
        }
        itree.rebuild();
        for (int i = 0; i < Q; ++i) {
            int s, t;
            scanf("%d%d", &s, &t);
            printf("%d\n", itree.query_include(s - 1, t - 1).val);
        }
    }
    return 0;
}
