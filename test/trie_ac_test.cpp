#include <gtest/gtest.h>
#include <random>

#include <ojlibs/string/trie.hpp>
#include <ojlibs/string/ac_automaton.hpp>
using namespace std;
using uni = std::uniform_int_distribution<>;
std::mt19937 gen;

TEST(BASIC, simple) {
    ojlibs::trie<int, 10, '0'> tr(-1);

    tr["123"] = 12;
    EXPECT_EQ(tr["123"], 12);
    EXPECT_EQ(tr["12"], -1);
    EXPECT_EQ(tr[""], -1);
}
TEST(BASIC, ac_automaton) {
    vector<string> temp = {"00", "01", "1001"};
    ojlibs::ac_automaton<2, '0'> ac;
    for (int i = 0; i < temp.size(); ++i)
        ac.add(temp[i], i);
    ac.build();

    string s = "10101001101";
    vector<int> match = ac.traverse(s);
    vector<int> expected = { -1, -1, 1, -1, 1, -1, -1, 2, -1, -1, 1 };
    EXPECT_EQ(match, expected);
}
