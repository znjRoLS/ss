//
// Created by rols on 6/7/16.
//

#ifndef SS_LINKER_H
#define SS_LINKER_H

#include <vector>
#include <iostream>
#include <fstream>

#include "Enums.h"
#include "Section.h"
#include "Symbol.h"
#include "Relocation.h"
#include "LoaderScriptFile.h"


using namespace std;

class Linker {

public:
    void Link(ifstream&, vector<ifstream> &inputFiles, ofstream &outputFile);

private:
    void LoadFile(ifstream &inputFile);

    void FixRelocations();

    void WriteOutputFile(ofstream &outputFile);

    static ofstream logFile;

    unordered_map<string,Section> sections;
    unordered_map<string,Symbol> symbols;
    vector<Relocation> relocations;
    unordered_map<string,int> sectionPositions;

    LoaderScriptFile loaderScript;

    void AddSymbol(Symbol&);
    int AddSection(Symbol&, Section&);

    void GenerateOutput();


};


#endif //SS_LINKER_H
