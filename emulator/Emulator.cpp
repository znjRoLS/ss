//
// Created by rols on 6/7/16.
//

#include <exception>
#include <iostream>
#include <fstream>
#include <cstring>
#include <functional>

#include "Emulator.h"
#include "Instruction.h"

using namespace std;

ofstream Emulator::logFile("emulator/log");

void Emulator::Emulate(ifstream &inputFiles)
{
    try {

        logFile << "Loading section" << endl;
        LoadSection(inputFiles);

        logFile << "Started executing" << endl;
        Execute();

    }

    catch(exception e)
    {
        cerr << e.what() << endl;
        logFile << "ERROR: " << e.what() << endl;
    }

}

void Emulator::LoadSection(ifstream &inputFiles)
{
    string line;
    string sectionCumulate;
    bool outputSection = false;
    bool mainOutputPassed = false;

    while (getline(inputFiles, line))
    {
        if (!outputSection)
        {
            if (line == "%OUTPUT SECTION%")
            {
                outputSection = true;
            }
            continue;
        }
        if (line == "%END%")
        {
            outputSection = false;
            continue;
        }

        if (!mainOutputPassed)
        {
            mainOutputPassed = true;
            stringstream ss;
            ss << line;
            string token;
            ss >> token;
            ss >> startPoint;
            continue;
        }

        if (line == ".end")
        {
            programBinary = Section::Deserialize(sectionCumulate);

            sectionCumulate.clear();
        }

        else
        {
            sectionCumulate += line + "\n";
        }
    }
}

void Emulator::Execute()
{

    unordered_map<int, function<void()> > instructionExecutors =
        {
            {
                Instruction::INT, [&](){
                
                }
            }
        };



    programCounter = startPoint;

    //TODO: Y U DO DIS
    Instruction currentInstruction(Instruction::NO_INSTRUCTION, Instruction::NO_CONDITION, false);

    u_int32_t currentCode;

    while(true)
    {
        if (programCounter >= programBinary.size + 4)
        {
            throw runtime_error("Out of scope ! Please use halt at the end of program.");
        }

        memcpy(&currentCode, programBinary.memory + programCounter, 4);
        currentInstruction = Instruction::Deserialize(currentCode);

        instructionExecutors[currentInstruction.instructionSymbol]();
    }
}