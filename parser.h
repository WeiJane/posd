#ifndef PARSER_H
#define PARSER_H
#include <string>
using std::string;

#include "atom.h"
#include "variable.h"
#include "global.h"
#include "scanner.h"
#include "struct.h"
#include "list.h"
#include "exp.h"
#include <stack>

using std::stack;

class Parser{
public:
  Parser(Scanner scanner) : _scanner(scanner), _terms() {}

  Term* createTerm(){
      int token = _scanner.nextToken();
      _currentToken = token;
      if(token == VAR){
          Variable * var = new Variable(symtable[_scanner.tokenValue()].first);
          if (_term_symbol_table.empty()) {
              _term_symbol_table.push_back(var);
          } else {
              for (int i = 0; i < _term_symbol_table.size(); i++) {
                  Variable * term = dynamic_cast<Variable *> (_term_symbol_table[i]);
                  if (term && _term_symbol_table[i]->symbol() == var->symbol()) {
                      var = term;
                      break;
                  }
                  if (i == _term_symbol_table.size() - 1) {
                      _term_symbol_table.push_back(var);
                  }
              }
          }
          return var;
      }else if(token == NUMBER){
          Number * num = new Number(_scanner.tokenValue());
          if (_term_symbol_table.empty()) {
              _term_symbol_table.push_back(num);
          } else {
              for (int i = 0; i < _term_symbol_table.size(); i++) {
                  Number * term = dynamic_cast<Number *> (_term_symbol_table[i]);
                  if (term && _term_symbol_table[i]->symbol() == num->symbol()) {
                      num = term;
                      break;
                  }
                  if (i == _term_symbol_table.size() - 1) {
                      _term_symbol_table.push_back(num);
                  }
              }
          }
          return num;
      }else if(token == ATOM || token == ATOMSC){
          Atom* atom = new Atom(symtable[_scanner.tokenValue()].first);
          if(_scanner.currentChar() == '(' ) {
              return structure();
          }
          else {
              if (_term_symbol_table.empty()) {
                  _term_symbol_table.push_back(atom);
              } else {
                  for (int i = 0; i < _term_symbol_table.size(); i++) {
                      Atom * term = dynamic_cast<Atom *> (_term_symbol_table[i]);
                      if (term && _term_symbol_table[i]->symbol() == atom->symbol()) {
                          atom = term;
                          break;
                      }
                      if (i == _term_symbol_table.size() - 1) {
                          _term_symbol_table.push_back(atom);
                      }
                  }
              }
              return atom;
          }
      }
      else if(token == '['){
          return list();
      }

      return nullptr;
  }



  Term * structure() {
    Atom structName = Atom(symtable[_scanner.tokenValue()].first);
    int startIndexOfStructArgs = _terms.size();
    _scanner.nextToken();
    createTerms();
    if(_currentToken == ')')
    {
      vector<Term *> args(_terms.begin() + startIndexOfStructArgs, _terms.end());
      _terms.erase(_terms.begin() + startIndexOfStructArgs, _terms.end());
      return new Struct(structName, args);
    } else {
      throw string("Unbalanced operator");
    }
  }

  Term * list() {
    int startIndexOfListArgs = _terms.size();
    createTerms();
    if(_currentToken == ']')
    {
      vector<Term *> args(_terms.begin() + startIndexOfListArgs, _terms.end());
      _terms.erase(_terms.begin() + startIndexOfListArgs, _terms.end());
      if(args.size()==0){
        return new Atom("[]");
      }
      return new List(args);
    } else {
      throw string("Unbalanced operator");
    }
  }

  vector<Term *> & getTerms() {
    return _terms;
  }

  void buildExpression(){
    disjunctionMatch();
    restDisjunctionMatch();
    if (createTerm() != nullptr || _currentToken == EOS){
        throw string("Missing token '.'");
    } else if( _currentToken != '.'){
        string msg = string("Unexpected '");
        msg += (char)_currentToken;
        msg += "' before '.'";
        throw msg;
    }
  }

  void restDisjunctionMatch() {
    if (_scanner.currentChar() == ';') {
      _term_symbol_table.clear();
      createTerm();
      disjunctionMatch();
      Exp *right = _expStack.top();
      _expStack.pop();
      Exp *left = _expStack.top();
      _expStack.pop();
      _expStack.push(new DisjExp(left, right));
      restDisjunctionMatch();
    }
  }

  void disjunctionMatch() {
    conjunctionMatch();
    restConjunctionMatch();
  }

  void restConjunctionMatch() {
      if (_scanner.currentChar() == ',') {
          createTerm();
          conjunctionMatch();
          Exp *right = _expStack.top();
          _expStack.pop();
          Exp *left = _expStack.top();
          _expStack.pop();
          _expStack.push(new ConjExp(left, right));
          restConjunctionMatch();
      }
  }

  void conjunctionMatch() {
      if (_scanner.currentChar() == '.') {
          string msg = string("Unexpected '");
          msg += (char)_currentToken;
          msg += "' before '.'";
          throw msg;
      }
      Term * left = createTerm();
      if (createTerm() == nullptr) {
          if ( _currentToken == '='){
              Term * right = createTerm();
              _expStack.push(new MatchExp(left, right));
          }else if ( _currentToken == '.'){
              string msg = string( left->symbol() + " does never get assignment");
              throw msg;
          }else{
              string msg = string("Unexpected '");
              msg += (char)_currentToken;
              msg += "' before '.'";
              throw msg;
          }
      }
  }

  Exp* getExpressionTree(){
    return _expStack.top();
  }

private:
  FRIEND_TEST(ParserTest, createArgs);
  FRIEND_TEST(ParserTest,ListOfTermsEmpty);
  FRIEND_TEST(ParserTest,listofTermsTwoNumber);
  FRIEND_TEST(ParserTest, createTerm_nestedStruct3);
  FRIEND_TEST(ParserTest, createTerms);

  void createTerms() {
    Term* term = createTerm();
    if(term!=nullptr)
    {
      _terms.push_back(term);
      while((_currentToken = _scanner.nextToken()) == ',') {
        _terms.push_back(createTerm());
      }
    }
  }

  vector<Term *> _terms;
  Scanner _scanner;
  int _currentToken;
  //MatchExp* _root;
  stack<Exp*> _expStack;
  vector<Term *> _term_symbol_table;
};
#endif
