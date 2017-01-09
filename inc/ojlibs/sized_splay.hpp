#ifndef OJLIBS_INC_SIZED_SPLAY_H_
#define OJLIBS_INC_SIZED_SPLAY_H_

#include <ojlibs/intrusive_util.hpp>
#include <ojlibs/bst.hpp>
// DO NOT FORGET TO SPLAY, EVEN ON SEARCH OPERATION

namespace ojlibs { // TO_BE_REMOVED

// CORE
struct splay_head {
    splay_head *lc, *rc, *p;
    int size;
    splay_head() { reset(); }
    void reset() {
        lc = rc = p = nullptr;
        size = 1;
    }
    void update_size() {
        size = 1;
        if (lc) size += lc->size;
        if (rc) size += rc->size;
    }
};

static inline void splay_rotate(splay_head *x) { // size of x is not updated
    splay_head *y = x->p;
    bst_rotate(x);
    y->update_size();
}

static inline void splay_splay(splay_head *&root, splay_head *x, splay_head *target) {
    while (x->p != target) {
        if (x->p->p != target) {
            if ((x->p->lc == x) == (x->p->p->lc == x->p))
                splay_rotate(x->p);
            else
                splay_rotate(x);
        }
        splay_rotate(x);
    }
    x->update_size();
    if (!target)
        root = x;
}
static inline void splay_erase(splay_head *&root, splay_head *x) {
    splay_splay(root, x, nullptr);
    if (!x->lc) {
        if (x->rc) x->rc->p = nullptr;
        root = x->rc;
    } else {
        splay_head *prev = bst_rightmost(x->lc);
        splay_splay(root, prev, x);
        prev->rc = x->rc;
        if (x->rc) x->rc->p = prev;
        prev->update_size();
        prev->p = nullptr, root = prev;
    }
    x->reset();
}
// Rank related
static inline int splay_get_rank(splay_head *&root, splay_head *x) {
    if (!x) return root ? root->size : 0; // over-end iterator

    splay_splay(root, x, nullptr);
    return x->lc ? x->lc->size : 0;
}
static inline splay_head *splay_find_nth_inner(splay_head *&root, int k) {
    splay_head *p = root;
    while (true) {
        int lsize = p->lc ? p->lc->size : 0;
        if (k < lsize)
            p = p->lc;
        else if (k == lsize)
            return p;
        else
            k -= lsize + 1, p = p->rc;
    }
    return p;
}
static inline splay_head *splay_find_nth(splay_head *&root, int k) {
    splay_head *p = splay_find_nth_inner(root, k);
    splay_splay(root, p, nullptr);
    return p;
}

// Container
template <typename T, splay_head T::*field>
struct splay_iterator {
    typedef splay_head *head_ptr;

    // for iterator_traits
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef std::bidirectional_iterator_tag iterator_category;

    // TODO
};

} // namespace ojlibs TO_BE_REMOVED

// TODO: insert
// Two method:
// (I) to insert x before y
//
//      first splay y to root
//      if (!y->lc) insert into y->lc
//      else prev = prev(y)
//           splay prev under x
//           insert into prev->rc
// (II) insert to leaf
//      splay to root
//
// TODO : dynamic array
#endif /* end of include guard: OJLIBS_INC_SIZED_SPLAY_H_ */
