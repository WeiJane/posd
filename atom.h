#ifndef ATOM_H
#define ATOM_H

#include "term.h"
#include <string>

using std::string;

class Atom : public Term{
public:
    Atom(string);
    string symbol() const;
    bool match(Term&);

private:
    string _symbol;
};


#endif
