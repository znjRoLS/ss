//
// Created by rols on 6/12/16.
//

#include <cstring>


#include "Loader.h"
#include "Emulator.h"

Loader::Loader():
    programSection("", 0)
{ }

void Loader::Load(ifstream &inputFile)
{
    //TODO, separate method to single responsibility methods
    string line;
    string sectionCumulate;

    bool parsedOutputSection = false;
    bool outputSection = false;
    bool relocationSection = false;
    bool mainOutputPassed = false;

    while (getline(inputFile, line))
    {
        if (!outputSection && !relocationSection)
        {
            if (line == "%OUTPUT SECTION%")
            {
                outputSection = true;
                parsedOutputSection = true;
            }

            if (line == "%RELOCATIONS SECTION%")
            {
                relocationSection = true;
                //TODO, please fix this everywhere (change output format)
                getline(inputFile, line);
            }
            continue;
        }
        if (line == "%END%")
        {
            outputSection = relocationSection = false;

            sectionCumulate.clear();
            continue;
        }

        if (relocationSection)
        {
            Relocation rel = Relocation::Deserialize(line);
            relocations.push_back(rel);
        }

        if (outputSection && !mainOutputPassed)
        {
            mainOutputPassed = true;
            stringstream ss;
            ss << line;
            string token;
            ss >> token;
            ss >> startPoint;

            startPoint += CODE_START;
            Emulator::logFile << "Set starting point " << hex << startPoint << endl;

            continue;
        }

        if (line == ".end")
        {
            programSection = Section::Deserialize(sectionCumulate);

            sectionCumulate.clear();

            Emulator::logFile << "Loaded program section" <<  endl;
        }

        else
        {
            sectionCumulate += line + "\n";
        }
    }

    if (!parsedOutputSection)
    {
        throw runtime_error("No code to emulate !");
    }

    Emulator::logFile << "Fix relocations, offseted it" << endl;
    //fix relocations
    for (auto &rel:relocations)
    {
        //TODO: maybe you should create hashmap of lambda fucntions for every use of if elseif elseif...
        // but maybe it will be ugly... should try it nevertheless
        if (rel.relocationType == LDCRELOC)
        {
            u_int16_t high;
            u_int16_t low;
            u_int16_t codeStartHigh = CODE_START >> 16;
            u_int16_t codeStartLow = (CODE_START << 16) >>16;

            //TODO: read method, not seljacki like this. also, few lines down, same mistake
            memcpy(&high, programSection.memory + rel.offset + 2, 2);
            high += codeStartHigh;

            memcpy(&low, programSection.memory + rel.offset + 6, 2);
            low += codeStartLow;

            programSection.Write(&high, rel.offset + 2 , 2);
            programSection.Write(&low, rel.offset + 6, 2);
        }

        //TODO: LONGRELOC
        if (rel.relocationType == LONG)
        {
            u_int32_t longVal;

            memcpy(&longVal, programSection.memory + rel.offset, 4);
            longVal += CODE_START;

            programSection.Write(&longVal, rel.offset, 4);
        }
    }

    Emulator::logFile << "Program section now looks: " << endl;
    Emulator::logFile << programSection;

    Emulator::logFile << "Write programSection to memory" << endl;
    memcpy(memory + CODE_START, programSection.memory, programSection.size);

    Emulator::logFile << "Fill ivt table with dummy functions(IRET)" << endl;

    for (int i = 0 ; i < 16 ; i ++)
    {
        u_int32_t ivtPoint = IVT_START + i*IVT_SIZE;
        Emulator::logFile << "Copied code to " << hex <<4*i << " " << ivtPoint << endl;
        memcpy(memory + 4*i, &ivtPoint, 4);

        Emulator::logFile << "Copied code to " << hex << ivtPoint << " " << IRET_CODE << endl;
        memcpy(memory + ivtPoint, &IRET_CODE, 4);
    }


}


void Loader::LoadDefaultIVT(ifstream &inputFile)
{

    Emulator::logFile << "LoadDefaultIVT function" << endl;

    if (inputFile.is_open()) {
        string line;
        string sectionCumulate;
        bool outputSection = false;
        Section currentSection("random", 0);

        while (getline(inputFile, line)) {
            if (!outputSection) {
                if (line == "%SECTIONS SECTION%") {
                    outputSection = true;
                }
                continue;
            }
            if (line == "%END%") {
                outputSection = false;
                continue;
            }

            if (line == ".end") {
                currentSection = Section::Deserialize(sectionCumulate);

                sectionCumulate.clear();

                if (currentSection.name == ".text.start") {
                    //Emulator::logFile << "Copied code to " << IVT_START <<" " << (int)memory[64] <<  endl;
                    memcpy(memory + IVT_START, currentSection.memory, currentSection.size);
                    Emulator::logFile << "yay, loaded start ivt" << endl;
                }
                else if (currentSection.name == ".text.timer") {
                    //Emulator::logFile << "Copied code to " << IVT_START + 1*IVT_SIZE <<" " << (int)memory[64] <<  endl;
                    memcpy(memory + IVT_START + 1*IVT_SIZE, currentSection.memory, currentSection.size);
                    Emulator::logFile << "yay, loaded timer ivt" << endl;
                }
                else if (currentSection.name == ".text.illegal") {
                    //Emulator::logFile << "Copied code to " << IVT_START + 2*IVT_SIZE <<" " << (int)memory[64] <<  endl;
                    memcpy(memory + IVT_START + 2*IVT_SIZE, currentSection.memory, currentSection.size);
                    Emulator::logFile << "yay, loaded illegal ivt" << endl;
                }
                else if (currentSection.name == ".text.keyboard") {
                    //Emulator::logFile << "Copied code to " << IVT_START + 3*IVT_SIZE <<" " << (int)memory[64] <<  endl;
                    memcpy(memory + IVT_START + 3*IVT_SIZE, currentSection.memory, currentSection.size);
                    Emulator::logFile << "yay, loaded keyboard ivt" << endl;
                }

            }

            else {
                sectionCumulate += line + "\n";
            }
        }
    }
}