//
// Created by rols on 6/9/16.
//

#include "Program.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Instruction.h"
#include "Emulator.h"

using namespace std;

ostream& operator<<(ostream& out, InstructionCodes::InstructionSymbol s);

ostream& operator<<(ostream& out, InstructionCodes::InstructionCondition s);

Program::Program():
    programSection("output", 0),
    currentInstruction(InstructionCodes::HALT, InstructionCodes::NO_CONDITION, false)
{
    instructionExecutors =
        {
            {
                InstructionCodes::INT, [&](){

                    StackPush(PSW.val);
                    LR = PC;
                    PC = currentInstruction.instrCode.instruction_int.src + CODE_START;

                }
            },
            {
                InstructionCodes::ADD, [&](){

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

                    PC = GetRegister(regDest) + imm + CODE_START;
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
    if (SP <= STACK_START)
    {
        throw runtime_error("Stack overflow !");
    }

    SP-=4;
    memcpy(memory + SP, &val, 4);
}

u_int32_t Program::StackPop()
{
    if (SP >= STACK_START + STACK_SIZE)
    {
        throw runtime_error("Stack underflow !");
    }

    u_int32_t ret;
    memcpy(&ret, memory+SP, 4);
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

            Emulator::logFile << "Copied code to " << CODE_START <<" " << (int)memory[64] <<  endl;
            memcpy(memory + CODE_START, programSection.memory, programSection.size);

            startPoint += CODE_START;
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
    SP = STACK_SIZE + STACK_START;

    PSW.val = 0;
    PSW.T = 1;

    executionStart = chrono::system_clock::now();
    lastTimerExecution = executionStart;
}

void Program::ReadNext()
{
    u_int32_t currentCode;

//    if (PC >= programSection.size + 4)
//    {
//        throw runtime_error("Out of scope ! Please use halt at the end of program.");
//    }

    memcpy(&currentCode, memory + PC, 4);
    currentInstruction = Instruction::Deserialize(currentCode);

    PC += 4;
}

bool Program::IsEnd()
{
    return (currentInstruction.instrCode.instruction.instr == InstructionCodes::HALT);

}

void Program::ExecuteCurrent()
{

    Emulator::logFile << "Registers: ";
    for (int i = 0 ; i < 16;  i ++)
        Emulator::logFile << " " << hex << registers[i];

    Emulator::logFile << endl;
    Emulator::logFile << "PC: " << hex << PC << endl;
    Emulator::logFile << "LR: " << hex << LR << endl;
    Emulator::logFile << "SP: " << hex << SP << endl;
    Emulator::logFile << "PSW: " << hex << PSW.val << endl;

    Emulator::logFile << "Executing: " << currentInstruction.instructionSymbol << " " << currentInstruction.instructionCondition << " " << currentInstruction.setFlags << " " << currentInstruction.instrCode.binaryCode << endl;

    instructionExecutors[currentInstruction.instructionSymbol]();

    Emulator::logFile << "Registers: ";
    for (int i = 0 ; i < 16;  i ++)
        Emulator::logFile << " " << hex << registers[i];

    Emulator::logFile << endl;
    Emulator::logFile << "PC: " << hex << PC << endl;
    Emulator::logFile << "LR: " << hex << LR << endl;
    Emulator::logFile << "SP: " << hex << SP << endl;
    Emulator::logFile << "PSW: " << hex << PSW.val << endl;

    HandleInterrupts();

}

void Program::HandleInterrupts()
{

    chrono::time_point<chrono::system_clock> timeNow = chrono::system_clock::now();
    chrono::duration<double> elapsed = timeNow - lastTimerExecution;
    //TODO: hardcoded
    Emulator::logFile << dec << "elapsed: " << elapsed.count() << endl;
    if (elapsed.count() >= 1.0)
    {
        lastTimerExecution = timeNow;
        TimerInterrupt();
    }


}

void Program::TimerInterrupt()
{
    if (PSW.I == 1)
    {
        Emulator::logFile << "Masked interrupts" << endl;
        return;
    }
    if (PSW.T == 0)
    {
        Emulator::logFile << "Masked timer interrupt" << endl;
        return;
    }

    Emulator::logFile << "Timer interrupt !" << endl;

    StackPush(PSW.val);
    LR = PC;
    StackPush(LR);
    memcpy(&PC, memory + 4, 4);
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


void Program::LoadDefaultIVT(ifstream &inputFile)
{

    for (int i = 0 ; i < 16 ; i ++)
    {
        u_int32_t ivtPoint = IVT_START + i*IVT_SIZE;
        //Emulator::logFile << "Copied code to " << 4*i << " " << (int)memory[64] << endl;
        memcpy(memory + 4*i, &ivtPoint, 4);

        //Emulator::logFile << "Copied code to " << ivtPoint << " " << (int)memory[64] << endl;
        memcpy(memory + ivtPoint, &IRET_CODE, 4);
    }

    if (inputFile.is_open()) {
        string line;
        string sectionCumulate;
        bool outputSection = false;
        Section currentSection("random", 0);

        while (getline(inputFile, line)) {
            if (!outputSection) {
                if (line == "%SECTIONS SECTION%") {
                    outputSection = true;
                }
                continue;
            }
            if (line == "%END%") {
                outputSection = false;
                continue;
            }

            if (line == ".end") {
                currentSection = Section::Deserialize(sectionCumulate);

                sectionCumulate.clear();

                if (currentSection.name == ".text.start") {
                    //Emulator::logFile << "Copied code to " << IVT_START <<" " << (int)memory[64] <<  endl;
                    memcpy(memory + IVT_START, currentSection.memory, currentSection.size);
                    Emulator::logFile << "yay, loaded start ivt" << endl;
                }
                else if (currentSection.name == ".text.timer") {
                    //Emulator::logFile << "Copied code to " << IVT_START + 1*IVT_SIZE <<" " << (int)memory[64] <<  endl;
                    memcpy(memory + IVT_START + 1*IVT_SIZE, currentSection.memory, currentSection.size);
                    Emulator::logFile << "yay, loaded timer ivt" << endl;
                }
                else if (currentSection.name == ".text.illegal") {
                    //Emulator::logFile << "Copied code to " << IVT_START + 2*IVT_SIZE <<" " << (int)memory[64] <<  endl;
                    memcpy(memory + IVT_START + 2*IVT_SIZE, currentSection.memory, currentSection.size);
                    Emulator::logFile << "yay, loaded illegal ivt" << endl;
                }
                else if (currentSection.name == ".text.keyboard") {
                    //Emulator::logFile << "Copied code to " << IVT_START + 3*IVT_SIZE <<" " << (int)memory[64] <<  endl;
                    memcpy(memory + IVT_START + 3*IVT_SIZE, currentSection.memory, currentSection.size);
                    Emulator::logFile << "yay, loaded keyboard ivt" << endl;
                }

            }

            else {
                sectionCumulate += line + "\n";
            }
        }
    }
}