//
// Created by rols on 6/4/16.
//

#include "Symbol.h"

Symbol::Symbol(string _name, bool _defined, SectionType _section, Scope _type, unsigned long _VA):
        name(_name),
        defined(_defined),
        section(_section),
        type(_type),
        VA(_VA)
{

}