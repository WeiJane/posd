#ifndef NUMBER_H
#define NUMBER_H

#include "term.h"
#include <string>
#include <sstream>
using std::string;
using std::ostringstream;

class Number : public Term{
public:
    Number(double db){
        std::ostringstream strs;
        strs << db;
        _symbol = strs.str();
    }
};

#endif
