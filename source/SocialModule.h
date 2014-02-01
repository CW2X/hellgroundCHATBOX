#ifndef SOCIALMODULE_H
#define SOCIALMODULE_H

#include "Module.h"

class SocialModule : public Module
{
public:
    void Handle(inc_pack* InPack);
    void Command(std::string cmd,std::string args);
private:
    void handle_smsg_contact_list(inc_pack* InPack);
    void handle_smsg_friend_status(inc_pack* InPack);
    void handle_smsg_guild_roster(inc_pack* InPack);

    void send_cmsg_guild_roster();

};

#endif