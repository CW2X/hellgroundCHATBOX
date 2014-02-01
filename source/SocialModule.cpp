#include "SocialModule.h"
#include "Util.h"
#include "Database.h"

void SocialModule::Handle(inc_pack* InPack)
{
    switch(InPack->gc())
    {
    case 0x0067: handle_smsg_contact_list(InPack);break; //SMSG_CONTACT_LIST
    case 0x0068: handle_smsg_friend_status(InPack);break;//SMSG_FRIEND_STATUS
    default:
        break;
    }
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
                command(string_format("%s%s\n",status ? "FrAN" : "FrAF",name.c_str()));
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
            command(string_format("%s%s\n",status ? "FrAN" : "FrAF",name.c_str()));
    }
}
