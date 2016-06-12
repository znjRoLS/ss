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

string Relocation::Serialize()
{
    stringstream out;

    out << right <<
        setw(MAXNAME) << "-" <<
        setw(MAXNAME) << symbolName <<
        setw(MAXNAME) << section <<
        setw(MAXNAME) << offset <<
        setw(MAXNAME) << relocationType <<
        endl;

    return out.str();
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