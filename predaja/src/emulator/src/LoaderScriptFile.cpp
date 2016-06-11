//
// Created by rols on 6/8/16.
//

#include "LoaderScriptFile.h"
#include "Enums.h"

#include <regex>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

regex section("^.(text|data|bss)(.[a-zA-Z_][a-zA-Z0-9]*)?$");
string allowedChars = "[_0-9a-zA-Z\\(\\)\\.,]";
regex assignment("^(" + allowedChars + "+)=(-?" + allowedChars + "+)((?:[\\+-]" + allowedChars + "+)*)$");

extern ofstream logFile;

void LoaderScriptFile::LoadFromFile(istream &inFile)
{
    string line;

    while(getline(inFile, line))
    {
        //strip whitespace
        line.erase(remove_if(line.begin(), line.end(), [](char x){return std::isspace(x);}), line.end());

        if (!line.empty())
            scriptInput.push_back(line);
    }
}


enum ScriptOperandType {DOT, DEC, HEX, SCRIPTLABEL, ALIGN};

unordered_map<int, regex> tokenParsers =
    {
        {DOT, regex("^\\.$")},
        {DEC, regex("^([0-9]+)$")},
        {HEX, regex("^(0x[0-9abcdef]+)$")},
        {SCRIPTLABEL, regex("^([_a-zA-Z][a-zA-Z0-9]*)$")},
        {ALIGN, regex("^align\\(([_a-zA-Z][a-zA-Z0-9]*|\\.),([0-9]+|0x[0-9abcdef]+)\\)$")},
    };



ScriptOperandType GetType(string token)
{
    bool defined = false;
    ScriptOperandType ret;

    for (auto &parseRule: tokenParsers)
    {
        if (parseRule.first == SYMBOL)
            continue;

        if (regex_match(token, parseRule.second))
        {
            if (!defined)
            {
                defined = true;
                ret = (ScriptOperandType)parseRule.first;
            }
            else
            {
                throw runtime_error("Ambigous token! " + token);
            }
        }
    }

    if (!defined)
        throw runtime_error("Illegal token! " + token);

    return ret;
}

u_int32_t GetLabelVal(string label, unordered_map<string, Symbol> &symbols, unordered_map<string, int> &sectionPositions )
{
    u_int32_t ret = 0;

    auto symbol = symbols.find(label);
    if (symbol == symbols.end())
    {
        throw runtime_error("Symbol not found " + label);
    }

    if (symbol->second.symbolType == TokenType::LABEL || symbol->second.symbolType == TokenType::OPERAND_DEC)
    {
        ret += symbol->second.offset;
    }

    if (symbol->second.symbolType == TokenType::LABEL || symbol->second.symbolType == TokenType::SECTION)
    {
        string sectionName = symbol->second.sectionName;
        auto sectionPos = sectionPositions.find(sectionName);
        if (sectionPos == sectionPositions.end())
        {
            throw runtime_error("Not allowed, recursive declaration (section position of label not yet calculated) " + sectionName);
        }

        ret += sectionPos->second;
    }

    return ret;
}

u_int32_t ParseOperand(string token, int dotVal, unordered_map<string, Symbol> &symbols, unordered_map<string, int> &sectionPositions)
{
    smatch base_match;
    u_int32_t ret;

    if (regex_match(token, base_match, tokenParsers[DEC]))
    {
        stringstream ss;
        ss << base_match[1];
        ss >> ret;
    }

    else if (regex_match(token, base_match, tokenParsers[HEX]))
    {
        stringstream ss;
        ss << base_match[1];
        ss >> hex >> ret;
    }

    else if (regex_match(token, base_match, tokenParsers[DOT]))
    {
        ret = dotVal;
    }

    else if (regex_match(token, base_match, tokenParsers[SCRIPTLABEL]))
    {
        //cout << base_match[1] << endl;
        //cout << base_match[0] << endl;
        ret = GetLabelVal(base_match[1], symbols, sectionPositions);
    }

    else if (regex_match(token, base_match, tokenParsers[ALIGN]))
    {

        u_int32_t first = ParseOperand(base_match[1], dotVal, symbols, sectionPositions);
        u_int32_t second = ParseOperand(base_match[2], dotVal, symbols, sectionPositions);

        if (first != first/4*4)
        {
            first = (first/4+1)*4;
        }

        ret = first;
    }

    return ret;
}

void AddSymbol(unordered_map<string, Symbol> &symbols, Symbol& sym)
{
    auto symbolOld = symbols.find(sym.name);

    if (symbolOld == symbols.end())
    {
        symbols.insert({sym.name, sym});
    }
    else if (sym.defined)
    {
        if (symbolOld->second.defined) {
            throw runtime_error("Defined symbol twice ! " + symbolOld->second.name);
        }
        else {
            symbolOld->second.defined = true;
            symbolOld->second.offset = sym.offset;
            symbolOld->second.sectionName = sym.sectionName;
            symbolOld->second.scope = ScopeType::GLOBAL;
            symbolOld->second.symbolType = sym.symbolType;
        }
    }
}


void splitExpression(const string &s, vector<pair<string, bool> > & v){
    // to avoid modifying original string
    // first duplicate the original string and return a char pointer then free the memory
    static regex r("^([\\+-])(" + allowedChars + "+).*");

    string processMe = s;
    smatch base_match;

    while(regex_match(processMe, base_match, r))
    {
        //cout << "found " << base_match[0] << " " << base_match[1] << " " << base_match[2] << endl;
        v.push_back({base_match[2], base_match[1] == "-"});

        size_t slice = processMe.find_first_of("+-", 1);

        if (slice == string::npos)
        {
            return;
        }

        processMe = processMe.substr(slice);
    }

    throw runtime_error("Irregular expression !");
}


void LoaderScriptFile::FillSymbolsAndSectionPositions(unordered_map<string,Symbol> &symbols, unordered_map<string,int> &sectionPositions)
{
    u_int32_t locationCounter = 0;

//    /unordered_map<string, int> sectionPositions;

    for (auto &line : scriptInput)
    {
        //cout << line << endl;
        smatch base_match;
        if (regex_match(line, base_match, section))
        {
            //TODO: check if section exists
            sectionPositions[symbols.find(line)->second.name] = locationCounter;
            locationCounter += symbols.find(line)->second.size;
        }

        else if (regex_match(line, base_match, assignment))
        {
            //TODO: what have youuuu doneeeeeeeee
//            cout << "Whaaa" << endl;
//            cout << "left val " << base_match[1] << endl;
//            cout << "right val first " << base_match[2] << endl;
//            cout << "right val rest " << base_match[3] << endl;
            u_int32_t expressionVal = 0;

            ScriptOperandType tokenType1 = GetType(base_match[1]);

            if (tokenType1 != DOT && tokenType1 != SCRIPTLABEL)
            {
                throw runtime_error("Bad left operand ! " + base_match[1].str());
            }

            string firstRightOperand = base_match[2];

            bool negative = false;

            if (firstRightOperand[0] == '-')
            {
                firstRightOperand = firstRightOperand.substr(1);
                negative = true;
            }

            int firstRightOperandVal = ParseOperand(firstRightOperand, locationCounter, symbols, sectionPositions);

            expressionVal += (negative?-1:1) * firstRightOperandVal;

            if (!base_match[3].str().empty())
            {
                vector<pair<string, bool > > expressionTokens;
                splitExpression(base_match[3].str(), expressionTokens);

                for (auto &expr: expressionTokens)
                {
                    expressionVal += (expr.second?-1:1) * ParseOperand(expr.first, locationCounter, symbols, sectionPositions);
                }

            }

            if (tokenType1 == DOT)
            {
                if (locationCounter > expressionVal)
                {
                    throw runtime_error("Cannot assing smaller value to current address");
                }
                locationCounter = expressionVal;
            }

            else if (tokenType1 == SCRIPTLABEL)
            {
                Symbol symbol(base_match[1].str(), true, "script", ScopeType ::GLOBAL, expressionVal, TokenType::OPERAND_DEC, 0);
                AddSymbol(symbols, symbol);
            }

        }
    }

}




