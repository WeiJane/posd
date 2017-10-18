#ifndef ATOM_H
#define ATOM_H

#include "term.h"
#include <string>

using std::string;

class Atom : public Term{
public:
  Atom (string s):_symbol(s) {}

  string symbol() const{
    return _symbol;
  }

  string _symbol;
};


#endif
