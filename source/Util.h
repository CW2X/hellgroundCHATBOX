#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include "base_defs.h"

inline void string_to_uppercase(std::string& str)
{
    for (uint8 i=0;i<str.size();i++)
        if (str.c_str()[i] >= 'a' && str.c_str()[i] <= 'z')
            str.replace(i,1,1,str.c_str()[i]-32);
}

inline std::string utostr(uint32 u)
{
    char buf[16];
    _itoa_s(u,buf,16,10);
    return std::string(buf);
}

void convert_codepages(std::string*, bool);
std::string string_format(const std::string fmt, ...);
uint32 getMsTime();
#endif
