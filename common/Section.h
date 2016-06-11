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

    void Write(void *src, int pos, size_t length);
    void WriteZeros(int pos, size_t length);

    Section& operator+=(Section&);

    string name;
    size_t size;
    u_int8_t *memory;

    friend ostream& operator<<(ostream&, Section&);

    string Serialize();

    static Section Deserialize(string);

};


#endif //SS_SECTION_H
