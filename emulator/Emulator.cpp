//
// Created by rols on 6/7/16.
//

#include <exception>
#include <iostream>
#include <fstream>

#include "Emulator.h"

using namespace std;

ofstream Emulator::logFile("emulator/log");

void Emulator::Emulate(ifstream &inputFiles)
{
    try {

    }

    catch(exception e)
    {
        cerr << e.what() << endl;
        logFile << "ERROR: " << e.what() << endl;
    }

}