//
// Created by rols on 6/8/16.
//

#include <cstring>

#include "Section.h"

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