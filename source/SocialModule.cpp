#include "SocialModule.h"
#include "Util.h"
#include "Database.h"

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
    if(cmd == "loadguild")
        send_cmsg_guild_roster();
}

void SocialModule::handle_smsg_contact_list(inc_pack* InPack)
{
    sDB->FriendInfoMap.clear();
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
            sDB->FriendInfoMap[guid].note = note;
            sDB->FriendInfoMap[guid].status = status;
            if (status)
                InPack->skip(12);
            name = sDB->Guid_to_name(guid,false);
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
        *InPack >> note;
    if (result == 2 || result == 6)
    {
        *InPack >> status;
        InPack->skip(12);
    }
    if (result == 2 || result == 3 || result == 6 || result == 7)
    {
        if (note != "")
            sDB->FriendInfoMap[guid].note = note;
        sDB->FriendInfoMap[guid].status = status;
        name = sDB->Guid_to_name(guid,false);
        if (name[0] != '#')
            i_comm(string_format("%s%s\n",status ? "FrAN" : "FrAF",name.c_str()));
    }
}

void SocialModule::handle_smsg_guild_roster(inc_pack* InPack)
{
    print("Received Guild Rooster!\r\n");
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
            sDB->OnlineGuildMembers.push_back(guid);
        }
        sDB->Guid_to_name(guid,false);
    }
}

void SocialModule::handle_smsg_guild_event(inc_pack* InPack)
{
    uint8 Event;
    *InPack >> Event;
    switch (Event)
    {
    case GE_SIGNED_ON:
        {
            std::string name;
            uint32 guid;

            InPack->skip(1);
            *InPack >> name;
            *InPack >> guid;
            InPack->skip(4);
            
            sDB->OnlineGuildMembers.push_back(guid);
            i_comm(string_format("GuR%s\nGuA%s\n",name.c_str(),name.c_str()));
            print(string_format("[%s] has come online\r\n",name.c_str()));
            sDB->Guid_to_name(guid,false);
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

            sDB->OnlineGuildMembers.remove(guid);
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
