#include "Session.h"

bool Session::ClUpdate(cli_pack* InPack,out_pack* OuPack)
{
    switch(InPack->type)
    {
    case 0x01:  return handle_Cl(InPack,OuPack);
    case 0x02:  return send_chat_message(InPack->data,OuPack);
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
    {activechannel = *cmd - 30; return true;}
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