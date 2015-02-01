/* This source file is part of koostosh's chb project
see README for copyright notice */

#ifndef MODULE_H
#define MODULE_H

#include "..\base_defs.h"
#include "..\iopackets.h"

class Module
{
public:
    virtual void Handle(inc_pack* InPack) {};
    virtual void Command(std::string cmd,std::string args) {};
protected:
    out_pack OuPack;
    void send_out_pack();
    void print(std::string s);
    void print(const char* c) {print(std::string(c));};
    void i_comm(std::string s);
};

#endif
