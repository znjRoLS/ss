//
// Created by rols on 6/2/16.
//

#ifndef SS_COMPILER_H
#define SS_COMPILER_H

#include "SectionType.h"
#include "Compiler.h"
#include "Symbol.h"
#include "Instruction.h"

#include <iostream>
#include <fstream>

using namespace std;


class Compiler
{
public:

    Compiler();
    ~Compiler();

    void Compile(ifstream& inputFile, ofstream& outputFile);

private:

    static unordered_map<string, function<void()> > sectionProcessFunctions;

    void LoadAssemblyFromFile(ifstream& inputFile);
    void FirstRun();
    void SecondRun();
    void WriteObjectFile(ofstream& outputFile);

    unordered_map<string, Symbol> symbols;
    vector<Instruction*> instructions;

    vector<vector<string> > assemblyInput;


};

#endif //SS_COMPILER_H
