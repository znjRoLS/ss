//
// Created by rols on 6/2/16.
//

#include <cstring>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <memory>

#include "Compiler.h"


//unordered_map<string, function<void()> > Compiler::sectionProcessFunctions =
//    {
//        {"public", [](Compiler& compiler, string sectionName, SectionType sectionType, u_int32_t locationCounter) {
//
//        }}
//    };


ostream& operator<<(ostream& out, Instruction& instr)
{
    out << "Instruction: " << instr.name << endl;

    for (int i = 0 ; i < instr.numParameters ; i++)
        out << "\t" << instr.parameters[i]  << endl;

    out << "\tcode:" << hex << instr.instrCode << endl;

    out << endl;

    return out;
}


ostream& operator<<(ostream& out, Symbol& symbol)
{
    out << "Symbol: " << symbol.name << endl;
    out << "\tDefined:\t" << symbol.defined << endl;
    out << "\tSection:\t" << symbol.section << endl;
    out << "\tVA:\t" << symbol.VA << endl;
    out << "\tType:\t" << symbol.scope << endl;

    return out;
}



void split(const string &s, const char* delim, vector<string> & v){
    // to avoid modifying original string
    // first duplicate the original string and return a char pointer then free the memory
    char * dup = strdup(s.c_str());
    char * token = strtok(dup, delim);
    while(token != NULL){
        v.push_back(string(token));
        // the call is treated as a subsequent calls to strtok:
        // the function continues from where it left in previous invocation
        token = strtok(NULL, delim);
    }
    free(dup);
}

Compiler::Compiler()
{

}


Compiler::~Compiler()
{

}


void Compiler::Compile(ifstream& inputFile, ofstream& outputFile)
{
    cout << "Compiling " << endl;

    LoadAssemblyFromFile(inputFile);
    FirstRun();
    SecondRun();
    WriteObjectFile(outputFile);

//
//    u_int32_t currentVA = 0;
//
//    string currentSection;
//    SectionType currentSectionType = GLOBAL;
//
//    string line;
//    while( getline(inputFile, line) ) {
//
//        vector<string> tokens;
//        split(line, " ,\t\n", tokens);
//
//        int wordNum = 0;
//
//
//        if (tokens.size() == 0)
//            continue;
//
//        if (tokens[wordNum][0] == '.') // section
//        {
//
//
//            if (tokens[wordNum][1] == 'p' || tokens[wordNum][1] == 'e')
//            {
//                // .public .extern
//                if (currentSectionType != GLOBAL)
//                {
//                    cout << "Please write public and extern declarations at the begining of the file" << endl;
//                    throw "random err";
//                }
//
//                for (wordNum ++; wordNum < tokens.size(); wordNum ++)
//                {
//
//                    if (symbols.find(tokens[wordNum]) != symbols.end())
//                    {
//                        cout << "Already defined symbol " << tokens[wordNum] << endl;
//                        throw "rando";
//                    }
//
//                    Symbol *sym = new Symbol(tokens[wordNum]);
//
//                    sym->defined = tokens[0][01] == 'e';
//                    sym->scope = Symbol::ScopeType::GLOBAL;
//
//                    symbols[tokens[wordNum]] = sym;
//
//
//                    //symbols.push_back(Symbol(tokens[wordNum], (tokens[0][1] == 'e'), GLOBAL, Symbol::GLOBAL, 0));
//                }
//
//            }
//
//            else
//            {
//                //.text .data .bss
//                currentSection = tokens[0];
//
//                //TODO@rols: nope.
//                if (tokens[wordNum][1] == 't')
//                    currentSectionType = TEXT;
//                if (tokens[wordNum][1] == 'd')
//                    currentSectionType = DATA;
//                if (tokens[wordNum][1] == 'b')
//                    currentSectionType = BSS;
//
//                Symbol *sym = new Symbol(tokens[wordNum]);
//
//                sym->defined = true;
//                sym->scope = Symbol::ScopeType::LOCAL;
//                sym->VA = currentVA;
//
//                symbols[tokens[wordNum]] = sym;
//            }
//
//            wordNum ++;
//        }
//
//        if (wordNum < tokens.size())
//        {
//            if (currentSectionType == GLOBAL)
//            {
//                cout << "Error, global section" << endl;
//                throw "rand err";
//            }
//
//            if (tokens[wordNum][tokens[0].size() - 1] == ':')
//            {
//                //label:
//
//                string labelName = tokens[wordNum].substr(0,tokens[wordNum].size() - 1);
//
//                Symbol *sym;
//
//                if (symbols.find(labelName) != symbols.end())
//                {
//                    sym = symbols[labelName];
//                }
//                else
//                {
//                    sym = new Symbol(labelName);
//                }
//
//                sym->VA = currentVA;
//                sym->section = currentSectionType;
//                sym->defined = true;
//
//                symbols[labelName] = sym;
//
//                //symbols.push_back(Symbol(tokens[wordNum], true, currentSectionType, Symbol::LOCAL, currentVA));
//
//                wordNum ++;
//            }
//
//            if (wordNum < tokens.size())
//            {
//                if (currentSectionType == DATA)
//                {
//                    if (tokens[wordNum][0] != '.')
//                    {
//                        cout << "Error in parsing" << endl;
//                        throw "random";
//                    }
//
//                    if (tokens[wordNum] == ".char")
//                    {
//                        currentVA += 1;
//                    }
//
//                    if (tokens[wordNum] == ".word")
//                    {
//                        currentVA += 4;
//                    }
//
//
//                    if (tokens[wordNum] == ".long")
//                    {
//                        currentVA += 4;
//                    }
//
//                    if (tokens[wordNum] == ".align")
//                    {
//                        if (currentVA/4*4 != currentVA)
//                        {
//                            currentVA = (currentVA/4+1)*4;
//                        }
//                    }
//
//                    if (tokens[wordNum] == ".skip")
//                    {
//                        currentVA += atoi(tokens[wordNum+1].c_str());
//                    }
//
//
//
//                }
//
//                if (currentSectionType == TEXT)
//                {
//                    vector<string> instr;
//                    string instrName;
//
//                    instrName = tokens[wordNum++];
//
//                    for (;wordNum < tokens.size(); wordNum ++)
//                        instr.push_back(tokens[wordNum]);
//
//                    //Instruction *currentInstruction = Instruction::ParseInstruction(instrName, instr, instructions, currentVA);
//                    Instruction::ParseInstruction(instrName, instr, instructions, currentVA);
//
////                    for(; wordNum < tokens.size(); wordNum ++)
////                    {
////                        if (currentInstruction == nullptr)
////                        {
////                            if (instructionsTable.find(tokens[wordNum]) == instructionsTable.end())
////                            {
////                                cout << "Instruction not supported" << endl;
////                                throw "rand";
////                            }
////
////                            currentInstruction = new Instruction(*instructionsTable[tokens[wordNum]]);
////                            continue;
////                        }
////
////                        currentInstruction->SetNextParameter(tokens[wordNum]);
////                    }
//
//                    //instructions.push_back(currentInstruction);
//
////                    /currentVA += 4;
//                }
//            }
//
//
//        }
//
//    }

}


void Compiler::LoadAssemblyFromFile(ifstream &inputFile)
{
    string line;

    while( getline(inputFile, line) ) {

        //strip comments
        size_t found = line.find(';');
        if (found != string::npos) {
            line = line.substr(0, found);
        }

        vector<string> tokens;
        split(line, " ,\t\n", tokens);

        if (tokens.size() == 0)
            continue;

        if (tokens[0] == ".end")
            break;

        assemblyInput.push_back(tokens);
    }
}


void Compiler::FirstRun()
{
    u_int32_t locationCounter = 0;

    string currentSection;
    SectionType currentSectionType = GLOBAL;

    for (auto &tokens: assemblyInput)
    {


        if (tokens[0][0] == '.') // section
        {
            string sectionName = tokens[0].substr(1, tokens[0].size() - 1);

            string sectionType = sectionName;

            size_t found = sectionType.find('.');
            if (found != string::npos)
            {
                sectionType = sectionType.substr(0,found);
            }

            if (sectionType == "text" || sectionType == "data" || sectionType == "bss")
            {
                //.text .data .bss
                //currentSection = tokens[0];

                if (sectionType == "text")
                    currentSectionType = TEXT;
                if (sectionType == "data")
                    currentSectionType = DATA;
                if (sectionType == "bss")
                    currentSectionType = BSS;

                Symbol sym;

                sym.name = sectionName;
                sym.defined = true;
                sym.scope = Symbol::ScopeType::LOCAL;
                sym.VA = locationCounter;

                symbols[sectionType] = sym;
            }

            continue;
        }

        int wordNum = 0;

        if (tokens[0][tokens[0].size()-1] == ':')
        {
            //label

            string labelName = tokens[0].substr(0,tokens[0].size() - 1);

            Symbol sym;

            if (symbols.find(labelName) != symbols.end())
            {
                sym = symbols[labelName];
            }

            sym.name = labelName;
            sym.VA = locationCounter;
            sym.section = currentSectionType;
            sym.defined = true;

            symbols[labelName] = sym;

            wordNum ++;
        }

        for (; wordNum < tokens.size(); wordNum ++)
        {
            if ()
        }
    }
}


void Compiler::SecondRun()
{
//    if (sectionType == "public" || sectionType == "extern")
//    {
//        if (currentSectionType != GLOBAL)
//        {
//            cout << "Please write public and extern declarations at the begining of the file" << endl;
//            throw "random err";
//        }
//
//        for (int wordNum = 1; wordNum < tokens.size(); wordNum ++)
//        {
//            if (symbols.find(tokens[wordNum]) != symbols.end())
//            {
//                cout << "Already defined symbol " << tokens[wordNum] << endl;
//                throw "rando";
//            }
//
//            Symbol sym;
//
//            sym.name = sectionName;
//            sym.defined = tokens[0][1] == 'e';
//            sym.scope = Symbol::ScopeType::GLOBAL;
//
//            symbols[tokens[wordNum]] = sym;
//        }
//    }
}


void Compiler::WriteObjectFile(ofstream& outputFile)
{
    cout << "Writing object" << endl;

    //typedef std::map<std::string, std::map<std::string, std::string>>::iterator it_type;
    for(auto iterator = symbols.begin(); iterator != symbols.end(); iterator++)
    {
        cout << (iterator->second);
    }

    for (int i = 0; i < instructions.size() ; i++)
    {
        cout << *(instructions[i]);
    }

}
