//
// Created by rols on 6/7/16.
//

#ifndef SS_EMULATOR_H
#define SS_EMULATOR_H

#include <vector>
#include <iostream>
#include <fstream>

#include "Section.h"

using namespace std;

class Emulator {
public:
    void Emulate(ifstream &inputFiles);

private:

    void Execute();

    void LoadSection(ifstream&);

    static ofstream logFile;

    Section programBinary;

    u_int32_t startPoint;

    u_int32_t programCounter;


};


#endif //SS_EMULATOR_H
