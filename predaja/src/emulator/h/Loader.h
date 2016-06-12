//
// Created by rols on 6/12/16.
//

#ifndef SS_LOADER_H
#define SS_LOADER_H

#include <iostream>
#include <fstream>
#include <vector>

#include "Section.h"
#include "Relocation.h"

#include "LoaderConf.h"

using namespace std;

class Loader {

public:

    Loader();

    void Load(ifstream &inputFile);
    void LoadSection(istream &inputFile);
    void LoadDefaultIVT(ifstream &inputFile);

    //TODO: this is probably a bad idea, smart pointers?
    u_int8_t memory[MEMORY_SIZE];

    u_int32_t startPoint;

    Section programSection;

    vector<Relocation> relocations;

};


#endif //SS_LOADER_H
