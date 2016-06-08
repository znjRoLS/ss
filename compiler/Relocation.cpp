//
// Created by rols on 6/8/16.
//

#include "Relocation.h"

Relocation::Relocation(string _section, u_int32_t _offset, RelocationType _relocationType):
    section(_section),
    offset(_offset),
    relocationType(_relocationType)
{

}