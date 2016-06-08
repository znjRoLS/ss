//
// Created by rols on 6/8/16.
//

#ifndef SS_RELOCATION_H
#define SS_RELOCATION_H

#include <string>
#include <iostream>

#include "Enums.h"

using namespace std;

class Relocation {

public:

    Relocation(string, u_int32_t, RelocationType);

    string section;
    u_int32_t offset;
    RelocationType relocationType;

    friend ostream& operator<<(ostream& out, Relocation&);
};


#endif //SS_RELOCATION_H
