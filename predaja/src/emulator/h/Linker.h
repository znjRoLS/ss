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
    void Link(ifstream&, vector<string> &inputFiles, ofstream &outputFile);

private:
    void LoadFile(string inputFile);

    void FixRelocations();

    void FillRemainingSections();

    void WriteOutputFile(ofstream &outputFile);

    static ofstream logFile;

    unordered_map<string,Section> sections;
    unordered_map<string,Symbol> symbols;
    vector<Relocation> relocations;
    unordered_map<string,int> sectionPositions;

    LoaderScriptFile loaderScript;

    u_int32_t locationCounter;

    void AddSymbol(Symbol&);
    int AddSection(Symbol&, Section&);

    void GenerateOutput();

    //u_int8_t *memory = nullptr;
    Section *outputSection = nullptr;

    u_int32_t GetSymbolVal(string symbolName);

    vector<string> symbolsNotDefined;


};


#endif //SS_LINKER_H
