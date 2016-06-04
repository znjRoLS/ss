//
// Created by rols on 6/4/16.
//

#include "Operand.h"

Operand::Operand(string input, OperandType opType):
    operandType(opType)
{
    if (opType == REG)
    {
        operandType = REG;
        regNum = atoi(input.substr(1).c_str());
    }

    else
    {
        operandType = IMM;
        immVal = atoi(input,c_str());
        immVal <<
    }
}