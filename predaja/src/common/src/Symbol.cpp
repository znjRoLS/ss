//
// Created by rols on 6/4/16.
//

#include <iostream>
#include <iomanip>

#include "Symbol.h"

using namespace std;

istream& operator>>(istream& , ScopeType&);
ostream& operator<<(ostream& , ScopeType);
istream& operator>>(istream& , TokenType &);
ostream& operator<<(ostream& , TokenType);

Symbol::Symbol(string _name, bool _defined, string _sectionName, ScopeType _scope, unsigned long _offset, TokenType _symbolType, int _size):
        name(_name),
        defined(_defined),
        sectionName(_sectionName),
        scope(_scope),
        offset(_offset),
        size(_size),
        symbolType(_symbolType)
{

}


string Symbol::Serialize()
{
    stringstream ss;
    ss << right <<
        setw(MAXNAME) << "-" <<
        setw(MAXNAME) << name <<
        setw(MAXNAME) << defined <<
        setw(MAXNAME) << sectionName <<
        setw(MAXNAME) << offset <<
        setw(MAXNAME) << scope <<
        setw(MAXNAME) << size <<
        setw(MAXNAME) << symbolType <<
        endl;

    return ss.str();
}

Symbol Symbol::Deserialize(string instr)
{
    stringstream in;
    in << instr;

    string token;
    in >> token;
    string name;
    in >> name;
    bool defined;
    in >> defined;
    string sectionName;
    in >> sectionName;
    int offset;
    in >> offset;
    ScopeType scope;
    in >> scope;
    int size;
    in >> size;
    TokenType symbolType;
    in >> symbolType;

    Symbol sym(name,defined, sectionName, scope, offset, symbolType, size);

    return sym;
}