#include "atom.h"
#include "variable.h"
#include "struct.h"

Atom::Atom(string s): _symbol(s) {}

string Atom::symbol() const { return _symbol; }

bool Atom::match(Term& term) {
    Struct * st = dynamic_cast<Struct *>(&term);
    Variable * vr = dynamic_cast<Variable *>(&term);

    if (st || vr) return term.match(*this);
    return value() == term.value();
}

