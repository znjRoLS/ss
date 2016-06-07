//
// Created by rols on 6/7/16.
//

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "Emulator.h"

using namespace std;

int main(int argc, char ** argv) {

    if (argc < 2)
    {
        cout << "Please call this program as ./emulator inputfile" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);


    unique_ptr<Emulator> emulator(new Emulator());

    if (!inputFile.is_open())
    {
        cerr << "Error openning input file" << endl;
        return 1;
    }


    emulator->Emulate(inputFile);


    return 0;
}