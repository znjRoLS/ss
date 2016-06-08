//
// Created by rols on 6/8/16.
//

#include "Relocation.h"

#include <iostream>
#include <iomanip>

using namespace std;

istream& operator>>(istream& in, RelocationType &s);
ostream& operator<<(ostream& out, RelocationType);

Relocation::Relocation(string _symbolName, string _section, u_int32_t _offset, RelocationType _relocationType):
    section(_section),
    offset(_offset),
    relocationType(_relocationType),
    symbolName(_symbolName)
{

}

stringstream Relocation::Serialize()
{
    stringstream out;

    out << right <<
        setw(15) << "-" <<
        setw(15) << symbolName <<
        setw(15) << section <<
        setw(15) << offset <<
        setw(15) << relocationType <<
        endl;

    return out;
}

Relocation Relocation::Deserialize(string instr)
{
    stringstream in;
    in << instr;

    string token;
    in >> token;
    string symbolName;
    in >> symbolName;
    string section;
    in >> section;
    u_int32_t offset;
    in >> offset;
    RelocationType relType;
    in >> relType;

    Relocation rel(symbolName, section, offset, relType);

    return rel;
}