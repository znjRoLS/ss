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
    //Instruction(string _name);

    friend ostream &operator<<(ostream &, Instruction &);

    void SetNextParameter(string param);

    static unordered_map<string,Instruction*> instructionTable;

    //static Instruction* ParseInstruction(string instructionName, vector<string>& parameters);
    static void ParseInstruction(string instructionName, vector<string>& parameters, vector<Instruction*>& instructions, u_int32_t& currentVA);
private:

    static u_int32_t GetBinaryFromInstruction(string instruction, string condition, string setFlags, vector<string> &parameters);

    u_int32_t instrCode;

    string name;

    int numParameters;

    string parameters[3];

};


#endif //SS_INSTRUCTION_H
