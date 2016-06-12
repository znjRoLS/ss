//
// Created by rols on 6/9/16.
//

#include "Program.h"
#include "LoaderConf.h"

#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Instruction.h"
#include "Emulator.h"
#include <pthread.h>


using namespace std;

volatile char Program::keyboardBuf;
volatile bool Program::keyboardInterrupt;

ostream& operator<<(ostream& out, InstructionCodes::InstructionSymbol s);

ostream& operator<<(ostream& out, InstructionCodes::InstructionCondition s);

Program::Program():
    programSection("output", 0),
    currentInstruction(InstructionCodes::HALT, InstructionCodes::NO_CONDITION, false)
{

    conditionTesters =
        {
            {
                InstructionCodes::EQ, [&](){
                    return PSW.Z == 1;
            }
            },
            {
                InstructionCodes::NE, [&](){
                return PSW.Z == 0;
            }
            },
            {
                InstructionCodes::GT, [&](){
                return (PSW.Z == 0 && PSW.N == 0);
            }
            },
            {
                InstructionCodes::GE, [&](){
                return (PSW.N == 0);
            }
            },
            {
                InstructionCodes::LT, [&](){
                return (PSW.N == 1);
            }
            },
            {
                InstructionCodes::LE, [&](){
                return (PSW.Z == 1 || PSW.N == 1);
            }
            },
            {
                InstructionCodes::NO_CONDITION, [&](){
                return true;
            }
            },
            {
                InstructionCodes::AL, [&](){
                return true;
            }
            },
        };


    instructionExecutors =
        {
            {
                InstructionCodes::INT, [&](){

                    StackPush(LR);
                    LR = PC;
                    StackPush(PSW.val);
                    PSW.I = 1;

                    u_int32_t intNum = currentInstruction.instrCode.instruction_int.src;// + CODE_START;

                    //memcpy(&PC, memory + 4*intNum + CODE_START, 4);
                    memcpy(&PC, memory + 4*intNum, 4);

                Emulator::logFile << "Operands: " << hex << intNum << endl;
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
                    Emulator::logFile << "Operands: " << hex <<firstOperand << " " << secondOperand << endl;

                }
            },
            {
                InstructionCodes::SUB, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_arithmetic_reg.dst;
                    result = GetRegister(regDest);

                    u_int32_t firstOperand = result;
                    u_int32_t secondOperand;

                    //cout << currentInstruction.instrCode.instruction_arithmetic_imm.imm << endl;
                    //cout << PC << endl;

                    if (currentInstruction.instrCode.instruction_arithmetic_reg.addr == 1)
                    {
                        secondOperand = SignExt(currentInstruction.instrCode.instruction_arithmetic_imm.imm, 18);
                        //cout << "yep" << endl;
                    }
                    else
                    {
                        secondOperand = GetRegister(currentInstruction.instrCode.instruction_arithmetic_reg.src);
                    }

                    result -= secondOperand;

                    //cout << "sec " << secondOperand << endl;
                    //cout << "fir " << firstOperand << endl;

                    SetRegister(result, regDest);

                    if (currentInstruction.setFlags)
                    {
                        PSW.Z = result == 0;

                        PSW.O = (int32_t)firstOperand > 0 && (int32_t)secondOperand > 0 && (int32_t)result < 0 ||
                                (int32_t)firstOperand < 0 && (int32_t)secondOperand < 0 && (int32_t)result > 0;

                        PSW.N = (int32_t)result < 0;

                        PSW.C = (u_int32_t)result < 0 && (u_int32_t)firstOperand >= 0;
                    }
                Emulator::logFile << "Operands: " << hex <<firstOperand << " " << secondOperand << endl;

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
                Emulator::logFile << "Operands: " << hex <<firstOperand << " " << secondOperand << endl;

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
                Emulator::logFile << "Operands: " << hex <<firstOperand << " " << secondOperand << endl;

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
                Emulator::logFile << "Operands: " << hex <<firstOperand << " " << secondOperand << endl;

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
                Emulator::logFile << "Operands: " << hex <<regDest << " " << regSrc << endl;
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
                Emulator::logFile << "Operands: " << hex <<regDest << " " << regSrc << endl;
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
                Emulator::logFile << "Operands: " << hex <<regDest << " " << regSrc << endl;
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
                Emulator::logFile << "Operands: " << hex <<regDest << " " << regSrc << endl;
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

                Emulator::logFile << "Operands: " << hex <<regDest << " " << regSrc << " " << imm << " " << f << endl;
                }
            },
            {
                InstructionCodes::CALL, [&](){

                    u_int32_t result;
                    u_int32_t regDest = currentInstruction.instrCode.instruction_call.dst;
                    u_int32_t imm = SignExt(currentInstruction.instrCode.instruction_call.imm,19);

                    StackPush(LR);
                    LR = PC;

                    PC = GetRegister(regDest) + imm;// + CODE_START;
                Emulator::logFile << "Operands: " << hex <<regDest << " " << imm << " " <<  endl;
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

                Emulator::logFile << "Operands: " << hex <<regDest << " " << regSrc << " " << endl;
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

                    //iret
                    if (regDest == 16 && regSrc == 17 && currentInstruction.setFlags)
                    {
                        PSW.val = StackPop();
                    }

                    if (regDest == 16 && regSrc == 17)
                    {
                        LR = StackPop();
                    }

                    //not same address space
//                    if (regDest == 16 && regSrc != 17)
//                    {
//                        PC += CODE_START;
//                    }
                Emulator::logFile << "Operands: " << hex <<regDest << " " << regSrc << " " << endl;

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

                Emulator::logFile << "Operands: " << hex <<regDest << " " << imm << endl;
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

    Emulator::logFile << "Fetched memory from " << ind << " - " << res<<endl;

    return res;
}

void Program::SetMemory(u_int32_t val, u_int32_t ind)
{
    if (ind >= MEMORY_SIZE)
    {
        throw runtime_error("Index for memory out of bounds !");
    }

    Emulator::logFile << "Set memory " << ind << " - " << val<<endl;

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

    Emulator::logFile << "Stack push " << hex << val << endl;

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

    SP += 4;

    Emulator::logFile << "Stack pop " << hex << ret << endl;

    return ret;
}


void Program::Init(u_int8_t *_memory, u_int32_t _startPoint)
{
    memory = _memory;
    startPoint = _startPoint;

    PC = startPoint;
    SP = STACK_SIZE + STACK_START;

    PSW.val = 0;
    PSW.T = 1;

    executionStart = chrono::system_clock::now();
    lastTimerExecution = executionStart;

    int iret1 = pthread_create( &keyboardThread, NULL, KeyboardThread, nullptr);

    if(iret1)
    {
        throw runtime_error("Error - pthread_create() return code: " + iret1);
    }

    //call first routine
    StackPush(LR);
    LR = PC;
    StackPush(PSW.val);
    PSW.I = 1;

    memcpy(&PC, memory, 4);

    //PC += CODE_START;

}


void *Program::KeyboardThread(void *ptr)
{
    while (true)
    {
        while (keyboardInterrupt);

        //cout << "Passed barrier" << endl;
        //Emulator::logFile << "Passed barrier" << endl;

        u_int8_t temp;

        cin >> noskipws >> temp;

        //cout << "Read char " << temp << endl;
        //Emulator::logFile << "Read char " << temp << endl;

        keyboardBuf = temp;

        keyboardInterrupt = true;
    }
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
    Emulator::logFile << "PC: " << hex << PC << " ";
    Emulator::logFile << "LR: " << hex << LR << " ";
    Emulator::logFile << "SP: " << hex << SP << " ";
    Emulator::logFile << "PSW: " << hex << PSW.val << endl;

    Emulator::logFile << "Executing: " << currentInstruction.instructionSymbol << " " << currentInstruction.instructionCondition << " " << currentInstruction.setFlags << " " << currentInstruction.instrCode.binaryCode << endl;
    Emulator::logFile << hex << currentInstruction.instrCode.bytes.byte0 << " ";
    Emulator::logFile << hex << currentInstruction.instrCode.bytes.byte1 << " ";
    Emulator::logFile << hex << currentInstruction.instrCode.bytes.byte2 << " ";
    Emulator::logFile << hex << currentInstruction.instrCode.bytes.byte3 << endl;

    if (conditionTesters[currentInstruction.instructionCondition]())
    {
        instructionExecutors[currentInstruction.instructionSymbol]();
    }
    else
    {
        Emulator::logFile << "Skipping instruction" << endl;
    }


    Emulator::logFile << "Registers: ";
    for (int i = 0 ; i < 16;  i ++)
        Emulator::logFile << " " << hex << registers[i];

    Emulator::logFile << endl;
    Emulator::logFile << "PC: " << hex << PC << " ";
    Emulator::logFile << "LR: " << hex << LR << " ";
    Emulator::logFile << "SP: " << hex << SP << " ";
    Emulator::logFile << "PSW: " << hex << PSW.val << endl;

    HandleInterrupts();

    Emulator::logFile << endl;

}

void Program::HandleInterrupts()
{

    chrono::time_point<chrono::system_clock> timeNow = chrono::system_clock::now();
    chrono::duration<double> elapsed = timeNow - lastTimerExecution;
    //TODO: hardcoded
    //Emulator::logFile << dec << "elapsed: " << elapsed.count() << endl;
    if (elapsed.count() >= 1.0)
    {
        lastTimerExecution = timeNow;
        TimerInterrupt();
    }

    if (keyboardInterrupt)
    {
        KeyboardInterrupt();
    }

    if (memory[OUTPUT_POS] != 0)
    {
        char temp = memory[OUTPUT_POS];
        memory[OUTPUT_POS] = 0;

        cout << temp;
        cout.flush();
        //cout << "Print char ! : " << temp << endl;
        Emulator::logFile << "Print char: " << temp << endl;
    }


}


void Program::KeyboardInterrupt()
{
    if (PSW.I == 1)
    {
        Emulator::logFile << "Masked interrupts (keyboard)" << endl;
        return;
    }

    Emulator::logFile << "Keyboard interrupt !" << endl;

    //copy the contents of buf

    u_int32_t temp = keyboardBuf;
    keyboardInterrupt = false;

    memcpy(memory + KEYBOARD_POS, &temp, 4);
    memcpy(memory + KEYBOARD_STATUS_POS, &KEYBOARD_STATUS_MASK, 4);

    StackPush(LR);
    LR = PC;
    StackPush(PSW.val);
    PSW.I = 1;

    memcpy(&PC, memory + 12, 4);

    //PC += CODE_START;
}

void Program::TimerInterrupt()
{
    if (PSW.I == 1)
    {
        Emulator::logFile << "Masked interrupts (timer)" << endl;
        return;
    }
    if (PSW.T == 0)
    {
        Emulator::logFile << "Masked timer interrupt" << endl;
        return;
    }

    Emulator::logFile << "Timer interrupt !" << endl;


    StackPush(LR);
    LR = PC;
    StackPush(PSW.val);
    PSW.I = 1;

    memcpy(&PC, memory + 4, 4);

    //PC += CODE_START;
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