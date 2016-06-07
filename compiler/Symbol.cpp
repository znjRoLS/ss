//
// Created by rols on 6/4/16.
//

#include "Symbol.h"

Symbol::Symbol(string _name, bool _defined, SectionType _section, string _sectionName, ScopeType _scope, unsigned long _offset):
        name(_name),
        defined(_defined),
        section(_section),
        sectionName(_sectionName),
        scope(_scope),
        offset(_offset)
{

}