//
// Created by rols on 6/4/16.
//

#include "Instruction.h"

unordered_map<string,Instruction*> Instruction::instructionTable =
        {
                {"add", new Instruction("add")},
                {"cmp", new Instruction("cmp")},
                {"je", new Instruction("je")},
                {"out", new Instruction("out")},
                {"iret", new Instruction("iret")},
                {"lds", new Instruction("lds")},
        };


void Instruction::SetParam(int ind, string param)
{
        parameters[ind] = param;
}