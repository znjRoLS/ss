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

#endif //SS_ENUMS_H
