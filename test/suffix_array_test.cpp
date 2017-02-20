#include <ojlibs/suffix_array.hpp>
#include <gtest/gtest.h>
#include <cstdio>
#include <random>
#include <string>
#include <iostream>
using namespace std;
std::mt19937 gen;
std::string avail = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
using uni = std::uniform_int_distribution<>;

std::string random_string(int len) {

    std::string ret;
    for (int i = 0; i < len; ++i)
	ret.push_back(avail[gen() % avail.size()]);
    return ret;
}

bool is_valid(const vector<int> &sa, const string &s) {
    if (sa.size() != s.size()) return printf("my 1\n"), false;
    vector<int> all = sa;
    sort(all.begin(), all.end());
    for (int i = 0; i < all.size(); ++i)
	if (all[i] != i) return printf("my 2\n"), false;

    for (int i = 1; i < s.size(); ++i)
	if (s.substr(sa[i - 1]) >= s.substr(sa[i])) return printf("my 3\n"), false;
    return true;
}
TEST(BASIC, corner) {
    std::string str = "";
    std::vector<int> sa = ojlibs::suffix_array(str.begin(), str.end());

    ASSERT_TRUE(is_valid(sa, str));
}

TEST(BASIC, random) {
    static const int TEST_GROUP = 100;
    for (int tg = 0; tg < TEST_GROUP; ++tg) {
	std::string str = random_string(uni(100, 1000)(gen));
	std::vector<int> sa = ojlibs::suffix_array(str.begin(), str.end());
	EXPECT_TRUE(is_valid(sa, str));
	if (!is_valid(sa, str)) {
	    for (int k : sa)
		cout << str.substr(k) << endl;
	}
    }
}
