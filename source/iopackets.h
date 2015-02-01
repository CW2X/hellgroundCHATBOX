/* This source file is part of koostosh's chb project
see README for copyright notice */

#ifndef IN_OUT_PACKETS
#define IN_OUT_PACKETS

#include "base_defs.h"

class inc_pack
{
public:
    void reset()
    {
        size = 0;
        cmd  = 0;
        pos  = 0;
    }

    void reset(uint16 s, uint16 c)
    {
        size = s;
        cmd  = c;
        pos  = 0;
        return;
    }

    void set(char* buff,uint16 offset = 0)
    {
        for(uint16 k=0;(k+offset<size) && (k+offset<BUFFER_SIZE_IN);k++)
            data[k+offset]=(uint8)buff[k];
    }

    void skip(uint16 i)
    {
        pos +=i; return;
    }

    inc_pack &operator >> (uint8 &u)
    {
        u = data[pos++];
        return *this;
    }

    inc_pack &operator >> (uint16 &u)
    {
        u = (uint16)(((uint8)data[pos] << 8) | (uint8)data[pos+1]);
        pos +=2;
        return *this;
    }

    inc_pack &operator >> (uint32 &u)
    {
        u = uint32(((uint8)data[pos]) | ((uint8)data[pos+1] << 8) | ((uint8)data[pos+2] << 16) | ((uint8)data[pos+3] << 24));
        pos +=4;
        return *this;
    }

    inc_pack &operator >> (std::string &str)
    {
        str = "";
        while(data[pos])
            str.append(1,(char)data[pos++]);
        pos++;
        return *this;
    }

    uint16 gs() {return size;}
    uint16 gc() {return cmd;}
private:
    uint16  size;
    uint16  cmd;
    uint32  pos;
    uint8   data[BUFFER_SIZE_IN];
};

class out_pack
{
public:
    out_pack()
    {   reset();    }

    void reset(uint32 c = 0)
    {
        cmd  = c;
        pos  = 0;
    }

    out_pack &operator <<(uint8 u)
    {
        data[pos++] = u;
        return *this;
    }

    out_pack &operator <<(uint16 u)
    {
        *this << (uint8)(u & 0x00FF);
        *this << (uint8)((u & 0xFF00) >>8);
        return *this;
    }

    out_pack &operator <<(uint32 u)
    {
        *this << (uint16)(u & 0x0000FFFF);
        *this << (uint16)((u & 0xFFFF0000) >>16);
        return *this;
    }

    out_pack &operator <<(std::string str)
    {
        for(uint8 i=0;i<str.size();i++)
            *this << (uint8)str.c_str()[i];
        *this << (uint8)0;
        return *this;
    }

    uint32 gc() {return cmd;}
    uint16 gs() {return pos;}
    uint8 gd(uint16 i) {return i< pos ?data[i] : 0;};
private:
    uint16  pos;
    uint32  cmd;
    uint8   data[BUFFER_SIZE_OUT];
};

#endif