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

ofstream Emulator::logFile("emulator.log");

Emulator::Emulator() { }

void Emulator::Emulate(ifstream &inputFile)
{
    try {

        logFile << "Loading section" << endl;

        loader.Load(inputFile);
        //program.LoadSection(inputFiles);



        ifstream defaultIVT("object_files/defaultIVT.o");
        loader.LoadDefaultIVT(inputFile);

        logFile << "Load default ivt" << endl;
        //program.LoadDefaultIVT(defaultIVT);


        logFile << "Started executing" << endl;
        Execute();

    }

    catch(exception &e)
    {
        cerr << e.what() << endl;
        logFile << "ERROR: " << e.what() << endl;

        cout.flush();
    }

}



void Emulator::Execute()
{
    //TODO: dont use pointers, please
    program.Init(loader.memory, loader.startPoint);

    program.ReadNext();

    while(!program.IsEnd())
    {
        program.ExecuteCurrent();

        program.ReadNext();
    }
}