#include "gtest/gtest.h"

#include "Parser.h"
#include "BasicParsers.h"


TEST(ParserTests, testStringParserSuccess) {
    Parser p = pstring("abacaba");
    std::string s = "abacaba";
    SStream ss(s);
    auto res = p.runParser(ss);

    ASSERT_EQ(ss.getPos(), 7);
    ASSERT_TRUE(res.success);
    ASSERT_EQ(res.obj, "abacaba");
}

TEST(ParserTests, testStringParserFailure) {
    Parser p = pstring("abadaba");

    std::string s = "abacaba";
    SStream ss(s);
    auto res = p.runParser(ss);

    ASSERT_EQ(ss.getPos(), 0);
    ASSERT_FALSE(res.success);
}

TEST(ParserTests, testSequence) {
    Parser p1 = pstring("aba");
    Parser p2 = pstring("dabaaba");
    Parser p3 = pstring("ca");
    Parser p4 = pstring("da");
    Parser p5 = pstring("ba");
    std::string s = "abacaba";
    SStream ss(s);

    p1.runParser(ss);
    p2.runParser(ss);
    p3.runParser(ss);
    p4.runParser(ss);
    auto res = p5.runParser(ss);

    ASSERT_EQ(ss.getPos(), 7);
    ASSERT_TRUE(res.success);
    ASSERT_EQ(res.obj, "ba");
}

TEST(ParserTests, testChar) {
    Parser p1 = pchar('a');
    Parser p2 = pchar('b');
    Parser p3 = pchar('c');
    std::string s = "ac";
    SStream ss(s);

    p1.runParser(ss);
    auto res1 = p2.runParser(ss);
    auto res2 = p3.runParser(ss);

    ASSERT_FALSE(res1.success);
    ASSERT_EQ(ss.getPos(), 2);
    ASSERT_TRUE(res2.success);
    ASSERT_EQ(res2.obj, 'c');
}