#include "gtest/gtest.h"

#include "Parser.h"
#include "BasicParsers.h"

TEST(CombinationTests, testAlternative) {
    Parser p1 = pstring("aba");
    Parser p2 = pstring("ada");
    std::string s = "abacaba";
    SStream ss(s);

    Parser p = p2 | p1;
    auto res = p.runParser(ss);

    ASSERT_TRUE(res.success);
    ASSERT_EQ(ss.getPos(), 3);
    ASSERT_EQ(res.obj, "aba");
}

TEST(CombinationTests, testAlternativeMany) {
    Parser p1 = pstring("aba");
    Parser p2 = pstring("ada");
    Parser p3 = pstring("aca");
    std::string s = "abacaba";
    SStream ss(s);

    Parser p = Parser<std::string>::oneOf({p3, p2, p1});
    auto res = p.runParser(ss);

    ASSERT_TRUE(res.success);
    ASSERT_EQ(ss.getPos(), 3);
    ASSERT_EQ(res.obj, "aba");
}

TEST(CombinationTests, testCombination) {
    Parser p = Parser<std::string>::combine([](auto &ss) {
        auto s1 = pstring("aba")[ss];
        auto s2 = pstring("cab")[ss];
        auto c1 = pchar('a')[ss];
        return s1 + s2 + c1;
    });

    std::string s = "abacaba";
    SStream ss(s);
    auto res = p.runParser(ss);

    ASSERT_EQ(ss.getPos(), 7);
    ASSERT_TRUE(res.success);
    ASSERT_EQ(res.obj, "abacaba");
}

TEST(CombinationTests, testMany) {
    auto f = [](char c) {return std::isdigit(c);};
    Parser pc = pcheck(f);
    Parser<std::vector<char>>  p = Parser<char>::many(pc);

    std::string s = "1209aaa1111";
    SStream ss(s);
    auto res = p.runParser(ss);

    ASSERT_EQ(ss.getPos(), 4);
    ASSERT_TRUE(res.success);
    std::vector <char> expected = {'1', '2', '0', '9'};
    ASSERT_EQ(res.obj, expected);
}