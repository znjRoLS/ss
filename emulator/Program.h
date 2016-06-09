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

    u_int32_t registers[16];

    u_int32_t PC, LR, PSW, SP;

    u_int32_t memory[MEMORY_SIZE];

    Section programSection;

    u_int32_t startPoint;

    u_int32_t programCounter;

    Instruction currentInstruction;

    void Init();
    void ReadNext();
    void ExecuteCurrent();
    bool IsEnd();

    unordered_map<int, function<void()> > instructionExecutors;

};




#endif //SS_PROGRAM_H
