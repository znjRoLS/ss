//
// Created by rols on 6/7/16.
//

#ifndef SS_ENUMS_H
#define SS_ENUMS_H

enum State {LINE_BEGIN, END, AFTER_EXT, AFTER_PUB, AFTER_SECTION, AFTER_DIRECTIVE, AFTER_LABEL, LINE_END};

enum TokenType {PUB_EXT, LABEL, SECTION, DIRECTIVE, INSTRUCTION, OPERAND_REG, OPERAND_REGINCDEC, OPERAND_REGSPEC, OPERAND_REGSPECINCDEC, OPERAND_DEC, OPERAND_HEX, ILLEGAL, SYMBOL, SYMBOLDIFF};

enum InstructionType {INT, ARITHMETIC, LOGICAL, LOADSTORE, CALL, IO, MOVSHIFT, LOADC};

enum SectionType { START, TEXT, DATA, BSS};

enum ScopeType { GLOBAL, LOCAL };

enum RelocationType { LONG, LDCRELOC };

namespace InstructionCodes
{
    enum InstructionSymbol {
        INT = 0,
        ADD = 1,
        SUB = 2,
        MUL = 3,
        DIV = 4,
        CMP = 5,
        AND = 6,
        OR = 7,
        NOT = 8,
        TEST = 9,
        LDRSTR = 10,
        NO_INSTRUCTION = 11,
        CALL = 12,
        INOUT = 13,
        MOVSHIFT = 14,
        LOADC = 15,
    };

    enum InstructionCondition {
        EQ = 0,
        NE = 1,
        GT = 2,
        GE = 3,
        LT = 4,
        LE = 5,
        NO_CONDITION = 6,
        AL = 7
    };
}






#endif //SS_ENUMS_H
