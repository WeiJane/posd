#ifndef ATOM_H
#define ATOM_H

#include <string>
#include <sstream>
using std::string;

template <class T>
class Iterator;
class Term{
public:
  virtual string symbol() const {return _symbol;}
  virtual string value() const {return symbol();}
  virtual bool match(Term & a);
  virtual Iterator<Term*>* createIterator();
  virtual Iterator<Term*>* createDFSIterator();
  virtual Iterator<Term*>* createBFSIterator();
protected:
  Term (string s = ""):_symbol(s) {}
  string _symbol;
};

class Atom : public Term{
public:
  Atom(string s):Term(s) {}
};

#endif
