//
// Created by rols on 6/7/16.
//

#include "Linker.h"
#include "ObjectFile.h"

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
            LoadFile(inputFile);
        }

        logFile << "Check output " << endl;
        FixRelocations();

        logFile << "Write output " << endl;
        WriteOutputFile(outputFile);
    }

    catch(exception e)
    {
        cerr << e.what() << endl;
        logFile << "ERROR: " << e.what() << endl;
    }

}

void Linker::LoadFile(ifstream &inputFile)
{

    ObjectFile objectFile;

    objectFile.LoadFromFile(inputFile);

    for (auto &section: objectFile.sections)
    {
        sections.insert({section.first, section.second});
    }

    for (auto &symbol: objectFile.symbols)
    {
        symbols.insert({symbol.first, symbol.second});
    }

    for (auto &relocation: objectFile.relocations)
    {
        relocations.push_back(relocation);
    }


    for (auto &symbol :symbols)
    {
        logFile << symbol.second;
    }


    for (int i = 0; i < relocations.size() ; i++)
    {
        logFile << (relocations[i]);
    }

    for (auto &section: sections)
    {
        logFile << section.second;
    }


}

void Linker::FixRelocations()
{

}

void Linker::WriteOutputFile(ofstream &outputFile)
{

}