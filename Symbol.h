//
// Created by rols on 6/4/16.
//

#ifndef SS_SYMBOL_H
#define SS_SYMBOL_H

#include "SectionType.h"

#include <iostream>
#include <unordered_map>

using namespace std;

class Symbol
{
public:
    enum Scope { GLOBAL, LOCAL };

    friend ostream &operator<<(ostream &, Symbol &);

    string name;
    bool defined;
    unsigned long VA;
    SectionType section;
    Scope type;

    Symbol(string _name, bool _defined, SectionType _section, Scope _type, unsigned long _VA);
};

#endif //SS_SYMBOL_H
