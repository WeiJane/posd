#ifndef NUMBER_H
#define NUMBER_H

#include "term.h"
#include <string>
using std::string;

class Number: public Term{
    public:
        Number(double);
        string value() const;
        string symbol() const;
        bool match(Term &);
    private:
        double _value;
};

#endif
