/* This source file is part of koostosh's chb project
see README for copyright notice */

#ifndef SOCIALMODULE_H
#define SOCIALMODULE_H

#include "Module.h"

enum GuildEvents
{
    GE_PROMOTION        = 0x00,
    GE_DEMOTION         = 0x01,
    GE_MOTD             = 0x02,
    GE_JOINED           = 0x03,
    GE_LEFT             = 0x04,
    GE_REMOVED          = 0x05,
    GE_LEADER_IS        = 0x06,
    GE_LEADER_CHANGED   = 0x07,
    GE_DISBANDED        = 0x08,
    GE_TABARDCHANGE     = 0x09,
    GE_SIGNED_ON        = 0x0C,
    GE_SIGNED_OFF       = 0x0D,
    GE_BANKTAB_PURCHASED= 0x0F,
};

class SocialModule : public Module
{
public:
    void Handle(inc_pack* InPack);
    void Command(std::string cmd,std::string args);
private:
    void handle_smsg_contact_list(inc_pack* InPack);
    void handle_smsg_friend_status(inc_pack* InPack);
    void handle_smsg_guild_roster(inc_pack* InPack);
    void handle_smsg_guild_event(inc_pack* InPack);

    void send_cmsg_guild_roster();

    void cmd_friend(std::string args);
    void cmd_unfriend(std::string args);
    void cmd_ignore(std::string args);
    void cmd_unignore(std::string args);
};

#endif