//
// Created by rols on 6/8/16.
//

#include <cstring>

#include "Section.h"

#include <iostream>
#include <iomanip>

using namespace std;

Section::Section(string _name, int _size):
    size(_size),
    memory(new u_int8_t[size]),
    name(_name)
{

}

Section::~Section()
{
//    /delete memory;
}

void Section::Write(void *src, int pos, int length)
{
    memcpy(memory + pos, src, length);
}

void Section::WriteZeros(int pos, int length)
{
    memset(memory+pos, 0, length);
}

Section& Section::operator+=(Section& other)
{
    int newSize = size + other.size;

    u_int8_t *newMem = new u_int8_t[newSize];

    memcpy(newMem, memory, size);
    memcpy(newMem + size, other.memory, other.size);

    delete memory;
    delete other.memory;

    memory = newMem;
    size = newSize;
}


stringstream Section::Serialize()
{
    stringstream out;
    const int tokensByLine = 8;
//    out << "Section:" << endl ;
    out << name << " " << dec << size;
    for (int i = 0; i < size; i ++)
    {
        if (i%tokensByLine == 0) out << endl;
        out << setfill('0') << setw(2) << hex << (u_int32_t )memory[i] << " ";

    }

    out << endl;
    out << ".end" << endl << endl;

    return out;
}

Section Section::Deserialize(string instr)
{
    stringstream in;
    in << instr;

    string name;
    in >> name;
    int size;
    in >> size;

    Section sec(name, size);

    //TODO: really, temp ?
    string temptemp;
    u_int32_t temp;
    u_int8_t tempIn;

    for (int i = 0; i < size ; i ++)
    {
        in >> temptemp;

        stringstream ss;
        ss << "0x";
        ss << temptemp;

        ss >> hex >> temp;

        tempIn = (u_int8_t)temp;

        sec.Write(&tempIn, i, 1);
    }

    return sec;

}