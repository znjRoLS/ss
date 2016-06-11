//
// Created by rols on 6/7/16.
//

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include "Linker.h"

using namespace std;

int main(int argc, char ** argv) {

    if (argc < 4)
    {
        cout << "Please call this program as ./linker scriptfile outputfile [inputfiles]+" << endl;
        return 1;
    }

    ifstream loaderFile(argv[1]);
    ofstream outputFile(argv[2]);

    unique_ptr<Linker> linker(new Linker());

    if (!loaderFile.is_open())
    {
        cerr << "Error opening loader file" << endl;
        return 1;
    }

    if (!outputFile.is_open())
    {
        cerr << "Error opening output file" << endl;
        return 1;
    }

    vector<string> inputFiles;

    for (int i = 3; i < argc; i ++)
    {
        inputFiles.push_back(argv[i]);
    }

    bool err = false;
    for (auto &inputFile: inputFiles)
    {
//        if (!inputFile.is_open())
//        {
//            err = true;
//            cerr << "Error opening input file " << endl;
//        }
    }

//    if (err) return 1;

    linker->Link(loaderFile, inputFiles, outputFile);

    return 0;
}