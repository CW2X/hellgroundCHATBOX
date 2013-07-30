#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <stdarg.h>

inline std::string string_format(const std::string fmt, ...)
{
    int size = 100;
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

#endif
