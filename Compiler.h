//
// Created by rols on 6/2/16.
//

#ifndef SS_COMPILER_H
#define SS_COMPILER_H

#include "SectionType.h"

#include <iostream>
#include <fstream>

using namespace std;


class Compiler
{
public:

    Compiler();
    ~Compiler();

    void Compile(ifstream& inputFile);
    void WriteObjectFile(ofstream& outputFile);

private:

};

#endif //SS_COMPILER_H
