//
// Created by rols on 6/8/16.
//

#ifndef SS_OBJECTFILE_H
#define SS_OBJECTFILE_H

#include "Enums.h"
#include "Section.h"
#include "Symbol.h"
#include "Relocation.h"

#include <vector>
#include <unordered_map>

class ObjectFile {
public:

    void LoadFromFile(string );

    unordered_map<string,Section> sections;
    unordered_map<string,Symbol> symbols;
    vector<Relocation> relocations;


};


#endif //SS_OBJECTFILE_H
