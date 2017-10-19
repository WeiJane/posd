#ifndef STRUCT_H
#define STRUCT_H

//#include <sstream>
#include <string>
#include <vector>
#include "atom.h"
#include "term.h"

//using std::ostringstream;
using std::string;
using std::vector;

class Struct : public Term {
public:
    Struct(Atom const &,vector<Term*>);
    Atom const & name();
    Term* args(int) const;
    string symbol() const;
    string value() const;
    int size() const;
    bool match(Term&);

private:
    Atom _name;
    vector<Term*> _args;
};

#endif
