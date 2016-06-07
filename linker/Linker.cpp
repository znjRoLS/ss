//
// Created by rols on 6/7/16.
//

#include "Linker.h"

#include <exception>
#include <iostream>
#include <fstream>

using namespace std;

ofstream Linker::logFile("linker/log");

void Linker::Link(vector<ifstream> &inputFiles, ofstream &outputFile)
{
    try {
        for (auto &inputFile: inputFiles)
        {
            logFile << "Linking file " << endl;
            LinkFile(inputFile);
        }

        logFile << "Check output " << endl;
        CheckOutputFile();

        logFile << "Write output " << endl;
        WriteOutputFile(outputFile);
    }

    catch(exception e)
    {
        cerr << e.what() << endl;
        logFile << "ERROR: " << e.what() << endl;
    }

}

void Linker::LinkFile(ifstream &inputFile)
{

}

void Linker::CheckOutputFile()
{

}

void Linker::WriteOutputFile(ofstream &outputFile)
{

}