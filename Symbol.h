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

    enum ScopeType { GLOBAL, LOCAL };

    friend ostream &operator<<(ostream &, Symbol &);

    Symbol(string _name = "", bool _defined = false, SectionType _section = SectionType::GLOBAL, ScopeType _scope = ScopeType::LOCAL, unsigned long _offset = 0);

    string name;
    bool defined;
    unsigned long offset;
    SectionType section;
    ScopeType scope;

};

#endif //SS_SYMBOL_H
