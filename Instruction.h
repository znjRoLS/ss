//
// Created by rols on 6/4/16.
//

#ifndef SS_INSTRUCTION_H
#define SS_INSTRUCTION_H

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

class Instruction {
public:

    union instructionCode {
        struct {
            u_int32_t cond:3, flag:1, instr:4, :24;
        } instruction;

        struct {
            u_int32_t instr:8, src:4, :20;
        } instruction_int;

        struct {
            u_int32_t instr:8, dst:5, addr:1, src:5, :13;
        } instruction_arithmetic_reg;

        struct {
            u_int32_t instr:8, dst:5, addr:1, imm:18;
        } instruction_arithmetic_imm;

        struct {
            u_int32_t instr:8, dst:5, src:5, :14;
        } instruction_logical;

        struct {
            u_int32_t instr:8, a:5, r:5, f:3, ls:1, imm:10;
        } instruction_ldr_str;

        struct {
            u_int32_t instr:8, dst:5, imm:19;
        } instruction_call;

        struct {
            u_int32_t instr:8, dst:4, src:4, io:1, :15;
        } instruction_in_out;

        struct {
            u_int32_t instr:8, dst:5, src:5, imm:5, lr:1, :8;
        } instruction_mov_shr_shl;

        struct {
            u_int32_t instr:8, dst:4, hl:1, :3, c:16;
        } instruction_ldch_ldcl;

        struct {
            u_int32_t byte0:8,byte1:8, byte2:8, byte3:8;
        } bytes;

        u_int32_t binaryCode;

    } instrCode;

    Instruction(string _name, string _condition, bool _setFlags);

    friend ostream &operator<<(ostream &, Instruction &);

    //void SetNextParameter(string param);

    //static unordered_map<string,Instruction*> instructionTable;

    //static Instruction* ParseInstruction(string instructionName, vector<string>& parameters);
    //static void ParseInstruction(string instructionName, vector<string>& parameters, vector<Instruction*>& instructions, u_int32_t& currentVA);

    //static u_int32_t GetBinaryFromInstruction(string instruction, string condition, string setFlags, vector<string> &parameters);

//    /u_int32_t instrCode;

    string name;
    string condition;
    bool setFlags;
    vector<string> parameters;

};


#endif //SS_INSTRUCTION_H
