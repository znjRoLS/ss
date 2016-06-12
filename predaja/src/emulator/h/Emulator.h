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
#include "Loader.h"

using namespace std;

class Emulator {
public:
    Emulator();
    void Emulate(ifstream &inputFiles);

    static ofstream logFile;

private:


    void Execute();

//    /void LoadSection(ifstream&);


    Program program;


    Loader loader;


};


#endif //SS_EMULATOR_H
