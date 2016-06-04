//
// Created by rols on 6/4/16.
//

#ifndef SS_OPERAND_H
#define SS_OPERAND_H

class Operand
{
public:
    enum OperandType {REG, IMM};

    Operand( string input );


    OperandType operandType;
    int regNum;
    u_int32_t immVal;
};

#endif //SS_OPERAND_H
