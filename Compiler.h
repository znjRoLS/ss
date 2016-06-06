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
#include <queue>

using namespace std;


class Compiler
{
public:

    Compiler();
    ~Compiler();

    void Compile(ifstream& inputFile, ofstream& outputFile);

private:

    void LoadAssemblyFromFile(ifstream& inputFile);
    void FirstRun(ofstream&);
    void SecondRun(ofstream&);
    void WriteObjectFile(ofstream& outputFile);

    void HandleDirective(string directiveName, queue<string> &tokens, u_int32_t &locationCounter, string sectionName, bool writeToMemory);
    void AddNewSymbol(string symName, bool symDefined, SectionType symSection,string,  Symbol::ScopeType symScope, u_int32_t locationCounter);
    void HandleInstruction(string, queue<string>&, u_int32_t&);

    unordered_map<string, Symbol> symbols;
    vector<Instruction> instructions;

    vector<vector<string> > assemblyInput;

    unordered_map<string, u_int8_t*> sections;


};

#endif //SS_COMPILER_H
