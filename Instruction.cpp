//
// Created by rols on 6/4/16.
//

#include "Instruction.h"


Instruction::Instruction(string _name):
        name(_name)
{
        numParameters = 0;
};

unordered_map<string,Instruction*> Instruction::instructionTable =
        {
                {"add", new Instruction("add")},
                {"cmp", new Instruction("cmp")},
                {"je", new Instruction("je")},
                {"out", new Instruction("out")},
                {"ldr", new Instruction("ldr")},
                {"lds", new Instruction("lds")},
        };


void Instruction::SetNextParameter(string param)
{
        parameters[numParameters++] = param;
}