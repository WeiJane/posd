#include "struct.h"
#include "iterator.h"

Iterator <Term*>* Struct::createIterator(){
    return new StructIterator <Term*>(this);
}

Iterator <Term*>* Struct::createDFSIterator(){
    return new DFSIterator <Term*>(this);
}

Iterator <Term*>* Struct::createBFSIterator(){
    return new BFSIterator <Term*>(this);
}

bool Struct::match( Term &term ){
    return false;
}

string Struct::symbol() const {
  if(_args.empty())
  return  _name.symbol() + "()";
  string ret = _name.symbol() + "(";
  std::vector<Term *>::const_iterator it = _args.begin();
  for (; it != _args.end()-1; ++it)
    ret += (*it)->symbol()+", ";
  ret  += (*it)->symbol()+")";
  return ret;
}
