//
// Created by rols on 6/4/16.
//

#include "Instruction.h"

#include <regex>


Instruction::Instruction(InstructionSymbol _name, InstructionCondition _condition, bool _setFlags):
        instructionSymbol(_name),
        instructionCondition(_condition),
        setFlags(_setFlags)
{

};



Instruction Instruction::Deserialize(u_int32_t instructionCode)
{
    Instruction instruction(NO_INSTRUCTION, NO_CONDITION, false);

    instruction.instrCode.binaryCode = instructionCode;

    instruction.instructionSymbol = (InstructionSymbol) instruction.instrCode.instruction.instr;

    instruction.instructionCondition = (InstructionCondition) instruction.instrCode.instruction.cond;

    instruction.setFlags = instruction.instrCode.instruction.flag != 0;

    return instruction;
}