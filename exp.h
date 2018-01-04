#ifndef EXP_H
#define EXP_H

#include "atom.h"

#include "variable.h"
#include "global.h"
#include "scanner.h"
#include "struct.h"
#include "list.h"

#include <map>
using std::map;

map<string,Variable*> _table;

class Exp {
public:
  virtual bool evaluate() = 0;
  virtual string getExpString() = 0;
};


class MatchExp : public Exp {
public:
  MatchExp(Term* left, Term* right): _left(left), _right(right){
    _table.clear();
  }

  bool evaluate(){
    return _left->match(*_right);
  }
  string getExpString(){
    if (evaluate()) {
      Variable  *var = _left->getVariable();
      Variable  *var_r = _right->getVariable();
      if ((var == var_r) && var != nullptr && (_table.find(var->symbol()) == _table.end())) {
          _table.insert(pair<string, Variable *>(var->symbol(), var));
          return "true";
      }
      if (var && (_table.find(var->symbol()) == _table.end() ) ) {
        _table.insert(pair<string, Variable *>(var->symbol(), var));
        return _left->symbol() + " = " + _right->value() ;
      }  
      else return "";
    }
    return "false";
  }

private:
  Term* _left;
  Term* _right;
};

class ConjExp : public Exp {
public:
  ConjExp(Exp *left, Exp *right) : _left(left), _right(right) {
    _table.clear();
  }

  bool evaluate() {
    return (_left->evaluate() && _right->evaluate());
  }

  string getExpString(){
    if (evaluate()) {
      string left = "";
      string right = "";

      left = _left->getExpString();
      if (left == "true") {
          left = "";
      } else if (left != "false") {
          left = left + ", ";
      }

      right = _right->getExpString();
      if (right == "true" ) {
          right = "";
      }
      if (right == "" && left != "false" ) {
          left = left.substr(0, left.length() - 2);
      }
      if ((left +  right) != "") {
          return (left +  right);
      } else {
          return "true";
      }
    }
    return "false";
  }

private:
  Exp * _left;
  Exp * _right;
};

class DisjExp : public Exp {
public:
  DisjExp(Exp *left, Exp *right) : _left(left), _right(right) {
    _table.clear();
  }

  bool evaluate() {
    return (_left->evaluate() || _right->evaluate());
  }

  string getExpString() {
    if (evaluate()) {
      string left = "";
      string right = "";
      
      left = _left->getExpString();
      if (_left->evaluate() == false) {
          left = "";
      }
      if (left != "") {
          left = left + "; ";
      }
      _table.clear();
      
      right = _right->getExpString();
      if (_right->evaluate() == false) {
          right = "";
      }
      if (right == "" && left != "" ) {
          left = left.substr(0, left.length() - 2);
      }
      return (left +  right);
    }
    return "false";
  }

private:
  Exp * _left;
  Exp * _right;
};
#endif
