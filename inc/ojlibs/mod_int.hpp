#ifndef OJLIBS_INC_MODULAR_INT_H_
#define OJLIBS_INC_MODULAR_INT_H_

#include <ojlibs/arith.hpp>

namespace ojlibs { // TO_BE_REMOVED

template <int32_t MOD>
struct mod_int {
    mod_int() : v(0) { }
    mod_int(int32_t in) : v(in) { fixup(); }
    void fixup() {
        if (v < 0 || v >= MOD) {
            v = v % MOD;
            if (v < 0) v += MOD;
        }
    }
    explicit operator int() const { return v; }
#define MINT_OP_GEN(op_str, op_eq_str, op_arith, op_eq_arith) \
    mod_int &operator op_eq_str(const mod_int &that) { \
        op_eq_arith(v, that.v); \
        return *this; \
    }\
    mod_int operator op_str(const mod_int &that) const { \
        return {op_arith(v, that.v)}; \
    }
    MINT_OP_GEN(+, +=, ADDX_MOD, ADDM_MOD)
    MINT_OP_GEN(-, -=, SUBX_MOD, SUBM_MOD)
    MINT_OP_GEN(*, *=, MUL_MOD, MULM_MOD)
    MINT_OP_GEN(/, /=, DIV_MOD, DIVM_MOD)
#undef MINT_OP_GEN
    int32_t v;
};

} // namespace ojlibs TO_BE_REMOVED

#endif /* end of include guard: OJLIBS_INC_MODULAR_INT_H_ */
