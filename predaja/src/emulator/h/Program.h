//
// Created by rols on 6/9/16.
//

#ifndef SS_PROGRAM_H
#define SS_PROGRAM_H

#include <iostream>
#include <functional>
#include <chrono>

#include "Enums.h"
#include "Section.h"
#include "Instruction.h"

using namespace std;

class Program {
public:

    Program();

    //void LoadSection(istream &);
    //void LoadDefaultIVT(ifstream &);



    u_int32_t registers[16];

    u_int32_t PC, LR, SP;

    union {
        u_int32_t val;

        struct{
            u_int32_t I:1, :1, T:1, :25, Z:1, O:1, C:1, N:1;
        };
    } PSW;

    u_int8_t *memory;

    //u_int8_t stack[STACK_SIZE];

    Section programSection;

    u_int32_t startPoint;

    //u_int32_t programCounter;

    Instruction currentInstruction;

    void Init(u_int8_t *memory, u_int32_t startPoint);
    void ReadNext();
    void ExecuteCurrent();
    bool IsEnd();

    unordered_map<int, function<void()> > instructionExecutors;
    unordered_map<int, function<bool()> > conditionTesters;


    void StackPush(u_int32_t);
    u_int32_t StackPop();

    u_int32_t SignExt(u_int32_t val, size_t size);

    u_int32_t GetMemory(u_int32_t ind);
    void SetMemory(u_int32_t val, u_int32_t ind);

    void SetRegister(u_int32_t val, u_int32_t ind);
    u_int32_t GetRegister(u_int32_t ind);

    void HandleInterrupts();

    chrono::time_point<chrono::system_clock> executionStart;
    chrono::time_point<chrono::system_clock> lastTimerExecution;

    void TimerInterrupt();
    static void *KeyboardThread(void *);

    static volatile char keyboardBuf;
    static volatile bool keyboardInterrupt;

    //TODO: hardcoded
    u_int32_t IVT [16];

    pthread_t keyboardThread;

    void KeyboardInterrupt();

};




#endif //SS_PROGRAM_H
