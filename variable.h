#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include "atom.h"
using std::string;

class Variable: public Term{
public:
  Variable(string s):_symbol(s){}
  string const _symbol;
  string value(){ return _value; }
  string symbol(){ return _symbol; }
  bool match(Term & term){
  }

private:
  string _value;
  bool _assignable = true;
};

#endif
