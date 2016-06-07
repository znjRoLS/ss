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

    if (argc < 3)
    {
        cout << "Please call this program as ./linker outputfile [inputfiles]+" << endl;
        return 1;
    }

    ofstream outputFile(argv[1]);

    unique_ptr<Linker> linker(new Linker());

    if (!outputFile.is_open())
    {
        cerr << "Error openning output file" << endl;
        return 1;
    }

    vector<ifstream> inputFiles;

    for (int i = 2; i < argc; i ++)
    {
        inputFiles.push_back(ifstream(argv[i]));
    }

    bool err = false;
    for (auto &inputFile: inputFiles)
    {
        if (!inputFile.is_open())
        {
            err = true;
            cerr << "Error openning input file " << endl;
        }
    }

    if (err) return 1;

    linker->Link(inputFiles, outputFile);

    return 0;
}