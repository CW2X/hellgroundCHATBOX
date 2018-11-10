/* This source file is part of koostosh's chb project
see README for copyright notice */

#include "SocialModule.h"
#include "..\Util.h"
#include "Database.h"


SocialModule::SocialModule( Database* database )
    : Module( database->GetCHBMain() )
    , m_database( database )
{

}

void SocialModule::Handle(inc_pack* InPack)
{
    switch(InPack->gc())
    {
    case 0x0067: handle_smsg_contact_list(InPack);break; //SMSG_CONTACT_LIST
    case 0x0068: handle_smsg_friend_status(InPack);break;//SMSG_FRIEND_STATUS
    case 0x008A: handle_smsg_guild_roster(InPack);break; //SMSG_GUILD_ROSTER
    case 0x0092: handle_smsg_guild_event(InPack);break;  //SMSG_GUILD_EVENT
    default:
        break;
    }
}

void SocialModule::Command(std::string cmd,std::string args)
{
    if (cmd == "loadguild")
        send_cmsg_guild_roster();
    else if (cmd == "friend" && !args.empty())
        cmd_friend(args);
    else if (cmd == "unfriend" && !args.empty())
        cmd_unfriend(args);
    else if (cmd == "ignore" && !args.empty())
        cmd_ignore(args);
    else if (cmd == "unignore" && !args.empty())
        cmd_unignore(args);
}

void SocialModule::handle_smsg_contact_list(inc_pack* InPack)
{
    m_database->FriendInfoMap.clear();
    uint32 size;
    uint32 guid;
    uint32 flag;
    std::string note;
    std::string name;
    uint8 status;

    InPack->skip(4);
    *InPack >> size;
    
    for(uint16 i=0; i<size; i++)
    {
        *InPack >> guid;
        InPack->skip(4);
        *InPack >> flag;
        *InPack >> note;
        if (flag == 0x01)
        {
            *InPack >> status;
            m_database->FriendInfoMap[guid].note = note;
            m_database->FriendInfoMap[guid].status = status;
            if (status)
                InPack->skip(12);
            name = m_database->Guid_to_name(guid,false);
            if (name[0] != '#')
                i_comm(string_format("%s%s\n",status ? "FrAN" : "FrAF",name.c_str()));
        };
    }
}

void SocialModule::handle_smsg_friend_status(inc_pack* InPack)
{
    uint8 result;
    uint8 status = 0;
    uint32 guid;
    std::string name;
    std::string note = "";

    *InPack >> result >> guid;
    InPack->skip(4);
    if (result == 6 || result == 7)
    {
        *InPack >> note;
        print("Friend added\r\n");
    }
    if (result == 2 || result == 6)
    {
        *InPack >> status;
        InPack->skip(12);
    }

    switch (result)
    {
    case 0:
        print("Friend database error\r\n");
        break;
    case 1:
        print("Friend list full\r\n");
        break;
    case 2:
    case 3:
    case 6:
    case 7:
        if (note != "")
            m_database->FriendInfoMap[guid].note = note;
        m_database->FriendInfoMap[guid].status = status;
        name = m_database->Guid_to_name(guid, false);
        if (name[0] != '#')
            i_comm(string_format("%s%s\n", status ? "FrAN" : "FrAF", name.c_str()));
        break;
    case 4:
        print("Friend not found\r\n");
        break;
    case 5:
        i_comm(string_format("FrR%s\n", m_database->PlayersInfoMap[guid].name.c_str()));
        m_database->FriendInfoMap.erase(guid);
        print("Friend removed\r\n");
        break;
    case 8:
        print("Already a friend\r\n");
        break;
    case 9:
        print("Cannot add yourself as friend\r\n");
        break;
    case 10:
        print("Cannot add enemy as a friend\r\n");
        break;
    case 11:
        print("Ignore list full\r\n");
        break;
    case 12:
        print("Cannot add yourself as ignore\r\n");
        break;
    case 13:
        print("Ignore target not found\r\n");
        break;
    case 14:
        print("Already ignored\r\n");
        break;
    case 15:
        print("Ignore added\r\n");
        break;
    case 16:
        print("Ignore removed\r\n");
        break;

    }

}

void SocialModule::handle_smsg_guild_roster(inc_pack* InPack)
{
    uint32 size;
    std::string Gmotd;
    std::string Ginfo;
    uint32 ranks;
    uint32 guid;
    uint8 online;
    std::string name;
    std::string spam;

    *InPack >> size >> Gmotd >> Ginfo >> ranks;
    InPack->skip(ranks * 56);

    for (uint16 i = 0 ; i < size; i++)
    {
        *InPack >> guid;
        InPack->skip(4);
        *InPack >> online;
        *InPack >> name;
        InPack->skip(11);
        if(!online)
            InPack->skip(4);
        *InPack >> spam >> spam;
        if(online)
        {
            i_comm(string_format("GuA%s\n",name.c_str()));
            m_database->OnlineGuildMembers.push_back(guid);
        }
        m_database->Guid_to_name(guid,false);
    }
}

void SocialModule::handle_smsg_guild_event(inc_pack* InPack)
{
    uint8 Event;
    *InPack >> Event;
    switch (Event)
    {
    case GE_MOTD:
        break;
    case GE_SIGNED_ON:
        {
            std::string name;
            uint32 guid;

            InPack->skip(1);
            *InPack >> name;
            *InPack >> guid;
            InPack->skip(4);
            
            m_database->OnlineGuildMembers.push_back(guid);
            i_comm(string_format("GuR%s\nGuA%s\n",name.c_str(),name.c_str()));
            print(string_format("[%s] has come online\r\n",name.c_str()));
            m_database->Guid_to_name(guid,false);
            break;
        }
    case GE_SIGNED_OFF:
        {
            std::string name;
            uint32 guid;

            InPack->skip(1);
            *InPack >> name;
            *InPack >> guid;
            InPack->skip(4);

            m_database->OnlineGuildMembers.remove(guid);
            i_comm(string_format("GuR%s\n",name.c_str()));
            print(string_format("[%s] has gone offline\r\n",name.c_str()));
            break;
        }
    default:
        print(string_format("Guild event %u\r\n",Event));
    }
}

void SocialModule::send_cmsg_guild_roster()
{
    OuPack.reset( 0x0089);
    send_out_pack();
}

void SocialModule::cmd_friend(std::string args)
{
    OuPack.reset(0x0069);
    OuPack << args;
    OuPack << uint8(0);
    send_out_pack();
}

void SocialModule::cmd_unfriend(std::string args)
{
    string_to_uppercase(args);
    std::string curentname;
    uint32 guid = 0;
    for (std::map<uint32, PlayerInfo>::iterator itr = m_database->PlayersInfoMap.begin(); itr != m_database->PlayersInfoMap.end(); itr++)
    {
        curentname = itr->second.name;
        string_to_uppercase(curentname);
        if (curentname == args)
            guid = itr->first;
    }

    if (guid == 0)
        return;

    OuPack.reset(0x006A);
    OuPack << guid << uint32(0);
    send_out_pack();
}

void SocialModule::cmd_ignore(std::string args)
{
    OuPack.reset(0x006C);
    OuPack << args;
    send_out_pack();
}

void SocialModule::cmd_unignore(std::string args)
{
    string_to_uppercase(args);
    std::string curentname;
    uint32 guid = 0;
    for (std::map<uint32, PlayerInfo>::iterator itr = m_database->PlayersInfoMap.begin(); itr != m_database->PlayersInfoMap.end(); itr++)
    {
        curentname = itr->second.name;
        string_to_uppercase(curentname);
        if (curentname == args)
            guid = itr->first;
    }

    if (guid == 0)
        return;

    OuPack.reset(0x006D);
    OuPack << guid << uint32(0);
    send_out_pack();
}
