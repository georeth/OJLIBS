#ifndef INC_OJLIBS_BST_H_
#define INC_OJLIBS_BST_H_
namespace ojlibs { // TO_BE_REMOVED
// concept BSTHead {
//      BSTHead *lc, *rc, *p;
//      other fields
// }

template <typename Head>
Head *bst_leftmost(Head *x) {
    while (x && x->lc)
        x = x->lc;
    return x;
}
template <typename Head>
Head *bst_rightmost(Head *x) {
    while (x && x->rc)
        x = x->rc;
    return x;
}
template <typename Head>
Head *bst_prev(Head *x) {
    if (x->lc) return bst_rightmost(x->lc);

    Head *p = x->p;
    while (p && x == p->lc) {
        x = p;
        p = p->p;
    }
    return p;
}
template <typename Head>
Head *bst_succ(Head *x) {
    if (x->rc) return bst_leftmost(x->rc);

    Head *p = x->p;
    while (p && x == p->rc) {
        x = p;
        p = p->p;
    }
    return p;
}
template <typename Head>
void bst_rotate(Head *x) {
    Head *y = x->p, *z = y->p;
    Head *b = (x == y->lc) ? x->rc : x->lc;

    x->p = z, y->p = x;
    if (b) b->p = y;
    if (z) (z->lc == y ? z->lc : z->rc) = x;
    if (x == y->lc) x->rc = y, y->lc = b;
    else x->lc = y, y->rc = b;
}

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: INC_OJLIBS_BST_H_ */
