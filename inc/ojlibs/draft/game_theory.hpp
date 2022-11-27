#ifndef OJLIBS_INC_GAME_THEORY_H_
#define OJLIBS_INC_GAME_THEORY_H_
#include <unordered_map>

namespace ojlibs { // TO_BE_REMOVED

namespace game {

// struct state {
//      size_t hash() const;
//      std::string to_string() const;
//      bool solve(std::function<bool(state)>) const;
// };

template <typename S>
struct solver {
    struct hasher { size_t operator()(const S &s) const { return s.hash(); } };

    unordered_map<S, bool, hasher> res;
    bool solve(const S &s) {
        if (res.count(s) == 0) {
            bool win = s.solve([this](const S &s){ return solve(s); });
            res[s] = win;
        }
        return res.at(s);
    }
};


}

} // namespace ojlibs TO_BE_REMOVED
#endif /* end of include guard: OJLIBS_INC_GAME_THEORY_H_ */
