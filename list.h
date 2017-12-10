#ifndef LIST_H
#define LIST_H

#include "atom.h"
#include <vector>
#include <string>
#include <typeinfo>
#include <iostream>
using std::vector;

template <class T>
class Iterator;
class Variable ;

class List : public Term {
public:
  string symbol() const ;
  string value() const ;
  bool match(Term & term) ;
public:
  List (): _elements(0) {}
  List (vector<Term *> const & elements):_elements(elements){}
  Term * head() const;
  List * tail() const;
  Term * args(int index) {
    return _elements[index];
  }
  int arity() const {return _elements.size();}
  Iterator <Term*>* createIterator();
  Iterator <Term*>* createDFSIterator();
  Iterator <Term*>* createBFSIterator();
private:
  vector<Term *> _elements;
};

#endif
