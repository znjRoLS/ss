//
// Created by rols on 6/7/16.
//

#include <iostream>

#include "Compiler.h"
#include "Enums.h"
#include "Instruction.h"
#include "Symbol.h"


using namespace std;

ostream& operator<<(ostream& out, State s)
{
    switch(s)
    {
        case State::LINE_END: return out << "LINE_END";
        case State::LINE_BEGIN: return out << "LINE_BEGIN";
        case State::END: return out << "END";
        case State::AFTER_EXT: return out << "AFTER_EXT";
        case State::AFTER_PUB: return out << "AFTER_PUB";
        case State::AFTER_SECTION: return out << "AFTER_SECTION";
        case State::AFTER_DIRECTIVE: return out << "AFTER_DIRECTIVE";
        case State::AFTER_LABEL: return out << "AFTER_LABEL";
    }
}

ostream& operator<<(ostream& out, TokenType s)
{
    switch(s)
    {
        case TokenType::PUB_EXT: return out << "PUB_EXT";
        case TokenType::LABEL: return out << "LABEL";
        case TokenType::SECTION: return out << "SECTION";
        case TokenType::DIRECTIVE: return out << "DIRECTIVE";
        case TokenType::INSTRUCTION: return out << "INSTRUCTION";
        case TokenType::SYMBOL: return out << "SYMBOL";
        case TokenType::OPERAND_REG: return out << "OPERAND_REG";
        case TokenType::OPERAND_DEC: return out << "OPERAND_DEC";
        case TokenType::OPERAND_HEX: return out << "OPERAND_HEX";
        case TokenType::ILLEGAL: return out << "ILLEGAL";
    }
}

ostream& operator<<(ostream& out, SectionType s)
{
    switch(s)
    {
        case SectionType::START: return out << "GLOBAL";
        case SectionType::TEXT: return out << "TEXT";
        case SectionType::DATA: return out << "DATA";
        case SectionType::BSS: return out << "BSS";
    }
}


ostream& operator<<(ostream& out, ScopeType s)
{
    switch(s)
    {
        case ScopeType::GLOBAL: return out << "GLOBAL";
        case ScopeType::LOCAL: return out << "LOCAL";
    }
}

ostream& operator<<(ostream& out, InstructionType s)
{
    switch(s)
    {
        case InstructionType::INT: return out << "INT";
        case InstructionType::ARITHMETIC: return out << "ARITHMETIC";
        case InstructionType::LOGICAL: return out << "LOGICAL";
        case InstructionType::LOADSTORE: return out << "LOADSTORE";
        case InstructionType::CALL: return out << "CALL";
        case InstructionType::IO: return out << "IO";
        case InstructionType::MOVSHIFT: return out << "MOVSHIFT";
        case InstructionType::LOADC: return out << "LOADC";
    }
}


ostream& operator<<(ostream& out, Instruction& instr)
{
    out << "Instruction: " << instr.name << endl;

    for (auto &temp: instr.parameters)
        out << "\t" << temp  << endl;

    out << "\tcode:" << hex << instr.instrCode.binaryCode << endl;

    out << endl;

    return out;
}


ostream& operator<<(ostream& out, Symbol& symbol)
{
    out << "Symbol: " << symbol.name << endl;
    out << "\tDefined:\t" << symbol.defined << endl;
    out << "\tSection:\t" << symbol.section << endl;
    out << "\toffset:\t" << symbol.offset << endl;
    out << "\tType:\t" << symbol.scope << endl;

    return out;
}
