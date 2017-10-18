#ifndef NUMBER_H
#define NUMBER_H

#include "term.h"
#include <string>
#include <sstream>
#include <iostream>
using std::string;
using std::stringstream;

class Number: public Term{
    public:
        Number(double value):_value(value) {}
        string value() const{
            stringstream ss;
            ss << _value;
            string val;
            ss >> val;
            return val;
        }
        string symbol() const{
            return value();
        }
        //bool match(Term & term) {
        //
        //}
    private:
        string _symbol;
        double _value;
};

#endif
