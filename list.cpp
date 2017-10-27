#include "list.h"
#include "variable.h"
#include <string>
#include <iostream>
using std::string;

string List::symbol() const {
    if (0 == _elements.size()) {
        return "[]";
    } else {
        string symbol = "[";
        for (int i = 0; i < _elements.size(); i++) {
            symbol += _elements[i]->symbol();
            if (i != _elements.size() - 1) {
                symbol += ", ";
            }
        }
        symbol += "]";
        return symbol;
    }
}

string List::value() const {
    if (0 == _elements.size()) {
        return "[]";
    } else {
        string value = "[";
        for (int i = 0; i < _elements.size(); i++) {
            value += _elements[i]->value();
            if (i != _elements.size() - 1) {
                value += ", ";
            }
        }
        value += "]";
        return value;
    }
}

bool List::match(Term & term) {
    List * l = dynamic_cast<List *>(&term);
    if (l) {
        if (_elements.size() != l->_elements.size()) {
            return false;
        }
        for (int i = 0; i < _elements.size(); i++) {
            if (_elements[i]->value() != l->_elements[i]->value()) {
                Variable * var = dynamic_cast<Variable *>(_elements[i]);
                if (var) {
                    _elements[i]->match(*l->_elements[i]);
                    continue;
                }
                return false;
            }
        }
        return true;
    }
    Variable * v = dynamic_cast<Variable *>(&term);
    if (v) {
        return term.match(*this);
    }
    return false;
}

Term* List::element(int index) const {
    return _elements[index];
}

vector<Term *> List::elements() const {
    return _elements;
}

Term * List::head() const {
    if (_elements.empty()) {
        throw std::string("Accessing head in an empty list");
    }
    return _elements[0];
}

List * List::tail() const {
    if (_elements.empty()) {
        throw std::string("Accessing tail in an empty list");
    }
    vector<Term *> _elements_tail;
    _elements_tail.assign(_elements.begin() + 1, _elements.end());
    List *tail = new List(_elements_tail);
    return tail;
}
