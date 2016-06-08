//
// Created by rols on 6/7/16.
//

#include <iostream>
#include <iomanip>

#include "../compiler/Compiler.h"
#include "Enums.h"
#include "../compiler/Instruction.h"
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
        case TokenType::SYMBOLDIFF: return out << "SYMBOLDIFF";
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


istream& operator>>(istream& in, ScopeType &s)
{
    string token;
    in >> token;

    if (token == "GLOBAL")
        s = ScopeType::GLOBAL;
    if (token == "LOCAL")
        s = ScopeType::LOCAL;

    return in;
}


ostream& operator<<(ostream& out, RelocationType s)
{
    switch(s)
    {
        case RelocationType::LONG: return out << "LONG";
        case RelocationType::INSTR: return out << "INSTR";
    }
}

istream& operator>>(istream& in, RelocationType &s)
{
    string token;
    in >> token;

    if (token == "LONG")
        s = RelocationType::LONG;
    if (token == "INSTR")
        s = RelocationType::INSTR;

    return in;
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
    out << "\tSectionName:\t" << symbol.sectionName << endl;
    out << "\tOffset:\t" << symbol.offset << endl;
    out << "\tType:\t" << symbol.scope << endl << endl;

    return out;
}

ostream& operator<<(ostream& out, Relocation& rel)
{
    out << "Relocation: " << endl;
    out << "\tSymbol:\t" << rel.symbolName << endl;
    out << "\tSection:\t" << rel.section << endl;
    out << "\tOffset:\t" << rel.offset << endl;
    out << "\tRelocationType:\t" << rel.relocationType << endl << endl;

    return out;
}


ostream& operator<<(ostream& out, Section& section)
{
    const int tokensByLine = 8;
    out << "Section: " << section.name << endl;
    out << "Size: " << dec << section.size;
    for (int i = 0; i < section.size; i ++)
    {
        if (i%tokensByLine == 0) out << endl;
        out << setfill('0') << setw(2) << hex << (u_int32_t )section.memory[i] << " ";

    }

    out << endl << endl;
}
