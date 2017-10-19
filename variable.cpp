#include "variable.h"

Variable::Variable(string s) {
    _symbol = s;
    _term = nullptr;
}

string Variable::symbol() const { return _symbol; }

string Variable::value() const {
    if (_term == nullptr) {
        return _symbol;
    }
    return _term->value();
}

bool Variable::match(Term& term) {
    if (_term == nullptr) {
        if (this != &term) {
            _term = &term;
        }
        return true;
    }
    return _term->match(term);
}
