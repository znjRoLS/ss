//
// Created by rols on 6/7/16.
//

#ifndef SS_EMULATOR_H
#define SS_EMULATOR_H

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Emulator {
public:
    void Emulate(ifstream &inputFiles);

private:

    static ofstream logFile;
};


#endif //SS_EMULATOR_H
