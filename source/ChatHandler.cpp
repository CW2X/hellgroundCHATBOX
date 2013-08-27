#include "Session.h"
#include "Util.h"

void Session::handle_smsg_messagechat(inc_pack* InPack)
{
    ChatMessage mes;

    *InPack >> mes.type;
    *InPack >> mes.lang >> mes.guid;
    InPack->skip(8);

    if (mes.lang == 0xFFFFFFFF)
        return;
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
    mes.who = Guid_to_name(mes.guid);

    switch(mes.type)
    {
    case 1:     //CHAT_MSG_SAY
    case 4:     //CHAT_MSG_GUILD
    case 5:     //CHAT_MSG_OFFICER
    case 6:     //CHAT_MSG_YELL
    case 17:    //CHAT_MSG_CHANNEL
        {
            printf("%s[%s]%s : ",ChatTagIdentifiers[mes.tag],mes.channel.c_str(),mes.who.c_str());
            printf("%s%s\n",(mes.lang <= 2 || mes.lang == 7) ? "":ChatLanguages(mes.lang),mes.what.c_str());
            break;
        }
    case 7:     //CHAT_MSG_WHISPER
        {
            printf("%s%s whispers : %s\n",ChatTagIdentifiers[mes.tag],mes.who.c_str(),mes.what.c_str());
            break;
        }
    case 9:     //CHAT_MSG_REPLY
        {
            printf("To %s : %s\n",mes.who.c_str(),mes.what.c_str());
            break;
        }
    case 0:     //CHAT_MSG_SYSTEM
        {
            printf("> %s\n",mes.what.c_str());
            break;
        }
    default:
        {
            printf("message type %u : %s\n",mes.type,mes.what.c_str());
            break;
        }
    }
    return;
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

std::string Session::Guid_to_name(uint32 guid)
{
    std::string name;
    std::map<uint32,PlayerInfo>::const_iterator itr = PlayersInfoMap.find(guid);
    if (itr != PlayersInfoMap.end())
        name = itr->second.name;
    else if (guid !=0)
    {
        name = string_format("#%u",guid);
        send_cmsg_name_query(guid);
        RequestedPlayers.push_back(guid);
    }
    return name;
}

void Session::send_cmsg_messagechat(std::string data)
{
    uint32 mtype;
    uint32 lang;

    if (ishordeplayer)
        lang = 1;
    else
        lang = 7;

    
    if( activechannel <20)
    {
        mtype = 17;
        if (!channelson[activechannel-1])
            return;
    }
    else
        mtype = activechannel - 20;

    OuPack.reset (0x0095);
    OuPack << mtype << lang;
    if (mtype == 17)
        OuPack << channels[activechannel-1];
    if (mtype == 7)
        OuPack << whisptarget;
    OuPack << data;
    sMainSocket.send_out_pack(&OuPack);
}

void Session::send_cmsg_name_query(uint32 guid)
{
    OuPack.reset( 0x0050);
    OuPack << guid << (uint32)0;
    sMainSocket.send_out_pack(&OuPack);
}

void Session::handle_smsg_name_query_response(inc_pack *InPack)
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
}

void Session::handle_smsg_chat_player_not_found(inc_pack *InPack)
{
    std::string playername;
    *InPack >> playername;
    printf("Player %s not found\n",playername.c_str());
}

void Session::handle_smsg_notification(inc_pack *InPack)
{
    std::string notification;
    *InPack >> notification;
    printf("NOTIFICATION: %s\n",notification.c_str());
}