//
// Created by rols on 6/4/16.
//

#ifndef SS_INSTRUCTION_H
#define SS_INSTRUCTION_H

#include <iostream>
#include <unordered_map>

using namespace std;

class Instruction {
public:
    Instruction(string _name);

    friend ostream &operator<<(ostream &, Instruction &);

    void SetNextParameter(string param);

    static unordered_map<string,Instruction*> instructionTable;
private:
    string name;

    int numParameters;

    string parameters[3];

};


#endif //SS_INSTRUCTION_H
