#include "struct.h"
#include "variable.h"

Struct::Struct(Atom const & name, vector<Term*> args):_name(name), _args(args) {}

Atom const & Struct::name() { return _name; }

Term* Struct::args(int index) const { return _args[index]; }

int Struct::arity() const { return _args.size(); }

string Struct::symbol() const {
    string sb = _name.symbol() + "(";
    for (int i = 0; i < _args.size(); i++) {
        sb += _args[i]->symbol();
        if (i != _args.size() - 1) {
            sb += ", ";
        }
    }
    sb += ")";
    return sb;
}

string Struct::value() const {
    string vl = _name.value() + "(";
    for (int i = 0; i < _args.size(); i++) {
        vl += _args[i]->value();
        if (i != _args.size() - 1) {
            vl += ", ";
        }
    }
    vl += ")";
    return vl;
}

bool Struct::match(Term& term) {
    Struct * ps = dynamic_cast<Struct *>(&term);
    if (ps){
        if (!_name.match(ps->_name))
            return false;
        if(_args.size()!= ps->_args.size())
            return false;
        for(int i=0;i<_args.size();i++){
            if(_args[i]->symbol() != ps->_args[i]->symbol())
                return false;
        }
        return true;
    }
    return false;
}
