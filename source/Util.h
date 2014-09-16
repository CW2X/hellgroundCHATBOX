#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <stdarg.h>

inline std::string string_format(const std::string fmt, ...)
{
    int size = 300; //max message length is 255, leave some space
    std::string str;
    va_list ap;
    while (1) 
   {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf_s((char *)str.c_str(),size, size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size)
      {
            str.resize(n);
            return str;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    }
    return str;
}

inline void string_to_uppercase(std::string& str)
{
    for (uint8 i=0;i<str.size();i++)
        if (str.c_str()[i] >= 'a' && str.c_str()[i] <= 'z')
            str.replace(i,1,1,str.c_str()[i]-32);
}

inline std::string utostr(uint32 u)
{
    char buf[10];
    _itoa_s(u,buf,10,10);
    return std::string(buf);
}
#endif
