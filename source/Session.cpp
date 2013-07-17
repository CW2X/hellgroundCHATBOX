#include "Session.h"
#include <iostream>

Session::Session()
{
    nofchannels = 0;
    activechannel = 1;
    cinredirect = 0;
}

bool Session::Update(inc_pack* InPack,out_pack* OuPack)
{
    switch(InPack->cmd)
    {
    case 0: break;
    case 0x003B: return handle_char_enum(InPack,OuPack);        //SMSG_CHAR_ENUM
    case 0x0096: return handle_chat_message(InPack,OuPack);     //SMSG_MESSAGECHAT
    case 0x01EE: return handle_auth_response(InPack,OuPack);    //SMSG_AUTH_RESPONSE
    case 0x0236: return handle_login_verify(InPack,OuPack);     //SMSG_LOGIN_VERIFY_WORLD
        //opcodes to be taken care of ... possibly
    case 0x0067: //SMSG_CONTACT_LIST
    case 0x0099: //SMSG_CHANNEL_NOTIFY
    case 0x0103: //SMSG_EMOTE
    case 0x01CB: //SMSG_NOTIFICATION
    case 0x033D: //SMSG_MOTD
    case 0x03F1: //SMSG_USERLIST_UPDATE
        break;
    default:
        {
            printf("received unhandled opcode: 0x%.4X size: %u\n",InPack->cmd,InPack->size);
            break;
        }
    }
    
    return true;
}

bool Session::handle_auth_response(inc_pack* InPack,out_pack* OuPack)
{
    if (InPack->data[0] != 0x0C)
    {
        printf("auth response received error: %u\n",InPack->data[0]);
        return false;
    }
    printf("authorization succesful\n");
    return true;
}

bool Session::handle_char_enum(inc_pack* InPack,out_pack* OuPack)
{
    // data: |number of characters|char 1|char2|...
    // character struct: |guid(8)|name|0|race(1)|class(1)|gender(1)|look(5)|level(1)|zone(4)|map(4)|xyz floats(12)|
    // |guild(4)|flags(4)|1|petdisplay(4)|petlvl(4)|petfamily(4)|items (20*9)| = 239 + name
    uint16 point = 1;
    
    printf("received characters list, %u characters\n",InPack->data[0]);
    for (uint8 i=0;i<InPack->data[0];i++)
    {
        characters[i].guid = MAKE_UINT32(InPack->data[point+3],InPack->data[point+2],InPack->data[point+1],InPack->data[point]);
        point += 8;
        while (InPack->data[point])
            characters[i].name.append(1,(char)InPack->data[point++]);
        characters[i].race   = InPack->data[point+1];
        characters[i].clas   = InPack->data[point+2];
        characters[i].gender = InPack->data[point+3];
        characters[i].level  = InPack->data[point+9];
        point += 231;
        printf("%s (guid %u): lvl %u %s %s %s\n",characters[i].name.c_str(),characters[i].guid,characters[i].level,
            (characters[i].gender ? "female": "male"),CharacterRaces[characters[i].race],CharacterClasses[characters[i].clas]);
    }
    printf("select character: ");
    cinredirect = 1;
    return true;
}

bool Session::send_cmsg_login(out_pack* OuPack,uint8 i)
{
    i -= 48;
    cinredirect = 0;
    if (i>7 || characters[--i].guid == 0)
        i = 0;
    OuPack->cmd = 0x003D;
    OuPack->size = 8;
    
    OuPack->data[3] = uint8(characters[i].guid & 0xFF000000);
    OuPack->data[2] = uint8(characters[i].guid & 0x00FF0000);
    OuPack->data[1] = uint8(characters[i].guid & 0x0000FF00);
    OuPack->data[0] = uint8(characters[i].guid & 0x000000FF);
    OuPack->data[4] = 0;OuPack->data[5] = 0;OuPack->data[6] = 0;OuPack->data[7] = 0;
    return true;
}

bool Session::handle_login_verify(inc_pack* InPack,out_pack* OuPack)
{
    printf("player logged in\n\n");
    return send_cmsg_join_channel(OuPack,"world");
    return true;
}

bool Session::send_cmsg_join_channel(out_pack* OuPack,std::string name)
{
    OuPack->cmd  = 0x097;
    OuPack->size = 8 + name.size();
    OuPack->data[0] = 0x00; OuPack->data[1] = 0x00; OuPack->data[2] = 0x00; OuPack->data[3] = 0x00;
    OuPack->data[4] = 0x00; OuPack->data[5] = 0x00;
    for(uint8 i=0;i<name.size();i++)
        OuPack->data[6+i] = (uint8)(name.c_str()[i]);
    OuPack->data[name.size()+6] = 0x00; OuPack->data[name.size()+7] = 0x00;
    channels[nofchannels++] = name;
    printf("Joining channel %s [%u]\n",name.c_str(),nofchannels);
    return true;
}

bool Session::handle_chat_message(inc_pack* InPack,out_pack* OuPack)
{
    ChatMessage mes;
    uint8       pos;

    mes.type = InPack->data[0];
    switch(mes.type)
    {
    case 17: //CHAT_MSG_CHANNEL
        {
            mes.lang = MAKE_UINT32(InPack->data[4],InPack->data[3],InPack->data[2],InPack->data[1]);
            mes.guid = MAKE_UINT32(InPack->data[8],InPack->data[7],InPack->data[6],InPack->data[5]);
            pos = 17;
            while(InPack->data[pos])
                mes.channel.append(1,(char)InPack->data[pos++]);
            mes.length = MAKE_UINT32(InPack->data[pos+12],InPack->data[pos+11],InPack->data[pos+10],InPack->data[pos+9]);
            pos += 13;
            while(InPack->data[pos])
                mes.what.append(1,(char)InPack->data[pos++]);
            mes.tag = InPack->data[++pos];
            printf("%s%-10u[%s][%s]:%s\n",ChatTagIdentifiers[mes.tag],mes.guid,mes.channel.c_str(),ChatLanguages(mes.lang),mes.what.c_str());
        }
    default:
        break;
    }
    return true;
}

char* Session::ChatLanguages(uint32 lang)
{
    switch(lang)
    {
    case 0: return "UNIV";
    case 1: return "ORCI";
    case 2: return "DARN";
    case 3: return "TAUR";
    case 6: return "DVAR";
    case 7: return "COMM";
    case 8: return "DEMO";
    case 9: return "TITA";
    case 10: return "THAL";
    case 11: return "DRAC";
    case 12: return "KALI";
    case 13: return "GNOM";
    case 14: return "TROL";
    case 33: return "GUTT";
    case 35: return "DRAE";
    case 36: return "ZOMB";
    case 37: return "GN_B";
    case 38: return "GO_B";
    case 0xFFFFFFFF: return "ADDO";
    default: return "UNKN";
    }
}

bool Session::send_chat_message(std::string data,out_pack* OuPack)
{
    uint32 mtype = 17;//CHAT_MSG_CHANNEL
    uint32 lang  =  7;//Common
    uint8 point  =  8;
    if (activechannel > nofchannels)
        return true;

    OuPack->cmd = 0x0095;
    OuPack->data[0] = uint8(mtype & 0x000000FF);
    OuPack->data[1] = uint8(mtype & 0x0000FF00);
    OuPack->data[2] = uint8(mtype & 0x00FF0000);
    OuPack->data[3] = uint8(mtype & 0xFF000000);
    OuPack->data[4] = uint8(lang & 0x000000FF);
    OuPack->data[5] = uint8(lang & 0x0000FF00);
    OuPack->data[6] = uint8(lang & 0x00FF0000);
    OuPack->data[7] = uint8(lang & 0xFF000000);
    while(channels[activechannel-1].c_str()[point-8] != 0x00)
    { 
        OuPack->data[point] = (uint8)(channels[activechannel-1].c_str()[point-8]);
        point++;
    }
    OuPack->data[point] = 0x00; 
    point++;
    
    for(uint8 i=0; i<data.size(); i++)
        OuPack->data[point+i] = data.c_str()[i]; 
    OuPack->data[point + data.size()] = 0x00; 
    OuPack->size = point + 1 + data.size(); 
    return true;
}

bool Session::send_char_enum(out_pack* OuPack)
{
    printf("requesting character list\n");
    OuPack->cmd  = 0x0037;
    OuPack->size = 0;
    return true;
}