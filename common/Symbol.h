//
// Created by rols on 6/4/16.
//

#ifndef SS_SYMBOL_H
#define SS_SYMBOL_H

#include <iostream>
#include <unordered_map>
#include <sstream>

#include "Enums.h"

using namespace std;

class Symbol
{
public:


    friend ostream &operator<<(ostream &, Symbol &);

    Symbol(string _name, bool _defined, string _sectionName, ScopeType _scope, unsigned long, int size = 0);

    string name;
    bool defined;
    unsigned long offset;
    //SectionType section;
    string sectionName;
    ScopeType scope;
    int size;

    stringstream Serialize();

    static Symbol Deserialize(string);

};

#endif //SS_SYMBOL_H
