#ifndef OJLIBS_INC_EBST_H_
#define OJLIBS_INC_EBST_H_
namespace ojlibs { // TO_BE_REMOVED

// struct bst {
//   bst *p, *c[2];
//   void push(); // e.g. reverse subtree
//   void pull(); // e.g. size, depth
// };
//
// assumption:
// 1. push is idempotent
// 2. pull is synthetic
// 3. when accessing p or c of x, x must be clean
// 4. when access x, path from root to x is clean (x itself can be dirty)
//
// we want to build a bst<int, int> to support:
// 1. query by n-th node (by pull)
// 2. negate values of a range (by push)
// 3. reverse a range (by push)

template <typename N>
N *bst_most(N *x, bool r) { // return leftmost / rightmost node in subtree x
    while (x && (x->push(), x->c[r]))
        x = x->c[r];
    return x;
}

template <typename N>
bst_move(N *x, bool r) { // return r ? ++x : --x; (x not null)
    x->push();
    if (x->c[r]) return bst_most(x->c[r], !r);
    while (N *p = x->p; p && x == p->c[r]) {
        x = p;
        p = p->p;
    }
    return p;
}

template <typename N>
void bst_rotate(N *x) {
    //      y               x
    //     / \             / \
    //    x   _    <->    _   y
    //   / \                 / \
    //  _   b               b   _
    //
    // (r=1 case)
    x->push();
    N *y = x->p, *z = y->p;
    bool r = x == y->c[0];
    N *b = x->c[r];

    x->p = z, y->p = x;
    if (b) b->p = y;
    y->c[!r] = b; y->pull();
    x->c[r] = y; x->pull();
    if (z) z->c[z->c[0] != y] = x;
}
    
} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_EBST_H_ */
