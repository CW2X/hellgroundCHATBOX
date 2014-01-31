#ifndef MODULE_H
#define MODULE_H

#include "base_defs.h"
#include "iopackets.h"

class Module
{
public:
    virtual void Handle(inc_pack* InPack) {};
    virtual void Command(std::string command,std::string args) {};
protected:
    out_pack OuPack;
    void send_out_pack();
    void print(std::string s);
    void command(std::string s);
};

#endif
