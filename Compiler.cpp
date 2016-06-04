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


class Instruction
{
public:
    Instruction(string _name):name(_name){};
    friend ostream& operator<<(ostream&, Instruction&);
private:
    string name;
};

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

    string currentSection;
    Instruction *currentInstruction;

    unordered_map<string, shared_ptr<Instruction>> instructions =
            {
                    {"add", new Instruction("add")}
            };

    string line;

    while( getline(inputFile, line) ) {

        vector<string> tokens;
        split(line, " ,", tokens);


        if (tokens.size() == 0)
            continue;

        if (tokens[0][0] == '.') // section
        {
            currentSection = tokens[0];

        }

        int instrNum = 0;
        for (int wordNum = 0; wordNum < tokens.size(); wordNum++)
        {
            if (wordNum == 0 && tokens[0][0] == '.')
            {
                currentSection = tokens[0];
                continue;
            }

            if (wordNum == 0 && tokens[0][tokens[0].size()-1] == ':')
            {
                //label = tokens[0];
                continue;
            }

            if (instrNum == 0)
            {
                //TODO@rols: very ugly
                currentInstruction = new Instruction(*instructions[tokens[wordNum]]);

            }

        }


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