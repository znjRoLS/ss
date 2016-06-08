//
// Created by rols on 6/8/16.
//

#ifndef SS_LOADERSCRIPTFILE_H
#define SS_LOADERSCRIPTFILE_H

#include "Symbol.h"
#include "Section.h"

#include <iostream>
#include <vector>

using namespace std;

class LoaderScriptFile {
public:

    void LoadFromFile(istream&);

    void FillSymbolsAndSectionPositions(unordered_map<string,Symbol> &, unordered_map<string, int> &);

    //unordered_map<string, Symbol> symbols;

    vector<string> scriptInput;
};


#endif //SS_LOADERSCRIPTFILE_H
