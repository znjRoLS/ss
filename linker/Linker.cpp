//
// Created by rols on 6/7/16.
//

#include "Linker.h"
#include "ObjectFile.h"

#include <exception>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <regex>

using namespace std;

ofstream Linker::logFile("linker/log");

regex sectionRegex("^.(text|data|bss)(.[a-zA-Z_][a-zA-Z0-9]*)?$");

void Linker::Link(ifstream &loaderScriptFile, vector<ifstream> &inputFiles, ofstream &outputFile)
{
    try {

        logFile << "Load script file" << endl;
        loaderScript.LoadFromFile(loaderScriptFile);


        for (auto &inputFile: inputFiles)
        {
            logFile << "Linking file " << endl;
            LoadFile(inputFile);
        }


        logFile << "Load symbols from script" << endl;
        loaderScript.FillSymbolsAndSectionPositions(symbols, sectionPositions);

        logFile << "Check output " << endl;
        FixRelocations();

        logFile << "Generating binary output" << endl;
        GenerateOutput();

        logFile << "Write output " << endl;
        WriteOutputFile(outputFile);
    }

    catch(exception &e)
    {
        cerr << e.what() << endl;
        logFile << "ERROR: " << e.what() << endl;
    }

}

void Linker::AddSymbol(Symbol& sym)
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

int Linker::AddSection(Symbol& sym, Section& section)
{
    auto symbolOld = symbols.find(sym.name);

    int offset;

    if (symbolOld == symbols.end())
    {
        symbols.insert({sym.name, sym});
        offset = 0;
    }
    else
    {
        offset = symbolOld->second.size;

        symbolOld->second.size += sym.size;

        sections.find(sym.name)->second += section;
    }

    return offset;
}

void Linker::LoadFile(ifstream &inputFile)
{

    ObjectFile objectFile;

    objectFile.LoadFromFile(inputFile);

    unordered_map<string, int> sectionOffsets;

//    for (auto &section: objectFile.sections)
//    {
//        sections.insert({section.first, section.second});
//    }

    for (auto &symbol: objectFile.symbols)
    {
        if (symbol.second.symbolType == TokenType::SECTION)
        {
            //TODO: kobasica
            auto section = objectFile.sections.find(symbol.second.name);
            sections.insert({section->first, section->second});

            logFile << "Adding section symbol " << symbol.second.name << endl;
            sectionOffsets[symbol.second.name] = AddSection(symbol.second, objectFile.sections.find(symbol.second.name)->second);
        }
    }

    for (auto &symbol: objectFile.symbols)
    {
        if (symbol.second.symbolType == TokenType::LABEL)
        {
            logFile << "Adding label symbol " << symbol.second.name << endl;
            symbol.second.offset += sectionOffsets[symbol.second.sectionName];
            AddSymbol(symbol.second);
        }
    }

    for (auto &relocation: objectFile.relocations)
    {
        relocations.push_back(relocation);
    }

}

void Linker::FixRelocations()
{
//    vector<Relocation> newRelocations;
//    for (auto &rel:relocations)
//    {
//        auto symbol = symbols.find(rel.symbolName);
//
//        if (symbol == symbols.end())
//        {
//            throw runtime_error("rand err");
//        }
//
//        if (symbol->second.defined)
//        {
//            u_int32_t symbolVal =
//
//            auto section = sections.find(rel.section);
//
//            if (section == sections.end())
//            {
//                throw runtime_error("random errr");
//            }
//
//            if (section->second.size <= rel.offset)
//            {
//                throw runtime_error("randddddom err");
//            }
//
//            section->second.Write(&rel.)
//        }
//    }
//
//    relocations = newRelocations;

}

void Linker::WriteOutputFile(ofstream &outputFile)
{
    for (auto &symbol :symbols)
    {
        logFile << symbol.second;
    }


    for (int i = 0; i < relocations.size() ; i++)
    {
        logFile << (relocations[i]);
    }

    for (auto &section: sections)
    {
        logFile << section.second;
    }


    outputFile << "%SYMBOLS SECTION%" << endl;

    outputFile << left << "  " <<
    setw(15) << "Symbol" <<
    setw(15) << "SymbolName" <<
    setw(15) << "Defined" <<
    setw(15) << "SectionName" <<
    setw(15) << "Offset" <<
    setw(15) << "Type" <<
    setw(15) << "Size" <<
    setw(15) << "SymbolType" <<
    endl;

    for (auto &symbol:symbols)
    {
        outputFile << symbol.second.Serialize().rdbuf();
    }

    outputFile << "%END%" << endl;

    outputFile << endl;

    outputFile << "%RELOCATIONS SECTION%" << endl;

    outputFile << left << "  " <<
    setw(15) << "Relocation" <<
    setw(15) << "SymbolName" <<
    setw(15) << "SectionName" <<
    setw(15) << "Offset" <<
    setw(15) << "Type" <<
    endl;

    for (auto &rel:relocations)
    {
        outputFile << rel.Serialize().rdbuf();
    }

    outputFile << "%END%" << endl;

    outputFile << endl << left;

    outputFile << "%SECTIONS SECTION%" << endl;

    for (auto &section:sections)
    {
        outputFile << sectionPositions[section.second.name] << " ";
        outputFile << section.second.Serialize().rdbuf();
    }

    outputFile << "%END%" << endl;


}


void Linker::GenerateOutput()
{

}