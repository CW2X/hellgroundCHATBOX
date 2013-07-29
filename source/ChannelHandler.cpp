#include "Session.h"

bool Session::send_cmsg_join_channel(out_pack* OuPack,std::string name)
{
    uint8 channelid = 9;
    for (uint8 i=9;i>0;i--)
    {
        if(channelson[i-1] == false)
            channelid = i-1;
    }

    if (channelid == 9)
    {
        printf("too many chanels, leave one to join next");
        return true;
    }

    OuPack->reset(0x097);
    *OuPack << channelid;
    *OuPack << (uint8)0;
    *OuPack << (uint32)0;
    *OuPack << name;
    *OuPack << (uint8)0; // channel password string
    channels[channelid] = name;
    channelson[channelid] = true;
    printf("Joining channel %s [%u]\n",name.c_str(),channelid+1);
    return true;
}

bool Session::send_cmsg_leave_channel(out_pack* OuPack,uint8 no)
{
    if (!no || no > 9)
    {
        printf("usage: /leave channel number\n");
        return true;
    }
    if (channelson[no-1] == false)
        return true;
    printf("leaving channel %s [%u]\n",channels[no-1].c_str(),no);
    OuPack->reset (0x0098);
    *OuPack << (uint32)0;
    *OuPack << channels[no-1];
    channels[no-1] = "";
    channelson[no-1] = false;
    return true;
}

bool Session::handle_smsg_channel_notify(inc_pack* InPack,out_pack* OuPack)
{
    uint8       type;
    std::string channelname;

    *InPack >> type;
    *InPack >> channelname;
    switch (type)
    {
    case 0x02:
        {printf("Joined channel %s\n",channelname.c_str());return send_cmsg_channel_list(OuPack,channelname);}
    default:
        printf("received notify %u for channel %s\n",type,channelname.c_str());
    }
    return true;
}

bool Session::send_cmsg_channel_list(out_pack* OuPack, std::string channelname)
{
    OuPack->reset(0x009A);
    *OuPack << channelname;
    return true;
}

bool Session::handle_smsg_channel_list(inc_pack* InPack)
{
    std::string channelname;
    uint32      nofplayers;
    uint32      guid;
    
    InPack->skip(1);
    *InPack >> channelname;
    InPack->skip(1);
    *InPack >> nofplayers;
    for (uint32 i=0;i<nofplayers;i++)
    {
        *InPack >> guid;
        InPack->skip(5);
        std::map<uint32,PlayerInfo>::const_iterator itr = PlayersInfoMap.find(guid);
        if (itr == PlayersInfoMap.end())
            UnkPlayers.push_back(guid);
    }
    
    return true;
}