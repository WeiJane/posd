#include "atom.h"
#include "variable.h"
#include "struct.h"

Atom::Atom(string s): _symbol(s) {}

string Atom::symbol() const { return _symbol; }

bool Atom::match(Term& term) {
    /*if (dynamic_cast<Variable*>(&term) != nullptr ||
        dynamic_cast<Struct*>(&term) != nullptr) {
        return term.match(*this);
    }*/
    Struct * st = dynamic_cast<Struct *>(&term);
    Variable * vr = dynamic_cast<Variable *>(&term);

    /*if (dynamic_cast<Variable*>(&term) ||
        dynamic_cast<Struct*>(&term)) {
        return term.match(*this);
    }*/
    if (st || vr) return term.match(*this);
    return value() == term.value();
}

