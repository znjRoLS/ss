//
// Created by rols on 6/2/16.
//

#include <cstring>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <memory>
#include <queue>

#include "Compiler.h"
#include "Instruction.h"


ofstream Compiler::logFile("../../log");

ostream& operator<<(ostream& out, TokenType s);
ostream& operator<<(ostream& out, State s);

unordered_map<string, u_int8_t> Compiler::instructionCodes =
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

unordered_map<string, u_int8_t> Compiler::branchCodes =
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


unordered_map<string, int> Compiler::instructionTypesMap =
    {
        {"int",  INT},
        {"add",  ARITHMETIC},
        {"sub",  ARITHMETIC},
        {"mul",  ARITHMETIC},
        {"div",  ARITHMETIC},
        {"cmp",  ARITHMETIC},
        {"and",  LOGICAL},
        {"or",   LOGICAL},
        {"not",  LOGICAL},
        {"test", LOGICAL},
        {"ldr",  LOADSTORE},
        {"str",  LOADSTORE},
        {"call", CALL},
        {"in",   IO},
        {"out",  IO},
        {"mov",  MOVSHIFT},
        {"shr",  MOVSHIFT},
        {"shl",  MOVSHIFT},
        {"ldch", LOADC},
        {"ldcl", LOADC}
    };


unordered_map<int, function<void(Instruction&, queue<string>&)> > Compiler::instructionsHandlers =
    {
        {INT, [](Instruction &instr, queue<string> &tokens) {

            string token;
            u_int32_t operand;
            TokenType operandType;
            GetOperand(tokens, token, operand, operandType, {OPERAND_HEX, OPERAND_DEC}, 4);

            instr.instrCode.instruction_int.src = operand;
        }
        },
        {ARITHMETIC, [](Instruction &instr, queue<string> &tokens) {

            string token1;
            u_int32_t operand1;
            TokenType operandType1;
            GetOperand(tokens, token1, operand1, operandType1, {OPERAND_REG, OPERAND_REGSPEC});

            bool addsub = true;
            if (instr.name == "mul" || instr.name == "div" || instr.name == "cmp")
                addsub = false;

            if (!addsub && operandType1 == OPERAND_REGSPEC)
            {
                throw runtime_error("Not allowed to use pc, lr, sp or psw in mul,div or cmp");
            }

            if (token1 == "psw")
            {
                throw runtime_error("Not allowed to use psw in add or sub");
            }

            string token2;
            u_int32_t operand2;
            TokenType operandType2;
            GetOperand(tokens, token2, operand2, operandType2, {OPERAND_REG, OPERAND_HEX, OPERAND_DEC}, 18);

            if (operandType2 == OPERAND_REG || operandType2 == OPERAND_REGSPEC)
            {
                if (!addsub && operandType2 == OPERAND_REGSPEC)
                {
                    throw runtime_error("Not allowed to use pc, lr, sp or psw in mul,div or cmp");
                }

                if (token2 == "psw")
                {
                    throw runtime_error("Not allowed to use psw in add or sub");
                }

                instr.instrCode.instruction_arithmetic_reg.dst = operand1;
                instr.instrCode.instruction_arithmetic_reg.src = operand2;
                instr.instrCode.instruction_arithmetic_reg.addr = 0;
            }
            else
            {
                instr.instrCode.instruction_arithmetic_imm.dst = operand1;
                instr.instrCode.instruction_arithmetic_imm.imm = operand2;
                instr.instrCode.instruction_arithmetic_imm.addr = 1;
            }

        }
        },
        {LOGICAL, [](Instruction &instr, queue<string> &tokens) {

            string token;
            u_int32_t operand;
            TokenType operandType;

            GetOperand(tokens, token, operand, operandType, {OPERAND_REG, OPERAND_REGSPEC});
            instr.instrCode.instruction_logical.dst = operand;

            if (operandType == OPERAND_REGSPEC && token != "sp")
            {
                throw runtime_error("Not allowed to use pc, lr, psw with and, or, not and test");
            }

            GetOperand(tokens, token, operand, operandType, {OPERAND_REG, OPERAND_REGSPEC});
            instr.instrCode.instruction_logical.src = operand;

            if (operandType == OPERAND_REGSPEC && token != "sp")
            {
                throw runtime_error("Not allowed to use pc, lr, psw with and, or, not and test");
            }
        }
        },
        {LOADSTORE, [](Instruction &instr, queue<string> &tokens) {

            string token;
            u_int32_t operand;
            TokenType operandType;

            GetOperand(tokens, token, operand, operandType, {OPERAND_REG});
            instr.instrCode.instruction_ldr_str.r = operand;

            if (token == "psw")
            {
                throw runtime_error("Not allowed to use psw in ldr and str");
            }

            GetOperand(tokens, token, operand, operandType, {OPERAND_REG, OPERAND_REGINCDEC, OPERAND_REGSPEC, OPERAND_REGSPECINCDEC});
            instr.instrCode.instruction_ldr_str.a = operand;

            bool isIncDec = (operandType == OPERAND_REGINCDEC || operandType == OPERAND_REGSPECINCDEC);
            bool isPost, isInc;
            u_int32_t f;

            if (isIncDec)
            {
                if (token[0] == '+'){ isPost = false; isInc = true;}
                if (token[0] == '-'){ isPost = false; isInc = false;}
                if (token[token.size()-1] == '+'){ isPost = true; isInc = true;}
                if (token[token.size()-1] == '-'){ isPost = true; isInc = false;}
            }

            if (isIncDec && token == "pc")
            {
                throw runtime_error("Not allowed to inc/dec pc register");
            }

            f = (token == "pc")?0:1;

            if (isIncDec)
            {
                f <<= 1;
                if (!isPost)
                    f <<= 1;
                if (!isInc)
                    f ++;
            }

            instr.instrCode.instruction_ldr_str.f = f;

            GetOperand(tokens, token, operand, operandType, {OPERAND_DEC, OPERAND_HEX}, 10);
            instr.instrCode.instruction_ldr_str.imm = operand;
        }
        },
        {CALL, [](Instruction &instr, queue<string> &tokens) {

            string token;
            u_int32_t operand;
            TokenType operandType;

            GetOperand(tokens, token, operand, operandType, {OPERAND_REG, OPERAND_REGSPEC});
            instr.instrCode.instruction_call.dst = operand;

            GetOperand(tokens, token, operand, operandType, {OPERAND_DEC, OPERAND_HEX}, 19);
            instr.instrCode.instruction_call.imm = operand;
        }
        },
        {IO, [](Instruction &instr, queue<string> &tokens) {

            string token;
            u_int32_t operand;
            TokenType operandType;

            GetOperand(tokens, token, operand, operandType, {OPERAND_REG});
            instr.instrCode.instruction_in_out.dst = operand;

            GetOperand(tokens, token, operand, operandType, {OPERAND_REG});
            instr.instrCode.instruction_in_out.src = operand;

            instr.instrCode.instruction_in_out.io = (instr.name == "in")?1:0;
        }
        },
        {MOVSHIFT, [](Instruction &instr, queue<string> &tokens) {

            string token;
            u_int32_t operand;
            TokenType operandType;

            GetOperand(tokens, token, operand, operandType, {OPERAND_REG, OPERAND_REGSPEC});
            instr.instrCode.instruction_mov_shr_shl.dst = operand;

            GetOperand(tokens, token, operand, operandType, {OPERAND_REG, OPERAND_REGSPEC});
            instr.instrCode.instruction_mov_shr_shl.src = operand;

            if (instr.name != "mov")
            {
                GetOperand(tokens, token, operand, operandType, {OPERAND_DEC, OPERAND_HEX}, 0); // 0 or 6 ? because unsigned 5, TODO@rols: check this
                instr.instrCode.instruction_mov_shr_shl.imm = operand;

                instr.instrCode.instruction_mov_shr_shl.lr = (instr.name == "shl")?1:0;
            }
        }
        },
        {LOADC, [](Instruction &instr, queue<string> &tokens) {

            string token;
            u_int32_t operand;
            TokenType operandType;

            GetOperand(tokens, token, operand, operandType, {OPERAND_REG});
            instr.instrCode.instruction_ldch_ldcl.dst = operand;

            GetOperand(tokens, token, operand, operandType, {OPERAND_DEC, OPERAND_HEX}, 16);
            instr.instrCode.instruction_ldch_ldcl.c = operand;

            instr.instrCode.instruction_ldch_ldcl.hl = (instr.name == "ldch")?1:0;
        }
        },
    };

unordered_map<string, u_int32_t> Compiler::specialRegisterValues =
    {
        {"pc", 16},
        {"sp", 17},
        {"lr", 18},
        {"psw", 19}
    };


unordered_map<int, regex> Compiler::tokenParsers =
    {
        { PUB_EXT, regex("^(.public|.extern)$")},
        { LABEL, regex("^([a-zA-Z_][a-zA-Z0-9]*):$")},
        { SECTION, regex("^.(text|data|bss)(.[a-zA-Z_][a-zA-Z0-9]*)?$")},
        { DIRECTIVE, regex("^.(char|word|long|align|skip)$")},
        { SYMBOL, regex("^([a-zA-Z_][a-zA-Z0-9]*)$")},
        { OPERAND_REG, regex("^r([0-9]{1,2})$")},
        { OPERAND_REGINCDEC, regex("^(?:(?:(?:\\+\\+|--)r([0-9]{1,2}))|(?:r([0-9]{1,2})(?:\\+\\+|--)))$")},
        { OPERAND_REGSPEC, regex("^(pc|sp|lr|psw)")},
        { OPERAND_REGSPECINCDEC, regex("^(?:(?:(?:\\+\\+|--)(pc|sp|lr|psw))|(?:(pc|sp|lr|psw)(?:\\+\\+|--)))$")},
        { OPERAND_DEC, regex("^([0-9]*)$")},
        { OPERAND_HEX, regex("^(0x[0-9abcdef]*)$")},
        { INSTRUCTION, regex("^(int|add|sub|mul|div|cmp|and|or|not|test|ldr|str|call|in|out|mov|shr|shl|ldch|ldcl)(eq|ne|gt|ge|lt|le|al)?(s)?$")}
    };

Compiler::Compiler()
{

}


Compiler::~Compiler()
{

}


void split(const string &s, const char* delim, vector<string> & v){
    // to avoid modifying original string
    // first duplicate the original string and return a char pointer then free the memory
    char * dup = strdup(s.c_str());
    char * token = strtok(dup, delim);
    while(token != NULL){
        v.push_back(string(token));
        // the call is treated as a subsequent calls to strtok:
        // the function continues from where it left in previous invocation
        token = strtok(NULL, delim);
    }
    free(dup);
}



void Compiler::Compile(ifstream& inputFile, ofstream& outputFile)
{
    try{
        logFile << "Compiling " << endl;

        LoadAssemblyFromFile(inputFile);
        logFile << "FirstRun " << endl;
        FirstRun();
        logFile << "SecondRun " << endl;
        SecondRun();
        logFile << "Writing object file " << endl;
        WriteObjectFile(outputFile);
    }
    catch( exception& e)
    {
        cerr << e.what() << endl;
        logFile << "ERROR: " << e.what() << endl;
    }
}


void Compiler::LoadAssemblyFromFile(ifstream &inputFile)
{
    string line;

    while( getline(inputFile, line) ) {

        //strip comments
        size_t found = line.find(';');
        if (found != string::npos) {
            line = line.substr(0, found);
        }

        vector<string> tokens;
        split(line, " ,\t\n", tokens);

        if (tokens.size() == 0)
            continue;

        if (tokens[0] == ".end")
            break;

        assemblyInput.push_back(tokens);
    }
}






void Compiler::FirstRun()
{
    //u_int32_t locationCounter = 0;
    u_int32_t offsetCounter = 0;
    queue<string> tokensQueue;
    SectionType currentSectionType = START;
    string currentSection = "global";
    State currentState;

    unordered_map<int, function<State()> > stateMachine =
        {
            {
                LINE_BEGIN, [&] {
                    //tokensQueue.empty will never be true ?

                    string currentToken = tokensQueue.front();
                    TokenType currentTokenType = ParseToken(currentToken);
                    if (currentTokenType == LABEL)
                    {
                        string labelName = currentToken.substr(0, currentToken.size() - 1);

                        tokensQueue.pop();

                        AddNewSymbol(labelName, true, currentSectionType, currentSection, ScopeType::LOCAL, offsetCounter);
                    }

                    return AFTER_LABEL;
                }
            },
            {
                AFTER_LABEL, [&](){

                if (tokensQueue.empty())
                {
                    return END;
                }

                string currentToken = tokensQueue.front();
                TokenType currentTokenType = ParseToken(currentToken);

                tokensQueue.pop();

                switch (currentTokenType)
                {
                    case PUB_EXT:
                        if (currentSectionType != START)
                        {
                            throw runtime_error("Not allowed to write public/extern not in GLOBAL section!");
                        }
                        while (!tokensQueue.empty()) tokensQueue.pop();
                        return LINE_END;
                    case DIRECTIVE:
                        if (currentSectionType != DATA)
                        {
                            throw runtime_error("Not allowed to write directives not in DATA section!");
                        }
                        HandleDirective(currentToken, tokensQueue, offsetCounter, currentSection, false);
                        return LINE_END;
                    case SECTION:
                        sections[currentSection] = new u_int8_t[offsetCounter];
                        currentSection = currentToken;
                        UpdateCurrentSection(currentToken, currentSectionType, offsetCounter);
                        AddNewSymbol(currentToken, true, currentSectionType, currentSection, ScopeType::LOCAL, offsetCounter);
                        return LINE_END;
                    case INSTRUCTION:
                        offsetCounter += 4;
                        while (!tokensQueue.empty()) tokensQueue.pop();
                        return LINE_END;
                    default:
                        stringstream ss;
                        ss << "Token not allowed here! " << currentTokenType << ": " << currentToken;
                        throw runtime_error(ss.str());
                }
            }
            },
                {
                    LINE_END, [&](){

                    if (!tokensQueue.empty())
                    {
                        throw runtime_error("Expected end of line, but token found! " + tokensQueue.front());
                    }

                    return END;
                }
            },
        };

    for (auto &tokens: assemblyInput)
    {
        for (auto &token: tokens)
        {
            tokensQueue.push(token);
        }

        currentState = LINE_BEGIN;

        while (currentState != END)
        {
            logFile << "current state " << currentState << endl;
            logFile << "current counter " << offsetCounter << endl;
            if (!tokensQueue.empty())
            {
                logFile << "current token type " << ParseToken(tokensQueue.front()) << endl;
                logFile << "current token " << tokensQueue.front() << endl;

            }
            else
            {
                logFile << "empty tokensqueue" << endl;
            }

            logFile << endl;

            logFile.flush();

            currentState = stateMachine[currentState]();
        }
    }

}


void Compiler::SecondRun()
{
    u_int32_t offsetCounter = 0;
    queue<string> tokensQueue;
    SectionType currentSectionType = START;
    string currentSection = "global";
    State currentState;

    unordered_map<int, function<State()> > stateMachine =
        {
            {
                LINE_BEGIN, [&] {
                    //tokensQueue.empty will never be true ?

                    string currentToken = tokensQueue.front();
                    TokenType currentTokenType = ParseToken(currentToken);
                    if (currentTokenType == LABEL)
                    {
                        tokensQueue.pop();
                    }

                    return AFTER_LABEL;
                }
            },
            {
                AFTER_LABEL, [&](){

                    if (tokensQueue.empty())
                    {
                        return END;
                    }

                    string currentToken = tokensQueue.front();
                    //cout << "currentToken " << currentToken << " tokensqueue " << tokensQueue.front() << endl;
                    TokenType currentTokenType = ParseToken(currentToken);

                    tokensQueue.pop();

                    switch (currentTokenType)
                    {
                        case PUB_EXT:
                            if (currentSectionType != START)
                            {
                                throw runtime_error("Not allowed to write public/extern not in GLOBAL section!");
                            }
                            if (currentToken == ".public")
                                return AFTER_PUB;
                            return AFTER_EXT;
                        case DIRECTIVE:
                            if (currentSectionType != DATA)
                            {
                                throw runtime_error("Not allowed to write directives not in DATA section!");
                            }
                            HandleDirective(currentToken, tokensQueue, offsetCounter, currentSection, true);
                            return LINE_END;
                        case SECTION:
                            currentSection = currentToken;
                            UpdateCurrentSection(currentToken, currentSectionType, offsetCounter);
                            return LINE_END;
                        case INSTRUCTION:

                            HandleInstruction(currentToken, tokensQueue, offsetCounter);

//                            offsetCounter += 4;
//                            while (!tokensQueue.empty()) tokensQueue.pop();
                            return LINE_END;
                        default:
                            throw runtime_error("Token not allowed here! " + currentTokenType + (" " + currentToken));
                    }
                }
            },
            {
                LINE_END, [&](){

                    if (!tokensQueue.empty())
                    {
                        throw runtime_error("Expected end of line, but token found! " + tokensQueue.front());
                    }

                    return END;
                }
            },
            {
                AFTER_PUB, [&](){

                    if (tokensQueue.empty())
                        return END;

                    string currentToken = tokensQueue.front();
                    tokensQueue.pop();

                    TokenType currentTokenType = ParseToken(currentToken);
                    if (currentTokenType !=  SYMBOL)
                    {
                        throw runtime_error("Symbol expected, " + currentToken);
                    }

                    auto symbol = symbols.find(currentToken);
                    if (symbol == symbols.end())
                    {
                        throw runtime_error("Symbol defined public, but not found " + currentToken);
                    }

                    symbol->second.scope = ScopeType::GLOBAL;
                    //symbols[currentToken].scope = ScopeType::GLOBAL;

                    return AFTER_PUB;
                }
            },
            {
                AFTER_EXT, [&](){

                    if (tokensQueue.empty())
                        return END;

                    string currentToken = tokensQueue.front();
                    tokensQueue.pop();

                    TokenType currentTokenType = ParseToken(currentToken);
                    if (currentTokenType !=  SYMBOL)
                    {
                        throw runtime_error("Symbol expected, " + currentToken);
                    }

                    AddNewSymbol(currentToken, false, currentSectionType, currentSection, ScopeType::GLOBAL, offsetCounter);

                    return AFTER_EXT;
                }
            },
        };

    for (auto &tokens: assemblyInput)
    {
        for (auto &token: tokens)
        {
            tokensQueue.push(token);
        }

        currentState = LINE_BEGIN;

        while (currentState != END)
        {
            logFile << "current state " << currentState << endl;
            logFile << "current counter " << offsetCounter << endl;
            if (!tokensQueue.empty())
            {
                logFile << "current token type " << ParseToken(tokensQueue.front()) << endl;
                logFile << "current token " << tokensQueue.front() << endl;

            }
            else
            {
                logFile << "empty tokensqueue" << endl;
            }

            logFile << endl;

            logFile.flush();

            currentState = stateMachine[currentState]();
        }
    }

}


void Compiler::WriteObjectFile(ofstream& outputFile)
{
    for(auto iterator = symbols.begin(); iterator != symbols.end(); iterator++)
    {
        Symbol &symbol = (iterator->second);

        outputFile << "Symbol: " << symbol.name << endl;
        outputFile << "\tDefined:\t" << (symbol.defined?"true":"false") << endl;
        outputFile << "\tSection:\t" << symbol.section << endl;
        outputFile << "\tSectionName:\t" << symbol.sectionName << endl;
        outputFile << "\toffset:\t" << symbol.offset << endl;
        outputFile << "\tType:\t" << symbol.scope << endl;
    }

    for (int i = 0; i < instructions.size() ; i++)
    {
        outputFile << (instructions[i]);
    }

}




TokenType Compiler::ParseToken(string token)
{
    TokenType ret = ILLEGAL;

    if (regex_match(token, tokenParsers[SYMBOL]))
        ret = SYMBOL;

    for (auto &parseRule: tokenParsers)
    {
        if (parseRule.first == SYMBOL)
            continue;

        if (regex_match(token, parseRule.second))
        {
            if (ret == ILLEGAL || ret == SYMBOL)
            {
                ret = (TokenType)parseRule.first;
            }
            else
            {
                throw runtime_error("Ambigous token! " + token);
            }
        }
    }

    logFile << "token " << token << " parsed as " << ret << endl;

    if (ret == ILLEGAL)
        throw runtime_error("Illegal token! " + token);


    return ret;
}


void Compiler::UpdateCurrentSection(string sectionName, SectionType &currentSection, u_int32_t &offsetCounter)
{

    offsetCounter = 0;

    smatch base_match;

    regex_match(sectionName, base_match, tokenParsers[SECTION]);

    if (base_match[1] == "text")
        currentSection = TEXT;
    if (base_match[1] == "data")
        currentSection = DATA;
    if (base_match[1] == "bss")
        currentSection = BSS;
}



u_int32_t Compiler::ParseOperand(string token, int immSize)
{
    smatch base_match;
    bool isReg = false;
    u_int32_t ret;


    if (regex_match(token, base_match, tokenParsers[OPERAND_REG]))
    {
        isReg = true;
        stringstream ss;
        ss << base_match[1];
        ss >> ret;
    }

    else if (regex_match(token, base_match, tokenParsers[OPERAND_REGINCDEC]))
    {
        isReg = true;
        stringstream ss;
        ss << base_match[1];
        ss >> ret;
    }

    else if (regex_match(token, base_match, tokenParsers[OPERAND_REGSPEC]))
    {
        isReg = true;
        ret = specialRegisterValues[base_match[1]];
    }

    else if (regex_match(token, base_match, tokenParsers[OPERAND_REGSPECINCDEC]))
    {
        isReg = true;
        ret = specialRegisterValues[base_match[1]];
    }

    else if (regex_match(token, base_match, tokenParsers[OPERAND_HEX]))
    {
        stringstream ss;
        ss << base_match[1];
        ss >> hex >> ret;
    }

    else if (regex_match(token, base_match, tokenParsers[OPERAND_DEC]))
    {
        stringstream ss;
        ss << base_match[1];
        ss >> ret;
    }

    if (!isReg && immSize != 0)
    {
        ret <<= 32-immSize;
        int32_t temp = (int32_t)ret;
        temp >>= 32-immSize;
        ret = (u_int32_t)temp;
    }

    return ret;
}


void Compiler::HandleDirective(string directiveName, queue<string> &tokens, u_int32_t &locationCounter, string sectionName, bool writeToMemory)
{
    if (directiveName == ".align")
    {
        if (locationCounter/4*4 != locationCounter)
        {
            locationCounter = (locationCounter/4+1)*4;
        }

        return;
    }

    string nextToken = tokens.front();
    tokens.pop();
    TokenType nextTokenType = ParseToken(nextToken);

    if (nextTokenType != OPERAND_HEX && nextTokenType != OPERAND_DEC)
    {
        throw runtime_error("Bad token " + nextToken + " after directive " + directiveName);
    }

    u_int32_t operandVal = ParseOperand(nextToken);


    if (directiveName == ".skip")
    {
        locationCounter += operandVal;
        return;
    }


    void *binVal = nullptr;
    size_t numBytes = 0;

    if (directiveName == ".char")
    {
        binVal = new u_int8_t(operandVal);
        numBytes = 1;
    }

    if (directiveName == ".word")
    {
        binVal = new u_int16_t(operandVal);
        numBytes = 2;
    }

    if (directiveName == ".long")
    {
        binVal = new u_int32_t(operandVal);
        numBytes = 4;
    }

    if (writeToMemory)
    {
        memcpy(sections[sectionName] + locationCounter, binVal, numBytes);
    }

    locationCounter += numBytes;

    delete binVal;
}


void Compiler::GetOperand(queue<string> &tokens, string &token, u_int32_t &operand, TokenType &operandType, vector<TokenType> operandAllowed, int operandImmSize)
{
    if (tokens.empty())
    {
        throw runtime_error("Not enough operands !");
    }

    token = tokens.front();
    tokens.pop();
    operandType = ParseToken(token);

    if (find(operandAllowed.begin(), operandAllowed.end(), operandType) == operandAllowed.end())
    {
        throw runtime_error("Illegal token: " + token);
    }

    operand = ParseOperand(token, operandImmSize);
}




void Compiler::HandleInstruction(string instructionName, queue<string> &tokens, u_int32_t &locationCounter)
{
    smatch base_match;

    if (regex_match(instructionName, base_match, tokenParsers[INSTRUCTION])) {

        string shortInstructionName = base_match[1];
        string condition = base_match[2];
        bool setFlags = (base_match[3] == "s");

        Instruction instruction(shortInstructionName, condition, setFlags);

        if (instructionTypesMap.find(shortInstructionName) == instructionTypesMap.end())
        {
            throw runtime_error("Error, not valid instruction " + shortInstructionName + " " + instructionName);
        }

        InstructionType instructionType = (InstructionType) instructionTypesMap[shortInstructionName];

        instruction.instrCode.instruction.instr = instructionCodes[shortInstructionName];
        instruction.instrCode.instruction.cond = branchCodes[condition];
        instruction.instrCode.instruction.flag = (setFlags?1:0);

        instructionsHandlers[instructionType](instruction, tokens);

        instructions.push_back(instruction);

        locationCounter += 4;
    }
}


void Compiler::AddNewSymbol(string symName, bool symDefined, SectionType symSection, string symSectionName, ScopeType symScope, u_int32_t locationCounter)
{
    if (symbols.find(symName) != symbols.end())
    {
        throw runtime_error("Error, symbol already defined ! " + symName);
    }

    Symbol sym(symName, symDefined, symSection, symSectionName, symScope, locationCounter);

    symbols.insert({symName, sym});
}