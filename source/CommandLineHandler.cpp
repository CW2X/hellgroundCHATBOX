#include "Session.h"

void Session::ClUpdate(std::string clData)
{
    if (clData.c_str()[0] == '/')
        handle_Cl(clData);
    else
    {
        {
            if (cinredirect == 0)
                return send_cmsg_messagechat(clData);
            else if (cinredirect == 1)
                return send_cmsg_login((uint8)clData.c_str()[0]);
        }
    }
}

void Session::handle_Cl(std::string clData)
{
    if(clData[0] == 0x00 || clData[1] == 0x00)
        return;

    std::string     cmd,args;
    uint8           space=0;

    while(clData.size() > space && clData.c_str()[space] != ' ')
        space++;
    
    cmd = clData.substr(1,space-1);
    if(clData.size() > space)
        args = clData.substr(space+1,clData.size() - space +1);

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
        printf("wrong channel name\r\n");
        return;
    }

    if (cmd == "leave")
    {
        if(args != "")
            return send_cmsg_leave_channel((uint8)args.c_str()[0]-48);
        printf("usage: /leave channel number\r\n");
        return;
    }
}
