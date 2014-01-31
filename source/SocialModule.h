#ifndef SOCIALMODULE_H
#define SOCIALMODULE_H

#include "Module.h"

class SocialModule : public Module
{
public:
    void Handle(inc_pack* InPack);
    void Command(std::string command,std::string args) {};
private:
    void handle_smsg_contact_list(inc_pack* InPack);
};

#endif