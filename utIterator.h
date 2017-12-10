#ifndef UTITERATOR_H
#define UTITERATOR_H

#include "struct.h"
#include "variable.h"
#include "atom.h"
#include "list.h"
#include "iterator.h"
TEST(iterator, first) {
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    Struct t(Atom("t"), { &X, &two });
    Struct s(Atom("s"), { &one, &t, &Y });
    // StructIterator it(&s);
    Iterator <Term*> *itStruct = s.createIterator();
    // Iterator& itStruct = it;
    // ASSERT_EQ(it.first()->symbol());
    itStruct->first();
    ASSERT_EQ("1", itStruct->currentItem()->symbol());
    ASSERT_FALSE(itStruct->isDone());
    itStruct->next();
    ASSERT_EQ("t(X, 2)", itStruct->currentItem()->symbol());
    ASSERT_FALSE(itStruct->isDone());
    itStruct->next();
    ASSERT_EQ("Y", itStruct->currentItem()->symbol());
    itStruct->next();
    ASSERT_TRUE(itStruct->isDone());
}

TEST(iterator, nested_iterator) {
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    Struct t(Atom("t"), { &X, &two });
    Struct s(Atom("s"), { &one, &t, &Y });
    Iterator <Term*> *it = s.createIterator();
    it->first();
    it->next();
    Struct *s2 = dynamic_cast<Struct *>(it->currentItem());

    Iterator <Term*> *it2 = s2->createIterator();
    it2->first();
    ASSERT_EQ("X", it2->currentItem()->symbol());
    ASSERT_FALSE(it2->isDone());
    it2->next();
    ASSERT_EQ("2", it2->currentItem()->symbol());
    ASSERT_FALSE(it2->isDone());
    it2->next();
    ASSERT_TRUE(it2->isDone());
}

TEST(iterator, firstList) {
    Number one(1);
    Variable X("X");
    Variable Y("Y");
    Number two(2);
    Struct t(Atom("t"), { &X, &two });
    List l({ &one, &t, &Y });
    ListIterator <Term*> it(&l);
    Iterator <Term*> * itList = &it;
    itList->first();
    ASSERT_EQ("1", itList->currentItem()->symbol());
    ASSERT_FALSE(itList->isDone());
    itList->next();
    ASSERT_EQ("t(X, 2)", itList->currentItem()->symbol());
    ASSERT_FALSE(itList->isDone());
    itList->next();
    ASSERT_EQ("Y", itList->currentItem()->symbol());
    itList->next();
    ASSERT_TRUE(itList->isDone());
}

TEST(iterator, NullIterator){
  Number one(1);
  NullIterator <Term*> nullIterator(&one);
  nullIterator.first();
  ASSERT_TRUE(nullIterator.isDone());
  Iterator <Term *> * it = one.createIterator();
  it->first();
  ASSERT_TRUE(it->isDone());
}

//s(1, t(X), Y)
// 1 > t(X) > Y > X
TEST(iterator, struct_strunctBFSIterator) {
    Number one(1);
    Variable X("X"), Y("Y");
    Struct t(Atom("t"), {&X});
    Struct s(Atom("s"), {&one, &t, &Y});

    Iterator<Term*> *it = s.createBFSIterator();
    it->first();
    ASSERT_EQ("1", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("t(X)", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("Y", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("X", it->currentItem()->symbol());
    it->next();
    ASSERT_TRUE(it->isDone());
}

//s(1, t(X), Y)
// 1 > t(X) > X > Y
TEST(iterator, struct_strunctDFSIterator) {
    Number one(1);
    Variable X("X"), Y("Y");
    Struct t(Atom("t"), {&X});
    Struct s(Atom("s"), {&one, &t, &Y});

    Iterator<Term*> *it = s.createDFSIterator();
    it->first();
    ASSERT_EQ("1", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("t(X)", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("X", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("Y", it->currentItem()->symbol());
    it->next();
    ASSERT_TRUE(it->isDone());
}


//s(1, [X, 2], Y)
// 1 > [X, 2] > Y > X > 2
TEST(iterator, struct_listBFSIterator) {
    Number one(1), two(2);
    Variable X("X"), Y("Y");
    List l({&X, &two});
    Struct s(Atom("s"), {&one, &l, &Y});

    Iterator<Term*> *it = s.createBFSIterator();
    it->first();
    ASSERT_EQ("1", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("[X, 2]", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("Y", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("X", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("2", it->currentItem()->symbol());
    it->next();
    ASSERT_TRUE(it->isDone());
}

//s(1, [X, 2], Y)
// 1 > [X, 2] > X > 2 > Y
TEST(iterator, struct_listDFSIterator) {
    Number one(1), two(2);
    Variable X("X"), Y("Y");
    List l({&X, &two});
    Struct s(Atom("s"), {&one, &l, &Y});

    Iterator<Term*> *it = s.createDFSIterator();
    it->first();
    ASSERT_EQ("1", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("[X, 2]", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("X", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("2", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("Y", it->currentItem()->symbol());
    it->next();
    ASSERT_TRUE(it->isDone());
}

//[1, s(X), 2]
// 1 > s(X) > 2 > X
TEST(iterator, list_structBFSIterator) {
    Number one(1), two(2);
    Variable X("X");
    Struct s(Atom("s"), {&X});
    List l({&one, &s, &two});

    Iterator<Term*> *it = l.createBFSIterator();
    it->first();
    ASSERT_EQ("1", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("s(X)", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("2", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("X", it->currentItem()->symbol());
    it->next();
    ASSERT_TRUE(it->isDone());
}

//[1, s(X), 2])
// 1 > s(X) > X > 2
TEST(iterator, list_structDFSIterator) { 
    Number one(1), two(2);
    Variable X("X");
    Struct s(Atom("s"), {&X});
    List l({&one, &s, &two});

    Iterator<Term*> *it = l.createDFSIterator();
    it->first();
    ASSERT_EQ("1", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("s(X)", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("X", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("2", it->currentItem()->symbol());
    it->next();
    ASSERT_TRUE(it->isDone());
}

//[1, [X, 2], Y]
// 1 > [X, 2] > Y > X > 2
TEST(iterator, list_listBFSIterator) {
    Number one(1), two(2);
    Variable X("X"), Y("Y");
    List l1({&X, &two});
    List l2({&one, &l1, &Y});

    Iterator<Term*> *it = l2.createBFSIterator();
    it->first();
    ASSERT_EQ("1", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("[X, 2]", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("Y", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("X", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("2", it->currentItem()->symbol());
    it->next();
    ASSERT_TRUE(it->isDone());
}


//[1, [X, 2], Y]]
// 1 > [X, 2] > X > 2 > Y
TEST(iterator, list_listDFSIterator) {
    Number one(1), two(2);
    Variable X("X"), Y("Y");
    List l1({&X, &two});
    List l2({&one, &l1, &Y});

    Iterator<Term*> *it = l2.createDFSIterator();
    it->first();
    ASSERT_EQ("1", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("[X, 2]", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("X", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("2", it->currentItem()->symbol());
    it->next();
    ASSERT_EQ("Y", it->currentItem()->symbol());
    it->next();
    ASSERT_TRUE(it->isDone());
}

#endif
