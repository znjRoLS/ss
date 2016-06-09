//
// Created by rols on 6/9/16.
//

#include "Program.h"

#include <cstring>
#include <Instruction.h>

Program::Program():
    programSection("output", 0),
    currentInstruction(InstructionCodes::NO_INSTRUCTION, InstructionCodes::NO_CONDITION, false)
{
    instructionExecutors =
        {
            {
                InstructionCodes::INT, [&](){

            }
            }
        };

}


void Program::LoadSection(istream &inputFile)
{
    string line;
    string sectionCumulate;
    bool outputSection = false;
    bool mainOutputPassed = false;

    while (getline(inputFile, line))
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
            programSection = Section::Deserialize(sectionCumulate);

            sectionCumulate.clear();
        }

        else
        {
            sectionCumulate += line + "\n";
        }
    }
}

void Program::Init()
{
    programCounter = startPoint;
}

void Program::ReadNext()
{
    u_int32_t currentCode;

    if (programCounter >= programSection.size + 4)
    {
        throw runtime_error("Out of scope ! Please use halt at the end of program.");
    }

    memcpy(&currentCode, programSection.memory + programCounter, 4);
    currentInstruction = Instruction::Deserialize(currentCode);
}

bool Program::IsEnd()
{
    return (currentInstruction.instrCode.instruction.instr == InstructionCodes::NOT);

}

void Program::ExecuteCurrent()
{
    instructionExecutors[currentInstruction.instructionSymbol]();
}