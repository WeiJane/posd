#include "variable.h"
#include "list.h"

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
    List * l = dynamic_cast<List *>(&term);
    if (_term == nullptr && l) {
        bool matchable = true;
        for (int i = 0; i< l->elements().size(); i++) {
            if (l->element(i)->symbol() == symbol()) {
                matchable = false;
                break;
            } else {
                _term = &term;
            }
        }
        return matchable;
    }
    if (_term == nullptr && !l) {
        if (this != &term) {
            _term = &term;
        }
        return true;
    }
    return _term->match(term);
}
