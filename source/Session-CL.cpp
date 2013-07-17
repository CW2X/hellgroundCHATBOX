#include "Session.h"

bool Session::ClUpdate(cli_pack* InPack,out_pack* OuPack)
{
    switch(InPack->type)
    {
    case 0x01:  return handle_Cl(InPack,OuPack);
    case 0x02:
        {
            if (cinredirect == 0)
                return send_cmsg_messagechat(InPack->data,OuPack);
            else if (cinredirect == 1)
                return send_cmsg_login(OuPack,(uint8)InPack->data.c_str()[0]);
        }
    }
    return true;
}

bool Session::handle_Cl(cli_pack* InPack,out_pack* OuPack)
{
    if(InPack->data[0] == 0x00 || InPack->data[1] == 0x00)
        return true;

    std::string     cmd,args;
    uint8           space=0;

    while(InPack->data.size() > space && InPack->data.c_str()[space] != ' ')
        space++;
    
    cmd = InPack->data.substr(1,space-1);
    if(InPack->data.size() > space)
        args = InPack->data.substr(space+1,InPack->data.size() - space +1);
    if(cmd[0] >'0' && cmd[0] <='9' && cmd.size() ==1)
    {activechannel = (uint8)cmd[0] - 48;return true;}
    if (cmd == "c")
        return send_cmsg_char_enum(OuPack);
    if (cmd == "join")
    {
        if(args != "")
            return send_cmsg_join_channel(OuPack,args);
        printf("wrong channel name\n");
        return true;
    }
    if (cmd == "leave")
    {
        if(args != "")
            return send_cmsg_leave_channel(OuPack, (uint8)args.c_str()[0]-48);
        printf("usage: /leave channel number\n");
        return true;
    }
    
    printf("unknown command: ");
    for(uint8 i=1;i<InPack->size;i++)
        printf("%c",InPack->data[i]);
    printf("\n");
    return true;
}