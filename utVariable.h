#ifndef UTVARIABLE_H
#define UTVARIABLE_H
#include "variable.h"
#include "struct.h"
#include "atom.h"
#include "number.h"
#include <iostream>
using std::cout;

TEST(Variable, constructor){
  Variable X("X");
  ASSERT_EQ("X", X.symbol());
}

TEST(Variable , matching){
  Atom tom("tom");
  Variable X("X");
  X.match(tom);
  ASSERT_EQ( "tom", X.value());
}

TEST (Variable , haveValue){
  Atom tom ("tom");
  Atom jerry ("jerry");
  Variable X("X");
  ASSERT_TRUE(X.match(tom));
  ASSERT_FALSE(X.match(jerry));
}

// ?- X=2.7182.
// X=2.7182
TEST(Variable , numE_to_varX){
    Variable X("X");
    Number n2_7182(2.7182);
    X.match(n2_7182);
    ASSERT_EQ("2.7182", X.value());
}

// ?- X=Y, X=1.
// Y=1
TEST (Variable, varY_to_varX_and_num1_to_varX) {
    Variable X("X");
    Variable Y("Y");
    X.match(Y);
    EXPECT_EQ("Y", X.value());
    Number n1(1);
    X.match(n1);
    EXPECT_EQ("1", X.value());
    ASSERT_EQ("1", Y.value());
}

// ?- X=Y, Y=1.
// X=1
TEST (Variable, varY_to_varX_and_num1_to_varY) {
    Variable X("X");
    Variable Y("Y");
    X.match(Y);
    EXPECT_EQ("Y", X.value());
    Number n1(1);
    Y.match(n1);
    EXPECT_EQ("1", Y.value());
    ASSERT_EQ("1", X.value());
}

// ?- X=X, X=1.
// X=1
TEST (Variable, varX_match_varX_and_num1_to_varX) {
    Variable X("X");
    X.match(X);
    EXPECT_EQ("X", X.value());
    Number n1(1);
    X.match(n1);
    ASSERT_EQ("1", X.value());
}

// ?- Y=1, X=Y.
// X=1
TEST (Variable, num1_to_varY_and_varX_match_varY) {
    Variable X("X");
    Variable Y("Y");
    Number n1(1);
    Y.match(n1);
    EXPECT_EQ("1", Y.value());
    Y.match(X);
    ASSERT_EQ("1", X.value());
}

// ?- X=Y, Y=Z, Z=1
// X=1, Y=1, Z=1
TEST (Variable, num1_to_varZ_to_varY_to_varX) {
    Variable X("X");
    Variable Y("Y");
    Variable Z("Z");
    Number n1(1);

    X.match(Y);
    EXPECT_EQ("Y", X.value());
    Y.match(Z);
    EXPECT_EQ("Z", Z.value());
    Z.match(n1);
    ASSERT_EQ("1", X.value());
    ASSERT_EQ("1", Y.value());
    ASSERT_EQ("1", Z.value());
}

// ?- X=Y, X=Z, Z=1
// X=1, Y=1, Z=1
TEST (Variable, num1_to_varZ_to_varX_and_varY_to_varX) {
    Variable X("X");
    Variable Y("Y");
    Variable Z("Z");
    Number n1(1);

    X.match(Y);
    EXPECT_EQ("Y", X.value());
    EXPECT_EQ("Y", Y.value());
    X.match(Z);
    EXPECT_EQ("Z", Y.value());
    EXPECT_EQ("Z", X.value());
    EXPECT_EQ("Z", Z.value());
    Z.match(n1);
    ASSERT_EQ("1", X.value());
    ASSERT_EQ("1", Y.value());
    ASSERT_EQ("1", Z.value());
}

// Give there is a Struct s contains Variable X
// And another Variable Y
// When Y matches Struct s
// Then #symbol() of Y should return "Y"
// And #value() of Y should return "s(X)"
TEST (Variable, Struct1) {
    Variable X("X");
    std::vector<Term*> v = {&X};
    Variable Y("Y");
    Struct s(Atom("s"), v);
    Y.match(s);
    EXPECT_EQ("s(X)", Y.value());
}

// Give there is a Struct s contains Variable X
// And another Variable Y
// When Y matches Struct s
// And X matches Atom "teddy"
// Then #symbol() of Y should return "Y"
// And #value() of Y should return "s(teddy)"
TEST (Variable, Struct2) {
    Variable X("X");
    std::vector<Term*> v = {&X};
    Struct s(Atom("s"), v);
    Variable Y("Y");
    Y.match(s);
    EXPECT_EQ("s(X)", Y.value());
    Atom teddy("teddy");
    X.match(teddy);
    EXPECT_EQ("teddy", X.value());
    ASSERT_EQ("Y", Y.symbol());
    ASSERT_EQ("s(teddy)", Y.value());
}

#endif
