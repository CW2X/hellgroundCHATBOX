#include "Session.h"
#include <iostream>

Session::Session()
{
    for(uint8 i=0;i<9;i++)
        channelson[i] = false;
    activechannel = 1;
    cinredirect = 0;
}

bool Session::Update(inc_pack* InPack,out_pack* OuPack)
{
    switch(InPack->gc())
    {
    case 0: break;
    case 0x003B: return handle_smsg_char_enum(InPack,OuPack);           //SMSG_CHAR_ENUM
    case 0x0051: return handle_smsg_name_query_response(InPack);        //SMSG_NAME_QUERY_RESPONSE
    case 0x0096: return handle_smsg_messagechat(InPack,OuPack);         //SMSG_MESSAGECHAT
    case 0x01EE: return handle_smsg_auth_response(InPack,OuPack);       //SMSG_AUTH_RESPONSE
    case 0x0236: return handle_smsg_login_verify_world(InPack,OuPack);  //SMSG_LOGIN_VERIFY_WORLD
        //opcodes to be taken care of ... possibly
    case 0x0067: //SMSG_CONTACT_LIST
    case 0x0092: //SMSG_GUILD_EVENT
    case 0x0099: //SMSG_CHANNEL_NOTIFY
    case 0x0103: //SMSG_EMOTE
    case 0x0105: //SMSG_TEXT_EMOTE
    case 0x01CB: //SMSG_NOTIFICATION
    case 0x02A9: //SMSG_CHAT_PLAYER_NOT_FOUND
    case 0x033D: //SMSG_MOTD
    case 0x03EF: //SMSG_USERLIST_ADD
    case 0x03F0: //SMSG_USERLIST_REMOVE
    case 0x03F1: //SMSG_USERLIST_UPDATE
        return true;
    default:
        {
            printf("received unhandled opcode: 0x%.4X size: %u\n",InPack->gc(),InPack->gs());
            return true;
        }
    }
    // here? means no inc_pack to process
    if (UnkPlayers.begin() != UnkPlayers.end())
        return send_cmsg_name_query(OuPack,*UnkPlayers.begin());

    return true;
}

bool Session::handle_smsg_auth_response(inc_pack* InPack,out_pack* OuPack)
{
    uint8 error;
    *InPack >> error;
    if (error != 0x0C)
    {
        printf("auth response received error: %u\n",error);
        return false;
    }
    printf("authorization succesful\n");
    return true;
}

bool Session::handle_smsg_char_enum(inc_pack* InPack,out_pack* OuPack)
{
    // data: |number of characters|char 1|char2|...
    // character struct: |guid(8)|name|0|race(1)|class(1)|gender(1)|look(5)|level(1)|zone(4)|map(4)|xyz floats(12)|
    // |guild(4)|flags(4)|1|petdisplay(4)|petlvl(4)|petfamily(4)|items (20*9)| = 239 + name
    uint16 point = 1;
    uint8  nofchars;

    *InPack >> nofchars;
    printf("received characters list, %u characters\n",nofchars);
    for (uint8 i=0;i<nofchars;i++)
    {
        *InPack >> characters[i].guid;
        InPack->skip(4);
        *InPack >> characters[i].name;
        *InPack >> characters[i].race >> characters[i].clas >> characters[i].gender;
        InPack->skip(5);
        *InPack >> characters[i].level;
        InPack->skip(29); InPack->skip(12);//pet data
        InPack->skip(20*9);//items
        printf("%s (guid %u): lvl %u %s %s %s\n",characters[i].name.c_str(),characters[i].guid,characters[i].level,
            (characters[i].gender ? "female": "male"),CharacterRaces[characters[i].race],CharacterClasses[characters[i].clas]);
        PlayersInfoMap[characters[i].guid].name = characters[i].name;
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
    OuPack->reset(0x003D);
    *OuPack << characters[i].guid;
    *OuPack << (uint32)0;
    return true;
}

bool Session::handle_smsg_login_verify_world(inc_pack* InPack,out_pack* OuPack)
{
    printf("player logged in\n\n");
    return send_cmsg_join_channel(OuPack,"world");
    return true;
}

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

bool Session::handle_smsg_messagechat(inc_pack* InPack,out_pack* OuPack)
{
    ChatMessage mes;

    *InPack >> mes.type;
    *InPack >> mes.lang >> mes.guid;
    InPack->skip(8);
    switch(mes.type)
    {
    case 1:     {mes.channel = "Say";break;}
    case 4:     {mes.channel = "Guild";break;} 
    case 5:     {mes.channel = "Officer";break;}
    case 6:     {mes.channel = "Yell";break;}
    case 17:    {*InPack >> mes.channel;break;}
    }
    
    InPack->skip(8);
    *InPack >> mes.length;
    *InPack >> mes.what;
    *InPack >> mes.tag;
    std::map<uint32,PlayerInfo>::const_iterator itr = PlayersInfoMap.find(mes.guid);
    if (itr != PlayersInfoMap.end())
        sprintf_s(mes.who,"%s",itr->second.name.c_str());
    else if (mes.guid !=0)
    {
        sprintf_s(mes.who,"#%u",mes.guid);
        UnkPlayers.push_back(mes.guid);
        RequestedPlayers.push_back(mes.guid);
    }

    switch(mes.type)
    {
    case 1:     //CHAT_MSG_SAY
    case 4:     //CHAT_MSG_GUILD
    case 5:     //CHAT_MSG_OFFICER
    case 6:     //CHAT_MSG_YELL
    case 17:    //CHAT_MSG_CHANNEL
        {
            printf("%s[%s]%s : ",ChatTagIdentifiers[mes.tag],mes.channel.c_str(),mes.who);
            printf("%s%s\n",(mes.lang <= 2 || mes.lang == 7) ? "":ChatLanguages(mes.lang),mes.what.c_str());
            break;
        }
    case 7:     //CHAT_MSG_WHISPER
        {
            printf("%s%s whispers:%s\n",ChatTagIdentifiers[mes.tag],mes.who,mes.what.c_str());
            break;
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

bool Session::send_cmsg_messagechat(std::string data,out_pack* OuPack)
{
    uint32 mtype;
    uint32 lang  =  7;//Common
    
    if( activechannel <20)
    {
        mtype = 17;
        if (!channelson[activechannel-1])
            return true;
    }
    else
        mtype = activechannel - 20;

    OuPack->reset (0x0095);
    *OuPack << mtype << lang;
    if (mtype == 17)
        *OuPack << channels[activechannel-1];
    if (mtype == 7)
        *OuPack << whisptarget;
    *OuPack << data;
    return true;
}

bool Session::send_cmsg_char_enum(out_pack* OuPack)
{
    printf("requesting character list\n");
    OuPack->reset( 0x0037);
    return true;
}

bool Session::send_cmsg_name_query(out_pack* OuPack,uint32 guid)
{
    OuPack->reset( 0x0050);
    *OuPack << guid << (uint32)0;
    UnkPlayers.remove(guid);
    return true;
}

bool Session::handle_smsg_name_query_response(inc_pack *InPack)
{
    uint32 guid;
    *InPack >> guid;
    InPack->skip(4);
    *InPack >> PlayersInfoMap[guid].name;
    InPack->skip(1);
    *InPack >> PlayersInfoMap[guid].race;
    *InPack >> PlayersInfoMap[guid].gender;
    *InPack >> PlayersInfoMap[guid].clas;

    for (std::list<uint32>::const_iterator itr = RequestedPlayers.begin(); itr !=RequestedPlayers.end();itr++)
    {
        if(*itr == guid)
        {
            printf("player guid %u is %s\n",guid, PlayersInfoMap[guid].name.c_str());
            RequestedPlayers.remove(guid);
            break;
        }
    }
    return true;
}