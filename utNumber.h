#ifndef UTNUMBER_H
#define UTNUMBER_H

#include "number.h"
#include <string>
#include <iostream>
using std::string;
using std::cout;

//test Number.value()
TEST (Number, ctor) {
    Number n12(12.12);
    ASSERT_EQ( "12", n12.value());
}
//test Number.symbol()
TEST (Number, symbol) {
    Number n23(23.3);
    ASSERT_EQ( "23", n23.symbol());
}
//?- 25 = 25.
// true.
TEST (Number, matchSuccess) {
    Number n25(25.2);
    ASSERT_TRUE(n25.match(n25));
}
//?- 25 = 0.
// false.
TEST (Number, matchFailureDiffValue) {
    Number n25(25);
    Number n0(25.5);
    ASSERT_FALSE(n25.match(n0));
}

#endif
