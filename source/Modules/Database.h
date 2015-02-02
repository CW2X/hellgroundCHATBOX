/* This source file is part of koostosh's chb project
see README for copyright notice */

#ifndef DATABASE_H
#define DATABASE_H

#include "Module.h"
#include <map>
#include <list>

struct PlayerInfo
{
    std::string name;
    uint32      clas;
    uint32      race;
    uint32      gender;
};

struct FriendInfo
{
    std::string note;
    uint8       status;
};

class Database : public Module
{
public:
    void Handle(inc_pack* InPack);
    void Command(std::string cmd,std::string args) {};

    static Database* getInstance()
    {
        static Database instance;
        return &instance;
    }

    std::string Guid_to_name(uint32 guid,bool info);
    void handle_smsg_name_query_response(inc_pack* InPack);

    std::map<uint32,PlayerInfo> PlayersInfoMap;
    std::list<uint32>           RequestedPlayers;
    std::map<uint32,FriendInfo> FriendInfoMap;
    std::list<uint32>           OnlineGuildMembers;
    bool ishordeplayer;
private:
    Database() {};
    void send_cmsg_name_query(uint32 guid);
};
#define sDB Database::getInstance()

#endif