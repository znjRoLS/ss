//
// Created by rols on 6/9/16.
//

#ifndef SS_PROGRAM_H
#define SS_PROGRAM_H

#include <iostream>
#include <functional>

#include "Enums.h"
#include "Section.h"
#include "Instruction.h"

using namespace std;

class Program {
public:

    Program();

    void LoadSection(istream &);

    static const size_t MEMORY_SIZE = 1024 * 1024;
    static const size_t STACK_SIZE = 8 * 1024;

    u_int32_t registers[16];

    u_int32_t PC, LR, SP;

    union {
        u_int32_t val;

        struct{
            u_int32_t I:1, :27, Z:1, O:1, C:1, N:1;
        };
    } PSW;

    u_int8_t memory[MEMORY_SIZE];

    u_int8_t stack[STACK_SIZE];

    Section programSection;

    u_int32_t startPoint;

    //u_int32_t programCounter;

    Instruction currentInstruction;

    void Init();
    void ReadNext();
    void ExecuteCurrent();
    bool IsEnd();

    unordered_map<int, function<void()> > instructionExecutors;


    void StackPush(u_int32_t);
    u_int32_t StackPop();

    u_int32_t SignExt(u_int32_t val, size_t size);

    u_int32_t GetMemory(u_int32_t ind);
    void SetMemory(u_int32_t val, u_int32_t ind);

    void SetRegister(u_int32_t val, u_int32_t ind);
    u_int32_t GetRegister(u_int32_t ind);
};




#endif //SS_PROGRAM_H
