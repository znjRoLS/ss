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

                    StackPush(PSW.val);
                    LR = PC;
                    PC = currentInstruction.instrCode.instruction_int.src;

                }
            },
            {
                InstructionCodes::ADD, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_arithmetic_reg.src;
                    result = GetRegister(regDest);

                    u_int32_t firstOperand = result;
                    u_int32_t secondOperand;

                    if (currentInstruction.instrCode.instruction_arithmetic_reg.addr == 1)
                    {
                        secondOperand = SignExt(currentInstruction.instrCode.instruction_arithmetic_imm.imm, 18);
                    }
                    else
                    {
                        secondOperand = GetRegister(currentInstruction.instrCode.instruction_arithmetic_reg.src);
                    }

                    result += secondOperand;
                    SetRegister(result, regDest);

                    if (currentInstruction.setFlags)
                    {
                        PSW.Z = result == 0;

                        PSW.O = (int32_t)firstOperand > 0 && (int32_t)secondOperand > 0 && (int32_t)result < 0 ||
                            (int32_t)firstOperand < 0 && (int32_t)secondOperand < 0 && (int32_t)result > 0;

                        PSW.N = (int32_t)result < 0;

                        PSW.C = (int32_t)result >= 0 && ((int32_t)firstOperand < 0 || (int32_t)secondOperand < 0);
                    }

                }
            },
            {
                InstructionCodes::SUB, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_arithmetic_reg.src;
                    result = GetRegister(regDest);

                    u_int32_t firstOperand = result;
                    u_int32_t secondOperand;

                    if (currentInstruction.instrCode.instruction_arithmetic_reg.addr == 1)
                    {
                        secondOperand = SignExt(currentInstruction.instrCode.instruction_arithmetic_imm.imm, 18);
                    }
                    else
                    {
                        secondOperand = GetRegister(currentInstruction.instrCode.instruction_arithmetic_reg.src);
                    }

                    result -= secondOperand;
                    SetRegister(result, regDest);

                    if (currentInstruction.setFlags)
                    {
                        PSW.Z = result == 0;

                        PSW.O = (int32_t)firstOperand > 0 && (int32_t)secondOperand > 0 && (int32_t)result < 0 ||
                                (int32_t)firstOperand < 0 && (int32_t)secondOperand < 0 && (int32_t)result > 0;

                        PSW.N = (int32_t)result < 0;

                        PSW.C = (u_int32_t)result < 0 && (u_int32_t)firstOperand >= 0;
                    }

                }
            },
            {
                InstructionCodes::MUL, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_arithmetic_reg.src;
                    result = GetRegister(regDest);

                    u_int32_t firstOperand = result;
                    u_int32_t secondOperand;

                    if (currentInstruction.instrCode.instruction_arithmetic_reg.addr == 1)
                    {
                        secondOperand = SignExt(currentInstruction.instrCode.instruction_arithmetic_imm.imm, 18);
                    }
                    else
                    {
                        secondOperand = GetRegister(currentInstruction.instrCode.instruction_arithmetic_reg.src);
                    }

                    result *= secondOperand;
                    SetRegister(result, regDest);

                    if (currentInstruction.setFlags)
                    {
                        PSW.Z = result == 0;
                        PSW.N = (int32_t)result < 0;
                    }

                }
            },
            {
                InstructionCodes::DIV, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_arithmetic_reg.src;
                    result = GetRegister(regDest);

                    u_int32_t firstOperand = result;
                    u_int32_t secondOperand;

                    if (currentInstruction.instrCode.instruction_arithmetic_reg.addr == 1)
                    {
                        secondOperand = SignExt(currentInstruction.instrCode.instruction_arithmetic_imm.imm, 18);
                    }
                    else
                    {
                        secondOperand = GetRegister(currentInstruction.instrCode.instruction_arithmetic_reg.src);
                    }

                    result /= secondOperand;
                    SetRegister(result, regDest);

                    if (currentInstruction.setFlags)
                    {
                        PSW.Z = result == 0;
                        PSW.N = (int32_t)result < 0;
                    }

                }
            },
            {
                InstructionCodes::CMP, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_arithmetic_reg.dst;
                    result = GetRegister(regDest);

                    u_int32_t firstOperand = result;
                    u_int32_t secondOperand;

                    if (currentInstruction.instrCode.instruction_arithmetic_reg.addr == 1)
                    {
                        secondOperand = SignExt(currentInstruction.instrCode.instruction_arithmetic_imm.imm, 18);
                    }
                    else
                    {
                        secondOperand = GetRegister(currentInstruction.instrCode.instruction_arithmetic_reg.src);
                    }

                    result -= secondOperand;
                    //registers[regDest] = result;

                    if (currentInstruction.setFlags)
                    {
                        PSW.Z = result == 0;

                        PSW.O = (int32_t)firstOperand > 0 && (int32_t)secondOperand > 0 && (int32_t)result < 0 ||
                                (int32_t)firstOperand < 0 && (int32_t)secondOperand < 0 && (int32_t)result > 0;

                        PSW.N = (int32_t)result < 0;

                        PSW.C = (u_int32_t)result < 0 && (u_int32_t)firstOperand >= 0;
                    }

                }
            },
            {
                InstructionCodes::AND, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_logical.dst;
                    u_int32_t regSrc = currentInstruction.instrCode.instruction_logical.src;

                    result = GetRegister(regDest);
                    result &= GetRegister(regSrc);
                    SetRegister(result, regDest);

                    if (currentInstruction.setFlags)
                    {
                        PSW.Z = result == 0;
                        PSW.N = (int32_t)result < 0;
                    }
                }
            },
            {
                InstructionCodes::OR, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_logical.dst;
                    u_int32_t regSrc = currentInstruction.instrCode.instruction_logical.src;

                    result = GetRegister(regDest);
                    result |= GetRegister(regSrc);
                    SetRegister(result, regDest);

                    if (currentInstruction.setFlags)
                    {
                        PSW.Z = result == 0;
                        PSW.N = (int32_t)result < 0;
                    }
                }
            },
            {
                InstructionCodes::NOT, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_logical.dst;
                    u_int32_t regSrc = currentInstruction.instrCode.instruction_logical.src;

                    result = ~GetRegister(regSrc);
                    SetRegister(result, regDest);

                    if (currentInstruction.setFlags)
                    {
                        PSW.Z = result == 0;
                        PSW.N = (int32_t)result < 0;
                    }
                }
            },
            {
                InstructionCodes::TEST, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_logical.dst;
                    u_int32_t regSrc = currentInstruction.instrCode.instruction_logical.src;

                    result = GetRegister(regDest);
                    result &= GetRegister(regSrc);

                    if (currentInstruction.setFlags)
                    {
                        PSW.Z = result == 0;
                        PSW.N = (int32_t)result < 0;
                    }
                }
            },
            {
                InstructionCodes::LDRSTR, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_ldr_str.r;
                    u_int32_t regSrc = currentInstruction.instrCode.instruction_ldr_str.a;
                    u_int32_t imm = SignExt(currentInstruction.instrCode.instruction_ldr_str.imm,10);
                    u_int32_t f = currentInstruction.instrCode.instruction_ldr_str.f;

                    if (f == 4) SetRegister(GetRegister(regSrc) + 4, regSrc);
                    if (f == 5) SetRegister(GetRegister(regSrc) - 4, regSrc);

                    if (currentInstruction.instrCode.instruction_ldr_str.ls == 1)
                        SetRegister(GetMemory(GetRegister(regSrc) + imm),regDest);
                    else
                        SetMemory(GetRegister(regDest), GetRegister(regSrc) + imm);

                    if (f == 2) SetRegister(GetRegister(regSrc) + 4, regSrc);
                    if (f == 3) SetRegister(GetRegister(regSrc) - 4, regSrc);
                }
            },
            {
                InstructionCodes::CALL, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_call.dst;
                    u_int32_t imm = SignExt(currentInstruction.instrCode.instruction_call.imm,19);

                    LR = PC;

                    PC = GetRegister(regDest) + imm;
                }
            },
            {
                InstructionCodes::INOUT, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_in_out.dst;
                    u_int32_t regSrc = currentInstruction.instrCode.instruction_in_out.src;

                    if (currentInstruction.instrCode.instruction_in_out.io == 1)
                        SetRegister(GetRegister(regSrc), regDest);
                    else
                        SetRegister(GetRegister(regDest), regSrc);
                }
            },
            {
                InstructionCodes::MOVSHIFT, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_mov_shr_shl.dst;
                    u_int32_t regSrc = currentInstruction.instrCode.instruction_mov_shr_shl.src;
                    u_int32_t imm = currentInstruction.instrCode.instruction_mov_shr_shl.imm;

                    result = GetRegister(regSrc);

                    if (currentInstruction.instrCode.instruction_mov_shr_shl.lr == 1)
                        result <<= imm;
                    else
                        result >>= imm;

                    SetRegister(result, regDest);
                }
            },
            {
                InstructionCodes::LOADC, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_ldch_ldcl.dst;
                    u_int32_t imm = currentInstruction.instrCode.instruction_ldch_ldcl.c;

                    result = GetRegister(regDest);

                    if (currentInstruction.instrCode.instruction_ldch_ldcl.hl == 1)
                    {
                        result <<= 16;
                        result >>= 16;
                        imm <<= 16;
                        result |= imm;
                    }
                    else
                    {
                        result >>= 16;
                        result <<=16;
                        result |= imm;
                    }

                    SetRegister(result, regDest);
                }
            },
        };

}


u_int32_t Program::GetMemory(u_int32_t ind)
{
    if (ind >= MEMORY_SIZE)
    {
        throw runtime_error("Index for memory out of bounds !");
    }

    u_int32_t res;
    memcpy(&res, memory + ind, 4);

    return res;
}

void Program::SetMemory(u_int32_t val, u_int32_t ind)
{
    if (ind >= MEMORY_SIZE)
    {
        throw runtime_error("Index for memory out of bounds !");
    }

    memcpy(memory +ind, &val, 4);
}

u_int32_t Program::SignExt(u_int32_t val, size_t size)
{
    val <<= 32 - size;
    int32_t temp = val;
    temp >>= 32 - size;
    val = temp;
    return val;
}

void Program::StackPush(u_int32_t val)
{
    if (SP <= 0)
    {
        throw runtime_error("Stack overflow !");
    }

    SP-=4;
    memcpy(stack + SP, &val, 4);
}

u_int32_t Program::StackPop()
{
    if (SP >= STACK_SIZE)
    {
        throw runtime_error("Stack underflow !");
    }

    u_int32_t ret;
    memcpy(&ret, stack+SP, 4);
    return ret;
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
    PC = startPoint;
    SP = STACK_SIZE;

    PSW.val = 0;
}

void Program::ReadNext()
{
    u_int32_t currentCode;

    if (PC >= programSection.size + 4)
    {
        throw runtime_error("Out of scope ! Please use halt at the end of program.");
    }

    memcpy(&currentCode, programSection.memory + PC, 4);
    currentInstruction = Instruction::Deserialize(currentCode);

    PC += 4;
}

bool Program::IsEnd()
{
    return (currentInstruction.instrCode.instruction.instr == InstructionCodes::NOT);

}

void Program::ExecuteCurrent()
{
    instructionExecutors[currentInstruction.instructionSymbol]();
}

void Program::SetRegister(u_int32_t val, u_int32_t ind)
{
    switch(ind){
        case 16:
            PC = val;
            break;
        case 17:
            LR = val;
            break;
        case 18:
            SP = val;
            break;
        case 19:
            PSW.val = val;
            break;
        default:
            registers[ind] = val;
    }
}

u_int32_t Program::GetRegister(u_int32_t ind)
{
    switch(ind){
        case 16:
            return PC;
        case 17:
            return LR;
        case 18:
            return SP;
        case 19:
            return PSW.val;
        default:
            return registers[ind];
    }
}