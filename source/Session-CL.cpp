#include "Session.h"

bool Session::ClUpdate(cli_pack* InPack,out_pack* OuPack)
{
    switch(InPack->type)
    {
    case 0x01:  return handle_Cl(InPack,OuPack);
    case 0x02:
        {
            if (cinredirect == 0)
                return send_chat_message(InPack->data,OuPack);
            else if (cinredirect == 1)
                return send_cmsg_login(OuPack,(uint8)InPack->data.c_str()[0]);
        }
    }
    return true;
}

bool Session::handle_Cl(cli_pack* InPack,out_pack* OuPack)
{
    if(InPack->data[0] = 0x00)
        return true;

    char            cmd[256];
    std::size_t     pos = InPack->data.find(' ');

    if (pos!=std::string::npos)
        {InPack->data.copy(cmd,pos-1,1);
    cmd[pos-1]= 0x00;}
    else
        {InPack->data.copy(cmd,InPack->data.size()-1,1);
    cmd[InPack->data.size()-1] =0x00;}

    if(*cmd >'0' && *cmd <='9')
    {activechannel = *cmd - 48;return true;}
    switch(*cmd)
    {
    case 'c':
        return send_char_enum(OuPack);
    }
    printf("unknown command: ");
    for(uint8 i=1;i<InPack->size;i++)
        printf("%c",InPack->data[i]);
    printf("\n");
    return true;
}