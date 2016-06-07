//
// Created by rols on 6/7/16.
//

#ifndef SS_LINKER_H
#define SS_LINKER_H

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Linker {

public:
    void Link(vector<ifstream> &inputFiles, ofstream &outputFile);

private:
    void LinkFile(ifstream &inputFile);

    void CheckOutputFile();

    void WriteOutputFile(ofstream &outputFile);

    static ofstream logFile;

};


#endif //SS_LINKER_H
