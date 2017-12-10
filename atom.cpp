#include "atom.h"
#include "variable.h"
#include "iterator.h"
#include <typeinfo>

Iterator <Term*>* Term::createIterator(){
    return new NullIterator <Term*>(this);
}

Iterator <Term*>* Term::createDFSIterator(){
    return new NullIterator <Term*>(this);
}

Iterator <Term*>* Term::createBFSIterator(){
    return new NullIterator <Term*>(this);
}

bool Term::match(Term & a){
  if (typeid(a) ==  typeid(Variable))
    return a.match(*this);
  else
    return symbol() == a.symbol();
}
