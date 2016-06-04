//
// Created by rols on 6/2/16.
//

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <memory>

#include "Compiler.h"
#include "Symbol.h"
#include "Instruction.h"




ostream& operator<<(ostream& out, Instruction& instr)
{
    out << instr.name;
    return out;
}

void split(const string&, const char*, vector<string>&);

Compiler::Compiler()
{

}


Compiler::~Compiler()
{

}


void Compiler::Compile(ifstream& inputFile)
{
    cout << "Compiling " << endl;

    vector<Symbol> symbols;

    string currentSection;

    SectionType currentSectionType = GLOBAL;

    Instruction *currentInstruction;

    unordered_map<string, Instruction*> &instructions = Instruction::instructionTable;

    string line;

    while( getline(inputFile, line) ) {

        vector<string> tokens;
        split(line, " ,\t\n", tokens);


        if (tokens.size() == 0)
            continue;

        if (tokens[0][0] == '.') // section
        {
            if (tokens[0][1] == 'p' || tokens[0][1] == 'e')
            {
                // .public .extern
                if (currentSectionType != GLOBAL)
                {
                    cout << "Please write public and extern declarations at the begining of the file" << endl;
                    throw "random err";
                }

                symbols.push_back(Symbol(tokens[1], (tokens[0][1] == 'e'), GLOBAL, Symbol::GLOBAL));

            }

            else
            {
                //.text .data .bss
                currentSection = tokens[0];

                //TODO@rols: nope.
                if (tokens[0][1] == 't')
                    currentSectionType = TEXT;
                if (tokens[0][1] == 'd')
                    currentSectionType = DATA;
                if (tokens[0][1] == 'b')
                    currentSectionType = BSS;
            }

        }


        if (tokens[0][tokens[0].size() - 1] == ':')
        {
            //label:

            symbols.push_back(Symbol(tokens[0], true, currentSectionType, Symbol::LOCAL));

        }

//        int instrNum = 0;
//        for (int wordNum = 0; wordNum < tokens.size(); wordNum++)
//        {
//            if (wordNum == 0 && tokens[0][0] == '.')
//            {
//                currentSection = tokens[0];
//
//                if (currentSection == ".public")
//                {
//                    symbols.push_back({tokens[1], true});
//                    wordNum ++;
//                }
//
//                continue;
//            }
//
//            if (wordNum == 0 && tokens[0][tokens[0].size()-1] == ':')
//            {
//                //label = tokens[0];
//
//                symbols.push_back({tokens[0], true});
//
//                continue;
//            }
//
//            if (instrNum == 0)
//            {
//                if (instructions.find(tokens[wordNum]) == instructions.end())
//                {
//                    cout << "Not a valid instruction! " << line << endl;
//                    throw "err";
//                }
//
//                currentInstruction = new Instruction(*instructions[tokens[wordNum]]);
//
//            }
//
//            if (instrNum >=1)
//            {
//                currentInstruction->SetParam(instrNum, tokens[wordNum]);
//            }
//
//            instrNum ++;
//
//        }


    }




}


void Compiler::WriteObjectFile(ofstream& outputFile)
{
    cout << "Writing object" << endl;
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