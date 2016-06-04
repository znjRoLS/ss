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

    string name;
    bool defined;
    int value;
    SectionType section;
    Scope type;

    Symbol(string _name, bool _defined, SectionType _section, Scope _type);
};

#endif //SS_SYMBOL_H
