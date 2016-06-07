//
// Created by rols on 6/2/16.
//

#ifndef SS_COMPILER_H
#define SS_COMPILER_H

#include "Compiler.h"
#include "Symbol.h"
#include "Instruction.h"
#include "Enums.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <regex>

using namespace std;

class Symbol;
class Compiler
{
public:


    Compiler();
    ~Compiler();

    void Compile(ifstream& inputFile, ofstream& outputFile);

private:

    void LoadAssemblyFromFile(ifstream& inputFile);
    void FirstRun();
    void SecondRun();
    void WriteObjectFile(ofstream& outputFile);

    void HandleDirective(string directiveName, queue<string> &tokens, u_int32_t &locationCounter, string sectionName, bool writeToMemory);
    void AddNewSymbol(string symName, bool symDefined, SectionType symSection,string,  ScopeType symScope, u_int32_t locationCounter);
    void HandleInstruction(string, queue<string>&, u_int32_t&);
    TokenType ParseToken(string token);
    void UpdateCurrentSection(string sectionName, SectionType &currentSection, u_int32_t &offsetCounter);
    u_int32_t ParseOperand(string token, int immSize = 0);
    void GetOperand(queue<string> &tokens, string &token, u_int32_t &operand, TokenType &operandType, vector<TokenType> operandAllowed, int operandImmSize = 0);

    unordered_map<string, Symbol> symbols;
    vector<Instruction> instructions;

    vector<vector<string> > assemblyInput;

    unordered_map<string, u_int8_t*> sections;

    static ofstream logFile;

    static unordered_map<int, regex> tokenParsers;
    unordered_map<int, function<void(Instruction&, queue<string>&)> > instructionsHandlers;
    static unordered_map<string, u_int8_t> instructionCodes;
    static unordered_map<string, u_int8_t> branchCodes;
    static unordered_map<string, int> instructionTypesMap;
    static unordered_map<string, u_int32_t> specialRegisterValues;
};

#endif //SS_COMPILER_H
