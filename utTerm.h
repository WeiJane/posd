#ifndef UTTERM_H
#define UTTERM_H

#include "variable.h"
#include "atom.h"
#include "number.h"
#include <string>
#include <iostream>
using std::string;
using std::cout;


//test Number.value()
TEST (Number, ctor) {
    Number n12(12);
   ASSERT_EQ( "12", n12.value());
}
//test Number.symbol()
TEST (Number, symbol) {
   Number n23(23);
   ASSERT_EQ( "23", n23.symbol());
}
//?- 25 = 25.
// true.
TEST (Number, matchSuccess) {
    Number n25(25);
    ASSERT_TRUE(n25.match(n25));
}
//?- 25 = 0.
// false.
TEST (Number, matchFailureDiffValue) {
    Number n25(25);
    Number n0(0);
    ASSERT_FALSE(n25.match(n0));
}
//?- 25 = tom.
// false.
TEST (Number, matchFailureDiffConstant) {
    Number n25(25);
    Atom tom("tom");
    ASSERT_FALSE(n25.match(tom));
}
//?- 25 = X.
// true.
TEST (Number, matchSuccessToVar) {
    Number n25(25);
    Variable X("X");
    ASSERT_TRUE(n25.match(X));
    ASSERT_EQ(n25.value(), X.value());
}

//?- tom = 25.
// false.
TEST (Atom, matchFailureDiffConstant) {
    Atom tom("tom");
    Number n25(25);
    ASSERT_FALSE(tom.match(n25));
}

// ?- tom = X.
// X = tom.
TEST (Atom, matchSuccessToVar) {
    Atom tom("tom");
    Variable X("X");
    EXPECT_TRUE(tom.match(X));
    ASSERT_EQ(X.value(), tom._symbol);
}

// ?- X = tom, tom = X.
// X = tom.
TEST (Atom, matchSuccessToVarInstantedToDiffConstant) {
    Variable X("X");
    Atom tom("tom");
    EXPECT_TRUE(X.match(tom));
    EXPECT_TRUE(tom.match(X));
    ASSERT_EQ(X.value(), tom._symbol);
}

// ?- X = jerry, tom = X.
// false.
TEST (Atom, matchFailureToVarInstantedToDiffConstant) {
    Variable X("X");
    Atom tom("tom");
    Atom jerry("jerry");
    EXPECT_TRUE(X.match(jerry));
    EXPECT_EQ(X.value(), jerry._symbol);
    ASSERT_FALSE(tom.match(X));
}

// ?- X = 5.
// X = 5.
TEST (Variable, matchSuccessToNumber) {
    Variable X("X");
    Number n5(5);
    EXPECT_TRUE(X.match(n5));
    ASSERT_EQ(X.value(), n5.value());
}

// ?- X = 25, X = 100.
// false.
TEST (Variable, matchFailureToTwoDiffNumbers) {
    Variable X("X");
    Number n25(25);
    Number n100(100);
    EXPECT_TRUE(X.match(n25));
    ASSERT_FALSE(X.match(n100));
}

// ?- X = tom, X = 25.
// false.
TEST (Variable, matchSuccessToAtomThenFailureToNumber) {
    Variable X("X");
    Atom tom("tom");
    Number n25(25);
    EXPECT_TRUE(X.match(tom));
    EXPECT_EQ(X.value(), tom._symbol);
    ASSERT_FALSE(X.match(n25));
}
//?- tom = X, 25 = X.
// false.
TEST (Variable, matchSuccessToAtomThenFailureToNumber2) {
    Variable X("X");
    Atom tom("tom");
    Number n25(25);
    EXPECT_TRUE(tom.match(X));
    EXPECT_EQ(tom._symbol, X.value());
    ASSERT_FALSE(n25.match(X));
}
//?- X = tom, X = tom.
// true.
TEST(Variable, reAssignTheSameAtom){
    Variable X("X");
    Atom tom("tom");
    EXPECT_TRUE(X.match(tom));
    EXPECT_EQ(X.value(), tom._symbol);
    ASSERT_TRUE(X.match(tom));
}
#endif
