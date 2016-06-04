//
// Created by rols on 6/4/16.
//

#include "Instruction.h"

#include <regex>

//Instruction::Instruction(string _name):
//        name(_name)
//{
//        numParameters = 0;
//};

//unordered_map<string,Instruction*> Instruction::instructionTable =
//        {
//                {"add", new Instruction("add")},
//                {"cmp", new Instruction("cmp")},
//                {"je", new Instruction("je")},
//                {"out", new Instruction("out")},
//                {"ldr", new Instruction("ldr")},
//                {"lds", new Instruction("lds")},
//        };

unordered_map<string, unsigned char> instructionCodes =
    {
        {"int",  0},
        {"add",  1},
        {"sub",  2},
        {"mul",  3},
        {"div",  4},
        {"cmp",  5},
        {"and",  6},
        {"or",   7},
        {"not",  8},
        {"test", 9},
        {"ldr",  10},
        {"str",  10},
        {"call", 12},
        {"in",   13},
        {"out",  13},
        {"mov",  14},
        {"shr",  14},
        {"shl",  14},
        {"ldch", 15},
        {"ldcl", 15}
    };

unordered_map<string, unsigned char> branchCodes =
    {
        {"eq", 0},
        {"ne", 1},
        {"gt", 2},
        {"ge", 3},
        {"lt", 4},
        {"le", 5},
        {"",   6},
        {"al", 7},
    };


void Instruction::SetNextParameter(string param) {
    parameters[numParameters++] = param;
}


void pushOperandToCode(string operand, int numBits, u_int32_t &binaryCode, bool isRegType) {
    u_int32_t mask = 1U << numBits;
    mask--;

    if (isRegType) {
        if (operand[0] != 'r') {
            cout << "error" << endl;
            throw "rand";
        }

        operand = operand.substr(1);
    }

    else if (operand[0] == 'r') {
        cout << "error" << endl;
        throw "rand";
    }

    u_int32_t operandNum;// = atoi(operand.c_str());

    stringstream ss;

    ss << operand;
    ss >> hex >> operandNum;

    cout << "cast operand " << operand << " to " << operandNum << " " << hex << operandNum << endl;

    operandNum &= mask;

    binaryCode <<= numBits;
    binaryCode |= operandNum;
}


u_int32_t Instruction::GetBinaryFromInstruction(string instruction, string condition, string setFlags, vector<string> &parameters)
{
    u_int32_t binaryCode = 0;

    binaryCode |= branchCodes[condition];

    binaryCode <<= 1;
    binaryCode |= (setFlags == "s");

    binaryCode <<= 4;
    binaryCode |= instructionCodes[instruction];

    switch (instructionCodes[instruction]) {
        case 0:
            pushOperandToCode(parameters[0], 4, binaryCode, false);
            binaryCode <<= 20;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            pushOperandToCode(parameters[0], 5, binaryCode, true);
            if (parameters[1][0] == 'r') {
                binaryCode <<= 1;
                pushOperandToCode(parameters[1], 5, binaryCode, true);
                binaryCode <<= 13;
            }
            else {
                binaryCode <<= 1;
                binaryCode |= 1;
                pushOperandToCode(parameters[1], 18, binaryCode, false);
            }
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            pushOperandToCode(parameters[0], 5, binaryCode, true);
            pushOperandToCode(parameters[1], 5, binaryCode, true);
            binaryCode <<= 14;
            break;
        case 10:
            break;
        case 12:
            pushOperandToCode(parameters[0], 5, binaryCode, true);
            pushOperandToCode(parameters[1], 19, binaryCode, false);
            break;
        case 13:
            pushOperandToCode(parameters[0], 4, binaryCode, true);
            pushOperandToCode(parameters[1], 4, binaryCode, true);
            binaryCode <<= 1;
            if (instruction == "in")
                binaryCode |= 1;
            binaryCode <<= 15;
            break;
        case 14:
            pushOperandToCode(parameters[0], 5, binaryCode, true);
            pushOperandToCode(parameters[1], 5, binaryCode, true);
            if (instruction != "mov") {
                pushOperandToCode(parameters[2], 5, binaryCode, false);
            }
            else {
                binaryCode <<= 5;
            }
            binaryCode <<= 1;
            if (instruction == "shl") {
                binaryCode |= 1;
            }
            binaryCode <<= 8;
            break;
        case 15:
            pushOperandToCode(parameters[0], 4, binaryCode, true);
            binaryCode <<= 1;
            if (instruction == "ldch") {
                binaryCode |= 1;
            }
            binaryCode <<= 3;
            pushOperandToCode(parameters[1], 16, binaryCode, false);
            break;
    }

    return binaryCode;


}


void Instruction::ParseInstruction(string instructionName, vector<string> &parameters, vector<Instruction*>& instructions, u_int32_t& currentVA) {


    regex r(
        "^(int|add|sub|mul|div|cmp|and|or|not|test|ldr|str|call|in|out|mov|shr|shl|ldch|ldcl)(eq|ne|gt|ge|lt|le|al)?(s)?$");

    smatch base_match;

    if (regex_match(instructionName, base_match, r)) {
        cout << "success! " << instructionName << endl;
        for (int i = 0; i < base_match.size(); i++) {
            cout << "\t" << i << ": " << base_match[i] << endl;
        }

        if (base_match[1] == "ldc")
        {
            string cParameter = parameters[1];

            u_int32_t cParameterVal = strtoul(cParameter.c_str(), NULL, 0);

            u_int16_t cParameterValLow = cParameterVal && ((1U << 16) - 1);

            u_int16_t cParameterValHigh = (cParameterVal>>16) && ((1U << 16) - 1);

            Instruction *instr = new Instruction();
            instr->name = instructionName;
            for (int i = 0; i < parameters.size(); i++) {
                instr->parameters[i] = parameters[i];
            }
            instr->numParameters = parameters.size();

            stringstream ss;
            ss << cParameterValHigh;
            parameters[1] = ss.str();

            instr->instrCode = GetBinaryFromInstruction("ldch", base_match[2], base_match[3], parameters);
            instructions.push_back(instr);

            instr = new Instruction();
            instr->name = instructionName;
            for (int i = 0; i < parameters.size(); i++) {
                instr->parameters[i] = parameters[i];
            }
            instr->numParameters = parameters.size();

            stringstream sss;
            sss << cParameterValLow;
            parameters[1] = ss.str();

            instr->instrCode = GetBinaryFromInstruction("ldch", base_match[2], base_match[3], parameters);
            instructions.push_back(instr);

            currentVA += 8;
        }

        else
        {
            Instruction *instr = new Instruction();
            instr->name = instructionName;
            for (int i = 0; i < parameters.size(); i++) {
                instr->parameters[i] = parameters[i];
            }
            instr->numParameters = parameters.size();

            instr->instrCode = GetBinaryFromInstruction(base_match[1], base_match[2], base_match[3], parameters);
            currentVA += 4;

            instructions.push_back(instr);
        }
    }
    else {
        cout << "fail! " << instructionName << endl;
        throw "randommmm";
    }


    //return instr;

}