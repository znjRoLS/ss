//
// Created by rols on 6/8/16.
//

#include "ObjectFile.h"

#include <fstream>

void ObjectFile::LoadFromFile(string inputFile)
{

    ifstream inFile(inputFile);

    if (!inFile.is_open())
    {
        cerr << "Error opening input file " << inputFile << endl;
    }

    string line;

    string sectionCumulate;

    bool symbolsSection = false;
    bool relocationSection = false;
    bool sectionsSection = false;

    while (getline(inFile, line))
    {
        if (!symbolsSection && !relocationSection && !sectionsSection)
        {
            if (line == "%SYMBOLS SECTION%")
            {
                symbolsSection = true;
                getline(inFile, line);
            }
            if (line == "%RELOCATIONS SECTION%")
            {
                relocationSection = true;
                getline(inFile, line);
            }
            if (line == "%SECTIONS SECTION%")
            {
                sectionsSection = true;
            }
            continue;
        }
        if (line == "%END%")
        {
            symbolsSection = relocationSection = sectionsSection = false;
            continue;
        }

        if (symbolsSection)
        {
            Symbol sym = Symbol::Deserialize(line);
            symbols.insert({sym.name, sym});
        }

        if (relocationSection)
        {
            Relocation rel = Relocation::Deserialize(line);
            relocations.push_back(rel);
        }

        if (sectionsSection)
        {
            if (line == ".end")
            {
                Section section = Section::Deserialize(sectionCumulate);
                sections.insert({section.name, section});

                sectionCumulate.clear();
            }

            else
            {
                sectionCumulate += line + "\n";
            }
        }
    }
}