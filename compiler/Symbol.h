//
// Created by rols on 6/4/16.
//

#ifndef SS_SYMBOL_H
#define SS_SYMBOL_H

#include <iostream>
#include <unordered_map>

#include "Enums.h"

using namespace std;

class Symbol
{
public:


    friend ostream &operator<<(ostream &, Symbol &);

    Symbol(string _name, bool _defined, SectionType _section, string _sectionName, ScopeType _scope, unsigned long);

    string name;
    bool defined;
    unsigned long offset;
    SectionType section;
    string sectionName;
    ScopeType scope;

    void Out(ostream& out);

};

#endif //SS_SYMBOL_H
