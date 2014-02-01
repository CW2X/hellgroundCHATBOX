#include "Database.h"
#include "Util.h"

void Database::Handle(inc_pack* InPack)
{
    switch(InPack->gc())
    {
    case 0x0051: handle_smsg_name_query_response(InPack); break; //SMSG_NAME_QUERY_RESPONSE
    default:
        break;
    }
}

std::string Database::Guid_to_name(uint32 guid,bool info)
{
    std::string name;
    std::map<uint32,PlayerInfo>::const_iterator itr = PlayersInfoMap.find(guid);
    if (itr != PlayersInfoMap.end())
        name = itr->second.name;
    else if (guid !=0)
    {
        name = "#" + utostr(guid);
        send_cmsg_name_query(guid);
        if (info)
            RequestedPlayers.push_back(guid);
    }
    return name;
}

void Database::send_cmsg_name_query(uint32 guid)
{
    OuPack.reset( 0x0050);
    OuPack << guid << (uint32)0;
    send_out_pack();
}

void Database::handle_smsg_name_query_response(inc_pack *InPack)
{
    uint32 guid;
    *InPack >> guid;
    InPack->skip(4);
    *InPack >> PlayersInfoMap[guid].name;
    InPack->skip(1);
    *InPack >> PlayersInfoMap[guid].race;
    *InPack >> PlayersInfoMap[guid].gender;
    *InPack >> PlayersInfoMap[guid].clas;
    
    for (std::list<uint32>::const_iterator itr = RequestedPlayers.begin(); itr !=RequestedPlayers.end();itr++)
    {
        if(*itr == guid)
        {
            Module::print(string_format("player guid %u is %s\r\n",guid,PlayersInfoMap[guid].name.c_str()));
            RequestedPlayers.remove(guid);
            break;
        }
    }

    std::map<uint32,FriendInfo>::const_iterator itr = FriendInfoMap.find(guid);
    if (itr != FriendInfoMap.end())
    {
        command(string_format("%s%s\n",FriendInfoMap[guid].status ? "FrAN" : "FrAF",PlayersInfoMap[guid].name.c_str()));
    }
}
