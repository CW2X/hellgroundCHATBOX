/* This source file is part of koostosh's chb project
see README for copyright notice */

#include "Util.h"
#include <windows.h>
#include <time.h>

std::string string_format(const std::string fmt, ...)
{
    int size = 300; //max message length is 255, leave some space
    std::string str;
    va_list ap;
    while (1)
    {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf_s((char *)str.c_str(), size, size, fmt.c_str(), ap);
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

void convert_codepages(std::string* s, bool r)
{
    int size = MultiByteToWideChar((r ? CP_ACP : CP_UTF8), 0, s->c_str(), s->size(), NULL, 0);
    std::wstring wstr(size, 0);
    MultiByteToWideChar((r ? CP_ACP : CP_UTF8), 0, s->c_str(), s->size(), &wstr[0], size);
    size = WideCharToMultiByte((r ? CP_UTF8 : CP_ACP), 0, &wstr[0], wstr.size(), NULL, 0, NULL, NULL);
    std::string out(size, 0);
    WideCharToMultiByte((r ? CP_UTF8 : CP_ACP), 0, &wstr[0], wstr.size(), &out[0], size, NULL, NULL);
    *s = out;
}

uint32 getMsTime()
{
    SYSTEMTIME stm;
    GetSystemTime(&stm);
    return (stm.wMinute * 60 * 1000 + stm.wSecond * 1000 + stm.wMilliseconds);
}
