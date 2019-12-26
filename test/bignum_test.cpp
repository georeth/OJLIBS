#include <iostream>
#include <ojlibs/bignum.hpp>
#include <gtest/gtest.h>
#include <random>
#include <boost/multiprecision/gmp.hpp>

using namespace boost::multiprecision;
using namespace ojlibs::bignum;

std::mt19937 gen;
std::uniform_int_distribution<uint32_t> dist(0, ~0u);

bool SameNumber(const mpz_int &mpz, const nat &bnat) {
    auto mpz_inner = mpz.backend().data();

    size_t mpz_sz = mpz_size(mpz_inner);
    size_t nat_sz = bnat.nlimb();

    size_t gcd = std::min(sizeof(mp_limb_t), sizeof(WORD));

    size_t mpz_k = sizeof(mp_limb_t) / gcd;
    size_t nat_k = sizeof(WORD) / gcd;

    size_t cnt = std::max(mpz_sz * mpz_k, nat_sz * nat_k);

    const mp_limb_t *mpz_data = mpz_limbs_read(mpz_inner);
    const WORD *nat_data = bnat.limbs.data();

    uint64_t mask = (1ULL << (8 * gcd)) - 1;

    for (int i = 0; i < cnt; ++i) {
        uint64_t mpz_part = 0;
        uint64_t nat_part = 0;
        if (i / mpz_k < mpz_sz)
            mpz_part = (mpz_data[i / mpz_k] >> (8 * gcd * (i % mpz_k))) & mask;
        if (i / nat_k < nat_sz)
            nat_part = (nat_data[i / nat_k] >> (8 * gcd * (i % nat_k))) & mask;
        if (mpz_part != nat_part) return false;
    }
    return true;
}
bool SameNumber(WORD n, nat nn) {
    if (n == 0) return nn.nlimb() == 0;
    return nn.nlimb() == 1 && nn.limbs[0] == n;
}
std::vector<WORD> random_limbs(int nlimb) {
    std::vector<WORD> ret(nlimb);
    for (int i = 0; i < nlimb; ++i)
        ret[i] = dist(gen);

    if (nlimb && ret[nlimb - 1] == 0) ret[nlimb - 1] = 1; // should be norm
    return ret;
}
mpz_int limbs_to_mpz(std::vector<WORD> vec) {
    mpz_int ret = 0;
    for (int i = vec.size(); i-- > 0; ) {
        ret = (ret << WBITS) + vec[i];
    }
    return ret;
}

TEST(SANITY, SameNumber) {
    ASSERT_TRUE(SameNumber(mpz_int(34), nat(34u)));
    ASSERT_FALSE(SameNumber(mpz_int(34), nat(35u)));

    for (int tgroup = 0; tgroup < 100; ++tgroup) {
        auto lb = random_limbs(10);
        EXPECT_TRUE(SameNumber(limbs_to_mpz(lb), nat(lb)));
    }
}

TEST(ADD, VW) {
    for (int tgroup = 0; tgroup < 100; ++tgroup) {
        uint32_t r = dist(gen);
        mpz_int mp = r;
        nat nt(r);

        ASSERT_TRUE(SameNumber(mp, nt));

        for (int titer = 0; titer < 100; ++titer) {
            uint32_t r = dist(gen);
            mpz_int mpx = r;
            nat     ntx(r);

            mp = mp + mpx;
            nt = nt + ntx;

            ASSERT_TRUE(SameNumber(mp, nt));
        }
    }

}
TEST(ADD, VV) {
    static const size_t TSIZE = KARA_THRES * 2;
    for (int tgroup = 0; tgroup < 1; ++tgroup) {
        auto lb1 = random_limbs(TSIZE), lb2 = random_limbs(TSIZE);

        nat nt1(lb1), nt2(lb2);
        mpz_int mp1 = limbs_to_mpz(lb1), mp2 = limbs_to_mpz(lb2);

        nat nt = nt1 + nt2;
        mpz_int mp = mp1 + mp2;

        EXPECT_TRUE(SameNumber(mp, nt));
    }
}

TEST(SUB, VV) {
    static const size_t TSIZE = 100;
    std::uniform_int_distribution<int> dist_sz(0, TSIZE);
    for (int tgroup = 0; tgroup < 1; ++tgroup) {
        auto lb1 = random_limbs(dist_sz(gen)), lb2 = random_limbs(dist_sz(gen));

        nat nt1(lb1), nt2(lb2);
        mpz_int mp1 = limbs_to_mpz(lb1), mp2 = limbs_to_mpz(lb2);

        if (mp1 < mp2) std::swap(nt1, nt2), std::swap(mp1, mp2);

        nat nt = nt1 - nt2;
        mpz_int mp = mp1 - mp2;

        EXPECT_TRUE(SameNumber(mp, nt));
    }
}

TEST(MUL, VW) {
    for (int tgroup = 0; tgroup < 100; ++tgroup) {
        uint32_t r = dist(gen);
        mpz_int mp = r;
        nat     nt(r);

        ASSERT_TRUE(SameNumber(mp, nt));

        for (int titer = 0; titer < 100; ++titer) {
            uint32_t r = dist(gen);
            mpz_int mpx = r;
            nat     ntx(r);

            mp = mp * mpx;
            nt = nt * ntx;


            ASSERT_TRUE(SameNumber(mp, nt));
        }

    }
}
TEST(MUL, BASIC) {
    static const size_t TSIZE = KARA_THRES / 2;
    for (int tgroup = 0; tgroup < 100; ++tgroup) {
        auto lb1 = random_limbs(TSIZE), lb2 = random_limbs(TSIZE);
        nat nt1(lb1), nt2(lb2);
        mpz_int mp1 = limbs_to_mpz(lb1), mp2 = limbs_to_mpz(lb2);

        // nat nt = nt1 * nt2;
        nat nt;
        nt.limbs.resize(2 * TSIZE);
        basicMul(nt.to_span(), nt1.to_cspan(), nt2.to_cspan());
        nt.norm();

        mpz_int mp = mp1 * mp2;

        EXPECT_TRUE(SameNumber(mp, nt));
        EXPECT_EQ(0, nt.cmp(nt1 * nt2));
    }
}
TEST(MUL, LEADING) {
    static const size_t TSIZE = KARA_THRES;
    VEC lb1(TSIZE, 0), lb2(TSIZE, 0);
    lb1[TSIZE - 1] = 1;
    lb2[TSIZE - 1] = 1;

    nat nt1(lb1), nt2(lb2);
    mpz_int mp1 = limbs_to_mpz(lb1), mp2 = limbs_to_mpz(lb2);

    nat nt = nt1 * nt2;
    mpz_int mp = mp1 * mp2;
    EXPECT_TRUE(SameNumber(mp, nt));
}
TEST(MUL, VV) {
    static const size_t TSIZE = KARA_THRES * 50;
    std::uniform_int_distribution<int> dist_sz(0, TSIZE);
    for (int tgroup = 0; tgroup < 50; ++tgroup) {
        auto lb1 = random_limbs(dist_sz(gen)), lb2 = random_limbs(dist_sz(gen));

        nat nt1(lb1), nt2(lb2);
        mpz_int mp1 = limbs_to_mpz(lb1), mp2 = limbs_to_mpz(lb2);

        nat nt = nt1 * nt2;
        mpz_int mp = mp1 * mp2;

        // FIXME: EXPECT
        EXPECT_TRUE(SameNumber(mp, nt));
        // fprintf(stderr, "PASS\n");
    }
}
TEST(DIV, VW) {
    static const size_t TSIZE = KARA_THRES * 5;
    std::uniform_int_distribution<int> dist_sz(1, TSIZE);
    for (int tgroup = 0; tgroup < 100; ++tgroup) {
        auto lb1 = random_limbs(dist_sz(gen));
        mpz_int mp1 = limbs_to_mpz(lb1);
        nat nt1{lb1};

        WORD b = dist(gen);
        mpz_int mp2 = b;
        nat nt2{b};

        nat nt3 = nt1 / nt2;
        nat nt4 = nt1 % nt2;

        mpz_int mp3 = mp1 / mp2;
        mpz_int mp4 = mp1 % mp2;

        EXPECT_TRUE(SameNumber(mp3, nt3));
        EXPECT_TRUE(SameNumber(mp4, nt4));
    }
}
TEST(DIV, VVsmall) {
    VEC lb1 = {0, 1, 1, 2};
    mpz_int mp1 = limbs_to_mpz(lb1);
    nat nt1{lb1};

    VEC lb2 = {1, 100, 1, 100};
    mpz_int mp2 = limbs_to_mpz(lb2);
    nat nt2{lb2};

    nat nt3 = nt1 / nt2;
    nat nt4 = nt1 % nt2;

    mpz_int mp3 = mp1 / mp2;
    mpz_int mp4 = mp1 % mp2;

    ASSERT_TRUE(SameNumber(mp3, nt3));
    ASSERT_TRUE(SameNumber(mp4, nt4));
}
TEST(DIV, VV) {
    static const size_t TSIZE = KARA_THRES * 5;
    std::uniform_int_distribution<int> dist_sz(1, TSIZE);
    // std::uniform_int_distribution<int> dist_sz(6, 6);
    for (int tgroup = 0; tgroup < 100; ++tgroup) {
        auto lb1 = random_limbs(dist_sz(gen));
        mpz_int mp1 = limbs_to_mpz(lb1);
        nat nt1{lb1};

        auto lb2 = random_limbs(dist_sz(gen));
        mpz_int mp2 = limbs_to_mpz(lb2);
        nat nt2{lb2};

        nat nt3 = nt1 / nt2;
        nat nt4 = nt1 % nt2;

        mpz_int mp3 = mp1 / mp2;
        mpz_int mp4 = mp1 % mp2;

        ASSERT_TRUE(SameNumber(mp4, nt4));
        ASSERT_TRUE(SameNumber(mp3, nt3));
    }
}
TEST(STRCONV, UTOA) {
    static const size_t TSIZE = 500;
    std::uniform_int_distribution<int> dist_sz(1, TSIZE);
    // std::uniform_int_distribution<int> dist_sz(6, 6);
    for (int tgroup = 0; tgroup < 100; ++tgroup) {
        auto lb1 = random_limbs(dist_sz(gen));
        mpz_int mp1 = limbs_to_mpz(lb1);
        nat nt1{lb1};

        auto mps = mp1.str();
        auto nts = utoa(nt1, 10);

        ASSERT_EQ(mps, nts);
    }
}
TEST(STRCONV, BACKFORTH) {
    static const size_t TSIZE = 50;
    std::uniform_int_distribution<int> dist_sz(1, TSIZE);
    // std::uniform_int_distribution<int> dist_sz(6, 6);
    for (int base = 2; base <= 62; ++base) {
        for (int tgroup = 0; tgroup < 100; ++tgroup) {
            auto lb1 = random_limbs(dist_sz(gen));

            nat nt(lb1);
            std::string s = utoa(nt, base);
            nat nt2 = str_to_nat(s.c_str(), base);

            ASSERT_TRUE(nt == nt2);
        }
    }
}
