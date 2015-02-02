/* This source file is part of koostosh's chb project
see README for copyright notice */

#ifndef CHATMODULE_H
#define CHATMODULE_H

#include "Module.h"

struct ChatMessage
{
    uint8       type;
    uint32      lang;
    uint32      guid;
    uint8       tag;
    uint32      length;
    std::string channel;
    std::string what;
    std::string who;
};

static char* ChatTagIdentifiers[]   = {"    ","AFK|","DND|","DND|","[GM]","[GM]","[GM]","[GM]"};

class ChatModule : public Module
{
public:
    ChatModule();
    void Handle(inc_pack* InPack);
    void Command(std::string cmd,std::string args);

private:
    void handle_smsg_messagechat(inc_pack* InPack);
    void handle_smsg_channel_notify(inc_pack* InPack);
    void handle_smsg_channel_list(inc_pack* InPack);
    void handle_smsg_notification(inc_pack* InPack);
    void handle_smsg_server_message(inc_pack* InPack);
    void handle_smsg_chat_player_not_found(inc_pack* InPack);
    void handle_smsg_userlist_add(inc_pack*);

    void send_cmsg_join_channel(std::string name);
    void send_cmsg_leave_channel(uint8 no);
    void send_cmsg_messagechat(std::string data);
    void send_cmsg_channel_list(std::string channelname);

    char* ChatLanguages(uint32 lang);

    std::string     channels[9];
    bool            channelson[9];
    uint8           activechannel;
    std::string     whisptarget;
    uint32          lang;
};

#endif