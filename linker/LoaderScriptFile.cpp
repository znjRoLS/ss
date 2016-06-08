//
// Created by rols on 6/8/16.
//

#include "LoaderScriptFile.h"

#include <regex>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

regex section("^.(text|data|bss)(.[a-zA-Z_][a-zA-Z0-9]*)?$");
regex assignment("^([a-zA-Z_][a-zA-Z0-9]*)=([\\+-](0-9a-zA-Z\\(\\)))+$");

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

void LoaderScriptFile::FillSymbols(unordered_map<string,Symbol> &symbols)
{

    for (auto &line : scriptInput)
    {
        smatch base_match;
        if (regex_match(line, base_match, section))
        {

        }

        else if (regex_match(line, base_match, assignment))
        {
            u_int32_t expressionVal = 0;

            for (int i = 2; i < base_match.size() ; i ++)
            {
                cout << base_match[i] << endl;
            }

        }
    }

}



void AddSymbol(unordered_map<string, Symbol> symbols, Symbol& sym)
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
        }
    }
}
