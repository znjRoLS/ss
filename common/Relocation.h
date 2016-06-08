//
// Created by rols on 6/8/16.
//

#ifndef SS_RELOCATION_H
#define SS_RELOCATION_H

#include <string>
#include <iostream>
#include <sstream>

#include "Enums.h"

using namespace std;

class Relocation {

public:

    Relocation(string, string, u_int32_t, RelocationType);

    string section;
    string symbolName;
    u_int32_t offset;
    RelocationType relocationType;

    friend ostream& operator<<(ostream& out, Relocation&);

    stringstream Serialize();

    static Relocation Deserialize(string);

};


#endif //SS_RELOCATION_H
