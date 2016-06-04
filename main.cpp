#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <exception>

#include "Compiler.h"

using namespace std;

int main(int argc, char ** argv) {

    if (argc < 3)
    {
        cout << "Please call this program as ./program inputfile outputfile" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    ofstream outputFile(argv[2]);

    unique_ptr<Compiler> compiler(new Compiler);

    if (!inputFile.is_open())
    {
        cout << "Error openning input file" << endl;
        return 1;
    }

    try {
        compiler->Compile(inputFile);

        compiler->WriteObjectFile(outputFile);
    }
    catch( exception& e)
    {
        cout << e.what() << endl;
    }






    return 0;
}