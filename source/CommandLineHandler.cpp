#include "Session.h"

void Session::ClUpdate(cli_pack* InPack)
{
    switch(InPack->type)
    {
    case 0x01:  return handle_Cl(InPack);
    case 0x02:
        {
            if (cinredirect == 0)
                return send_cmsg_messagechat(InPack->data);
            else if (cinredirect == 1)
                return send_cmsg_login((uint8)InPack->data.c_str()[0]);
        }
    }
}

void Session::handle_Cl(cli_pack* InPack)
{
    if(InPack->data[0] == 0x00 || InPack->data[1] == 0x00)
        return;

    std::string     cmd,args;
    uint8           space=0;

    while(InPack->data.size() > space && InPack->data.c_str()[space] != ' ')
        space++;
    
    cmd = InPack->data.substr(1,space-1);
    if(InPack->data.size() > space)
        args = InPack->data.substr(space+1,InPack->data.size() - space +1);

    if(cmd[0] >'0' && cmd[0] <='9' && cmd[1] == 0x00)
    {
        activechannel = (uint8)cmd[0] - 48;
        if(args != "")
            return send_cmsg_messagechat(args);
        return;
    }

    if (cmd == "say" || cmd == "s")
    {
        activechannel = 21;
        if(args != "")
            return send_cmsg_messagechat(args);
        return;
    }

    if (cmd == "yell" || cmd == "y")
    {
        activechannel = 26;
        if(args != "")
            return send_cmsg_messagechat(args);
        return;
    }

    if (cmd == "guild" || cmd == "g")
    {
        activechannel = 24;
        if(args != "")
            return send_cmsg_messagechat(args);
        return;
    }

    if (cmd == "officer" || cmd == "o")
    {
        activechannel = 25;
        if(args != "")
            return send_cmsg_messagechat(args);
        return;
    }

    if(cmd == "w" && args != "")
    {
        activechannel = 27;
        space = 0;

        while(args.size() > space && args.c_str()[space] != ' ')
            space++;
        if(args.size() > space)
        {
            whisptarget = args.substr(0,space);
            std::string what = args.substr(space+1,args.size() - space +1);
            return send_cmsg_messagechat(what);
        }
        return;
    }

    if(cmd == "r")
    {
        activechannel = 27;
        if(args != "")
            return send_cmsg_messagechat(args);
        return;
    }

    if (cmd == "join")
    {
        if(args != "")
            return send_cmsg_join_channel(args);
        printf("wrong channel name\n");
        return;
    }

    if (cmd == "leave")
    {
        if(args != "")
            return send_cmsg_leave_channel((uint8)args.c_str()[0]-48);
        printf("usage: /leave channel number\n");
        return;
    }
    
    printf("unknown command: ");
    for(uint8 i=1;i<InPack->size;i++)
        printf("%c",InPack->data[i]);
    printf("\n");
}