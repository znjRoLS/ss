//
// Created by rols on 6/7/16.
//

#ifndef SS_EMULATOR_H
#define SS_EMULATOR_H

#include <vector>
#include <iostream>
#include <fstream>

#include "Section.h"
#include "Program.h"

using namespace std;

class Emulator {
public:
    Emulator();
    void Emulate(ifstream &inputFiles);

private:


    void Execute();

//    /void LoadSection(ifstream&);

    static ofstream logFile;

    Program program;


};


#endif //SS_EMULATOR_H
