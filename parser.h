#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <iostream>
#include <algorithm>
using std::string;
using std::cout;
using std::find;

#include "atom.h"
#include "variable.h"
#include "global.h"
#include "scanner.h"
#include "struct.h"
#include "list.h"
#include "node.h"

#include "utParser.h"

class Parser{
public:
  Parser(Scanner scanner) : _scanner(scanner), _terms(){}

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
      throw string("unexpected token");
    }
  }

  Term * list() {
    int startIndexOfListArgs = _terms.size();
    createTerms();
    if(_currentToken == ']')
    {
      vector<Term *> args(_terms.begin() + startIndexOfListArgs, _terms.end());
      _terms.erase(_terms.begin() + startIndexOfListArgs, _terms.end());
      return new List(args);
    } else {
      throw string("unexpected token");
    }
  }

  vector<Term *> & getTerms() {
    return _terms;
  }

  Node * expressionTree() {
    return _tree;
  }

  void matchings() {
      while (_scanner.currentChar() != '.') {
          Term * term = createTerm();
          if (term != nullptr) {
              _terms.push_back(term);
              Node * termNode = new Node(TERM, term, nullptr, nullptr);
              if (_tree == nullptr) {
                  _tree = termNode;
              } else {
                  Node ** origin = &_tree;
                  while ((**origin).right != nullptr) {
                      origin = &(**origin).right;  
                  }
                  (**origin).right = termNode;
              }
          }
          if (_scanner.currentChar() == '=') {
              Node * eqNode = new Node(EQUALITY);
              Node ** origin = &_tree;
              while ((**origin).right != nullptr) {
                  origin = &(**origin).right;
              }
              eqNode->left = (*origin);
              (*origin) = eqNode;
          }
          if (_scanner.currentChar() == ',') {
              Node * comaNode = new Node(COMMA);
              Node ** origin = &_tree;
              while ((**origin).payload == COMMA) {
                  origin = &(**origin).right;
              }
              comaNode->left = (*origin);
              (*origin) = comaNode;
          }
          if (_scanner.currentChar() == ';') {
              _term_symbol_table.clear();
              Node * semiNode = new Node(SEMICOLON);
              Node ** origin = &_tree;
              semiNode->left = (*origin);
              (*origin) = semiNode;
          }
      }
  }

  vector<Term *> _term_symbol_table;

private:
  FRIEND_TEST(ParserTest, createArgs);
  FRIEND_TEST(ParserTest,ListOfTermsEmpty);
  FRIEND_TEST(ParserTest,listofTermsTwoNumber);
  FRIEND_TEST(ParserTest, createTerm_nestedStruct3);

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
  Node * _tree = nullptr;
};
#endif
