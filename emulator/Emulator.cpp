//
// Created by rols on 6/7/16.
//

#include <exception>
#include <iostream>
#include <fstream>
#include <cstring>
#include <functional>

#include "Emulator.h"
#include "Instruction.h"

using namespace std;

ofstream Emulator::logFile("emulator/log");

Emulator::Emulator() { }

void Emulator::Emulate(ifstream &inputFiles)
{
    try {

        logFile << "Loading section" << endl;
        program.LoadSection(inputFiles);


        ifstream defaultIVT("object_files/defaultIVT.o");

        logFile << "Load default ivt" << endl;
        program.LoadDefaultIVT(defaultIVT);


        logFile << "Started executing" << endl;
        Execute();

    }

    catch(exception &e)
    {
        cerr << e.what() << endl;
        logFile << "ERROR: " << e.what() << endl;
    }

}



void Emulator::Execute()
{
    program.Init();

    program.ReadNext();

    while(!program.IsEnd())
    {
        program.ExecuteCurrent();

        program.ReadNext();
    }
}