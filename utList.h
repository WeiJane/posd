#ifndef UTLIST_H
#define UTLIST_H

#include <string>
using std::string;

#include "struct.h"
#include "atom.h"
#include "number.h"
#include "variable.h"
#include "list.h"

// When create a new list without any item
// Then #symbol() of the list should return "[]"
TEST (List, constructor) {
    List l;
    ASSERT_EQ("[]", l.symbol());
}

// Given there are two perfect Numbers: 8128, 496
// When create a new list with the perfect Number
// Then #symbol() of the list should return "[496, 8128]"
TEST(List, Numbers) {
    Number n8128(8128);
    Number n496(496);
    vector <Term *> v = {&n496, &n8128};
    List l(v);
    ASSERT_EQ("[496, 8128]", l.symbol());
}

// Given there are two atoms: "terence_tao", "alan_mathison_turing"
// When create a new list with the Atoms
// Then #symbol() of the list should return "[terence_tao, alan_mathison_turing]"
TEST(List, Atoms) {
    Atom terence_tao("terence_tao");
    Atom alan_mathison_turing("alan_mathison_turing");
    vector <Term *> v = {&terence_tao, &alan_mathison_turing};
    List l(v);
    ASSERT_EQ("[terence_tao, alan_mathison_turing]", l.symbol());
}

// Given there are two variables: X, Y
// When create a new list with the variables
// Then #symbol() of the list should return "[X, Y]"
TEST(List, Vars) {
    Variable X("X");
    Variable Y("Y");
    vector <Term *> v = {&X, &Y};
    List l(v);
    ASSERT_EQ("[X, Y]", l.symbol());
}

// ?- tom = [496, X, terence_tao].
// false.
TEST(List, matchToAtomShouldFail) {
    Atom tom("tom");
    Atom terence_tao("terence_tao");
    Number n496(496);
    Variable X("X");
    vector <Term *> v = {&n496, &X, &terence_tao};
    List l(v);
    ASSERT_FALSE(tom.match(l));
}

// ?- 8128 = [496, X, terence_tao].
// false.
TEST(List, matchToNumberShouldFail) {
    Number n8128(8128);
    Atom terence_tao("terence_tao");
    Number n496(496);
    Variable X("X");
    vector <Term *> v = {&n496, &X, &terence_tao};
    List l(v);
    ASSERT_FALSE(n8128.match(l));
}

// ?- s(X) = [496, X, terence_tao].
// false.
TEST(List, matchToStructShouldFail) {
    Variable X("X");
    vector <Term *> v_struc = {&X};
    Struct s(Atom("s"), v_struc);
    EXPECT_EQ("s(X)", s.symbol());
    
    Atom terence_tao("terence_tao");
    Number n496(496);
    vector <Term *> v = {&n496, &X, &terence_tao};
    List l(v);
    ASSERT_FALSE(s.match(l));
}

// ?- Y = [496, X, terence_tao].
// Y = [496, X, terence_tao].
TEST(List, matchToVarShouldSucceed) {
    Variable X("X");
    Atom terence_tao("terence_tao");
    Number n496(496);
    vector <Term *> v = {&n496, &X, &terence_tao};
    List l(v);
    Variable Y("Y");
    ASSERT_TRUE(Y.match(l));
    ASSERT_EQ("[496, X, terence_tao]", Y.value());
}

// ?- X = [496, X, terence_tao].
// false.
TEST(List, matchToVarOccuredInListShouldFail) {
    Variable X("X");
    Atom terence_tao("terence_tao");
    Number n496(496);
    vector <Term *> v = {&n496, &X, &terence_tao};
    List l(v);
    ASSERT_FALSE(X.match(l));
}

// ?- [496, X, terence_tao] = [496, X, terence_tao].
// true.
TEST(List, matchToSameListShouldSucceed) {
    Variable X("X");
    Atom terence_tao("terence_tao");
    Number n496(496);
    vector <Term *> v = {&n496, &X, &terence_tao};
    List l1(v);
    List l2(v);
    ASSERT_TRUE(l1.match(l2));
}

// ?- [496, X, terence_tao] = [496, Y, terence_tao].
// true.
TEST(List, matchToSameListWithDiffVarNameShouldSucceed) {
    Variable X("X"), Y("Y");
    Atom terence_tao("terence_tao");
    Number n496(496);
    vector <Term *> v1 = {&n496, &X, &terence_tao};
    vector <Term *> v2 = {&n496, &Y, &terence_tao};
    List l1(v1);
    List l2(v2);
    ASSERT_TRUE(l1.match(l2));
}

// ?- [496, X, terence_tao] = [496, 8128, terence_tao].
// X = 8128.
TEST(List, matchToVarToAtominListShouldSucceed) {
    Variable X("X");
    Atom terence_tao("terence_tao");
    Number n496(496), n8128(8128);
    vector <Term *> v1 = {&n496, &X, &terence_tao};
    vector <Term *> v2 = {&n496, &n8128, &terence_tao};
    List l1(v1);
    List l2(v2);
    EXPECT_TRUE(l1.match(l2));
    ASSERT_EQ("8128",X.value());
}

// ?- Y = [496, X, terence_tao], X = alan_mathison_turing.
// Y = [496, alan_mathison_turing, terence_tao],
// X = alan_mathison_turing.
TEST(List, matchVarinListToAtomShouldSucceed) {
    Variable X("X"), Y("Y");
    Atom terence_tao("terence_tao"), alan_mathison_turing("alan_mathison_turing");
    Number n496(496), n8128(8128);
    vector <Term *> v1 = {&n496, &X, &terence_tao};
    vector <Term *> v2 = {&n496, &alan_mathison_turing, &terence_tao};
    List l1(v1);
    List l2(v2);

    EXPECT_TRUE(Y.match(l1));
    EXPECT_EQ("[496, X, terence_tao]", Y.value());

    EXPECT_TRUE(X.match(alan_mathison_turing));
    EXPECT_EQ("alan_mathison_turing", X.value());

    EXPECT_TRUE(Y.match(l2));
    ASSERT_EQ("alan_mathison_turing", X.value());
}

// Example: 
// ?- [first, second, third] = [H|T].
// H = first, T = [second, third].
TEST(List, headAndTailMatching1) {
    Atom f("first"), s("second"), t("third");
    vector<Term *> args = {&f, &s, &t};
    List l(args);

    EXPECT_EQ(string("first"), l.head()->symbol());
    EXPECT_EQ(string("[second, third]"), l.tail()->value());
}

// Example:
// ?- [first, second, third] = [first, H|T].
// H = second, T = [third].
TEST(List, headAndTailMatching2) {
    Atom f("first"), s("second"), t("third");
    vector<Term *> args = {&f, &s, &t};
    List l(args);

    EXPECT_EQ(string("second"), l.tail()->head()->value());
    EXPECT_EQ(string("[third]"), l.tail()->tail()->value());
}

// ?- [[first], second, third] = [H|T].
// H = [first], T = [second, third].
TEST(List, headAndTailMatching3) {
    Atom f("first"), s("second"), t("third");
    vector<Term *> args1 = {&f};
    List l1(args1);
    vector<Term *> args2 = {&l1, &s, &t};
    List l2(args2);

    ASSERT_EQ("[first]", l2.head()->value());
    ASSERT_EQ("[second, third]", l2.tail()->value());
}

// ?- [first, second, third] = [first, second, H|T].
// H = third, T = [].
TEST(List, headAndTailMatching4) {
    Atom f("first"), s("second"), t("third");
    vector<Term *> args = {&f, &s, &t};
    List l(args);

    ASSERT_EQ("third", l.tail()->tail()->head()->value());
    ASSERT_EQ("[]", l.tail()->tail()->tail()->value());
}
 
// Given there is a empty list
// When client still want to get the head of list
// Then it should throw a string: "Accessing head in an empty list" as an exception.
TEST (List, emptyExecptionOfHead) {
    vector<Term *> args = {};
    List l(args);

    try {
        l.head();
    } catch (string exception) {
        EXPECT_EQ("Accessing head in an empty list", exception);
    }
}

// Given there is a empty list
// When client still want to get the head of list
// Then it should throw a string: "Accessing tail in an empty list" as an exception.
TEST (List, emptyExecptionOfTail) {
    vector<Term *> args = {};
    List l(args);

    try {
        l.tail();
    } catch (string exception) {
        EXPECT_EQ("Accessing tail in an empty list", exception);
    }
}




#endif
