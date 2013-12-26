#include "Session.h"
#include "Util.h"

void Session::send_cmsg_join_channel(std::string name)
{
    uint8 channelid = 9;
    for (uint8 i=9;i>0;i--)
    {
        if(channelson[i-1] == false)
            channelid = i-1;
    }

    if (channelid == 9)
    {
        print("too many chanels, leave one to join next");
        return;
    }
    OuPack.reset(0x097);
    OuPack << channelid;
    OuPack << (uint8)0;
    OuPack << (uint32)0;
    OuPack << name;
    OuPack << (uint8)0;
    channels[channelid] = name;
    channelson[channelid] = true;
    print("Joining channel " + name + " [" + utostr(channelid+1) + "]\r\n");
    send_out_pack();
}

void Session::send_cmsg_leave_channel(uint8 no)
{
    if (!no || no > 9)
    {
        print("usage: /leave channel number\r\n");
        return;
    }
    if (channelson[no-1] == false)
        return;
    print("leaving channel " + channels[no-1] + " [" + utostr(no) + "]\r\n");
    OuPack.reset (0x0098);
    OuPack << (uint32)0;
    OuPack << channels[no-1];
    channels[no-1] = "";
    channelson[no-1] = false;
    send_out_pack();
}

void Session::handle_smsg_channel_notify(inc_pack* InPack)
{
    uint8       type;
    std::string channelname;

    *InPack >> type;
    *InPack >> channelname;
    switch (type)
    {
    case 0x02:
        {
            print("Joined channel " + channelname + "\r\n");
            send_cmsg_channel_list(channelname);
            break;
        }
    case 0x08:
        {
            uint32 guid;
            *InPack >> guid;
            print(channelname + " :owner changed to " + Guid_to_name(guid) + "\r\n");
            break;
        }
    case 0x0C:
        break;
    default:
        print("received notify " + utostr(type) + " for channel " + channelname + "\r\n");
    }
}

void Session::send_cmsg_channel_list(std::string channelname)
{

    OuPack.reset(0x009A);
    OuPack << channelname;
    send_out_pack();
}

void Session::handle_smsg_channel_list(inc_pack* InPack)
{
    std::string channelname;
    uint32      nofplayers;
    uint32      guid;
    
    InPack->skip(1);
    *InPack >> channelname;
    if (channelname == "")
        return;
    InPack->skip(1);
    *InPack >> nofplayers;
    for (uint32 i=0;i<nofplayers;i++)
    {
        *InPack >> guid;
        InPack->skip(5);
        std::map<uint32,PlayerInfo>::const_iterator itr = PlayersInfoMap.find(guid);
        if (itr == PlayersInfoMap.end())
            send_cmsg_name_query(guid);
    }
}

void Session::handle_smsg_userlist_add(inc_pack* InPack)
{
    uint32 guid;
    *InPack >> guid;
    std::map<uint32,PlayerInfo>::const_iterator itr = PlayersInfoMap.find(guid);
    if (itr == PlayersInfoMap.end())
        send_cmsg_name_query(guid);
}
