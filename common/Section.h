//
// Created by rols on 6/8/16.
//

#ifndef SS_SECTION_H
#define SS_SECTION_H

#include <iostream>
#include <sstream>

using namespace std;

class Section {

public:
    Section (string, int);
    ~Section();

    void Write(void *src, int pos, int length);
    void WriteZeros(int pos, int length);

    string name;
    int size;
    u_int8_t *memory;

    friend ostream& operator<<(ostream&, Section&);

    stringstream Serialize();

    static Section Deserialize(string);

};


#endif //SS_SECTION_H
