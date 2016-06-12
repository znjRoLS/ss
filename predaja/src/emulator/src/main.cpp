//
// Created by rols on 6/7/16.
//

//
// Created by rols on 6/7/16.
//

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>


#include "Emulator.h"
#include "Linker.h"

using namespace std;

int main(int argc, char ** argv) {

    if (argc < 2)
    {
        cout << "Please call this program as ./emulator scriptfile [inputfiles]+" << endl;
        return 1;
    }

    string firstArg = argv[2];

    size_t lastindex = firstArg.find_last_of(".");
    string outputFileName = firstArg.substr(0, lastindex) + ".output";

    {
        ifstream loaderFile(argv[1]);

        ofstream outputFile(outputFileName);

        unique_ptr<Linker> linker(new Linker());

        if (!loaderFile.is_open()) {
            cerr << "Error opening loader file" << endl;
            return 1;
        }

        if (!outputFile.is_open()) {
            cerr << "Error opening output file" << endl;
            return 1;
        }

        vector<string> inputFiles;

        for (int i = 2; i < argc; i++) {
            inputFiles.push_back(argv[i]);
        }


        linker->Link(loaderFile, inputFiles, outputFile);
    }

    {
        ifstream inputFile(outputFileName);


        unique_ptr<Emulator> emulator(new Emulator());

        if (!inputFile.is_open())
        {
            cerr << "Error opening input file" << endl;
            return 1;
        }

        emulator->Emulate(inputFile);
    }

    return 0;
}