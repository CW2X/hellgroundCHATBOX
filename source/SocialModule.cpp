#include "SocialModule.h"
#include "Util.h"
#include "Database.h"

void SocialModule::Handle(inc_pack* InPack)
{
    switch(InPack->gc())
    {
    case 0x0067: handle_smsg_contact_list(InPack);break;//SMSG_CONTACT_LIST
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
                command(string_format("%s%s",status ? "FrAN" : "FrAF",name.c_str()));
        };
    }
}
