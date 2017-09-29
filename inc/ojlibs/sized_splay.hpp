#ifndef OJLIBS_INC_SIZED_SPLAY_H_
#define OJLIBS_INC_SIZED_SPLAY_H_

#include <type_traits>
#include <ojlibs/intrusive_util.hpp>
#include <ojlibs/bst.hpp>
// DO NOT FORGET TO SPLAY, EVEN ON SEARCH OPERATION

namespace ojlibs { // TO_BE_REMOVED

// CORE
struct splay_head {
    splay_head *lc, *rc, *p;
    int size;
    splay_head() { reset(); }
    splay_head(const splay_head &that) = delete;
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
static inline void splay_insert_before(splay_head *&root, splay_head *x, splay_head *y) {
    // insert x before y
    if (!y) {
        x->lc = root;
        if (root) root->p = x;
        root = x;
        x->update_size();
    } else {
        splay_splay(root, y, nullptr);
        if (!y->lc)
            y->lc = x, x->p = y, y->update_size();
        else {
            splay_head *prev = bst_rightmost(y->lc);
            splay_splay(root, prev, y);
            prev->rc = x;
            x->p = prev;
            prev->update_size();
            y->update_size();
        }
    }
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
    if (!root || k >= root->size) return nullptr;
    splay_head *p = splay_find_nth_inner(root, k);
    splay_splay(root, p, nullptr);
    return p;
}

template <typename T, splay_head T::*field>
struct splay_array {
    typedef intrusive_helper<T, splay_head, field> helper_type;
    typedef T value_type;
    typedef bst_iterator<helper_type> iterator_type;

    splay_head *root = nullptr;
    splay_array() { }
    splay_array(const splay_array &that) = delete;

    int size() {
        return root ? root->size : 0;
    }
    iterator_type begin() const {
        return iterator_type(bst_leftmost(root));
    }
    iterator_type end() const {
        return iterator_type();
    }
    iterator_type find_nth(int k) {
        return iterator_type(splay_find_nth(root, k)); // splay!
    }
    int get_rank(iterator_type it) {
        return splay_get_rank(root, it.cur);
    }
    iterator_type insert_front(T *node) {
        splay_head *x = helper_type::to_member(node);
        x->rc = root;
        if (root) root->p = x;
        root = x;
        x->update_size();
        return iterator_type(x);
    }
    iterator_type insert_back(T *node) {
        splay_head *x = helper_type::to_member(node);
        x->lc = root;
        if (root) root->p = x;
        root = x;
        x->update_size();
        return iterator_type(x);
    }
    iterator_type insert_nth(T *node, int k) {
        if (k == 0) return insert_front(node);
        else if (k < size()) {
            splay_head *x = helper_type::to_member(node);
            splay_head *y = splay_find_nth_inner(root, k); // don't splay
            splay_insert_before(root, x, y); // let's splay here
            return iterator_type(x);
        } else if (k == size()) return insert_back(node);
        else throw 0;
    }
    iterator_type erase(iterator_type it) {
        iterator_type next = it;
        ++next;
        splay_erase(root, it.cur);
        return next;
    }
};

template <typename T, splay_head T::*field, typename Key = T, typename Accessor=identity_accessor<Key>,
          typename Comp = std::less<Key>>
struct sized_splay {
    typedef intrusive_helper<T, splay_head, field> helper_type;
    typedef T value_type;
    typedef Key key_type;
    typedef bst_iterator<helper_type> iterator_type;

    Accessor acc;
    Comp comp;

    splay_head *root = nullptr;
    sized_splay() { }
    sized_splay(const sized_splay &that) = delete;

    int size() const {
        return root ? root->size : 0;
    }
    iterator_type begin() const {
        return iterator_type(bst_leftmost(root));
    }
    iterator_type end() const {
        return iterator_type();
    }

    iterator_type find_nth(int k) {
        return iterator_type(splay_find_nth(root, k)); // splay!
    }
    int get_rank(iterator_type it) {
        return splay_get_rank(root, it.cur);
    }
    iterator_type erase(iterator_type it) {
        iterator_type next = it;
        ++next;
        splay_erase(root, it.cur);
        return next;
    }
    iterator_type find(const key_type &k) {
        splay_head *x = bst_lower_bound<helper_type>(root, k, acc, comp);
        if (x) splay_splay(root, x, nullptr);
        if (x && !comp(k, acc(*helper_type::to_parent(x))))
            return iterator_type(x);
        return iterator_type();
    }
    iterator_type lower_bound(const key_type &k) {
        splay_head *x = bst_lower_bound<helper_type>(root, k, acc, comp);
        if (x) splay_splay(root, x, nullptr);
        return iterator_type(x);
    }
    iterator_type upper_bound(const key_type &k) {
        splay_head *x = bst_upper_bound<helper_type>(root, k, acc, comp);
        if (x) splay_splay(root, x, nullptr);
        return iterator_type(x);
    }
    iterator_type insert(T *node) {
        splay_head *x = helper_type::to_member(node);
        splay_head *y = bst_lower_bound<helper_type>(root, acc(*node), acc, comp);
        splay_insert_before(root, x, y);
        return iterator_type(x);
    }
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
