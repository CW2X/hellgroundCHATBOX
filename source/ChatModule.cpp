#include "ChatModule.h"
#include "Util.h"
#include "Database.h"

ChatModule::ChatModule()
{
    lang = 0;
    for(uint8 i=0;i<9;i++)
        channelson[i] = false;
    activechannel = 21;
}

void ChatModule::Handle(inc_pack* InPack)
{
    switch(InPack->gc())
    {
    case 0x0096: handle_smsg_messagechat(InPack); break;                //SMSG_MESSAGECHAT
    case 0x0099: handle_smsg_channel_notify(InPack); break;             //SMSG_CHANNEL_NOTIFY
    case 0x009B: handle_smsg_channel_list(InPack); break;               //SMSG_CHANNEL_LIST
    case 0x01CB: handle_smsg_notification(InPack);break;                //SMSG_NOTIFICATION
    case 0x02A9: handle_smsg_chat_player_not_found(InPack);break;       //SMSG_CHAT_PLAYER_NOT_FOUND
    case 0x03EF:                                                        //SMSG_USERLIST_ADD
    case 0x03F1: handle_smsg_userlist_add(InPack); break;               //SMSG_USERLIST_UPDATE
    default:
        break;
    }
}

void ChatModule::Command(std::string cmd,std::string args)
{
    uint8 space;
    if (cmd == "msg")
        return send_cmsg_messagechat(args);

    if (cmd == "say" || cmd == "s")
    {
        activechannel = 21;
        if(args != "")
            send_cmsg_messagechat(args);
        command("Ch:Say\n");
        return;
    }

    if (cmd == "yell" || cmd == "y")
    {
        activechannel = 26;
        if(args != "")
            send_cmsg_messagechat(args);
        command("Ch:Yell\n");
        return;
    }

    if (cmd == "guild" || cmd == "g")
    {
        activechannel = 24;
        if(args != "")
            send_cmsg_messagechat(args);
        command("Ch:Guild\n");
        return;
    }

    if (cmd == "officer" || cmd == "o")
    {
        activechannel = 25;
        if(args != "")
            send_cmsg_messagechat(args);
        command("Ch:Officer\n");
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
            send_cmsg_messagechat(what);
        }
        command("Ch:Whisper "+whisptarget + "\n");
        return;
    }

    if(cmd == "r")
    {
        activechannel = 27;
        if(args != "")
            send_cmsg_messagechat(args);
        command("Ch:Whisper "+whisptarget + "\n");
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

    if(cmd[0] >'0' && cmd[0] <='9')
    {
        activechannel = (uint8)cmd[0] - 48;
        if(args != "")
            send_cmsg_messagechat(args);
        command(string_format("Ch:[%s]\n",channels[activechannel-1].c_str()));
        return;
    }  
}

void ChatModule::send_cmsg_join_channel(std::string name)
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
    print(string_format("Joining channel %s [%u]\r\n",name.c_str(),channelid+1));
    send_out_pack();
}

void ChatModule::send_cmsg_leave_channel(uint8 no)
{
    if (!no || no > 9)
    {
        print("usage: /leave channel number\r\n");
        return;
    }
    if (channelson[no-1] == false)
        return;
    print(string_format("leaving channel %s [%u]\r\n",channels[no-1].c_str(),no));
    OuPack.reset (0x0098);
    OuPack << (uint32)0;
    OuPack << channels[no-1];
    channels[no-1] = "";
    channelson[no-1] = false;
    send_out_pack();
}

void ChatModule::handle_smsg_channel_notify(inc_pack* InPack)
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
            print(string_format("%s :owner changed to %s\r\n",channelname.c_str(),sDB->Guid_to_name(guid,true).c_str()));
            break;
        }
    case 0x0C:
        break;
    default:
        print(string_format("received notify %u for channel %s",type,channelname.c_str()));
    }
}

void ChatModule::send_cmsg_channel_list(std::string channelname)
{

    OuPack.reset(0x009A);
    OuPack << channelname;
    send_out_pack();
}

void ChatModule::handle_smsg_channel_list(inc_pack* InPack)
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
        sDB->Guid_to_name(guid,false);
    }
}

void ChatModule::handle_smsg_userlist_add(inc_pack* InPack)
{
    uint32 guid;
    *InPack >> guid;
    sDB->Guid_to_name(guid,false);
}

void ChatModule::handle_smsg_messagechat(inc_pack* InPack)
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
    mes.who = sDB->Guid_to_name(mes.guid,true);

    switch(mes.type)
    {
    case 1:     //CHAT_MSG_SAY
    case 4:     //CHAT_MSG_GUILD
    case 5:     //CHAT_MSG_OFFICER
    case 6:     //CHAT_MSG_YELL
    case 17:    //CHAT_MSG_CHANNEL
        {
            print(string_format("%s[%s]%s : %s%s\r\n",ChatTagIdentifiers[mes.tag],mes.channel.c_str(),mes.who.c_str(),
                ((mes.lang <= 2 || mes.lang == 7) ? "":ChatLanguages(mes.lang)),mes.what.c_str()));
            break;
        }
    case 7:     //CHAT_MSG_WHISPER
        {
            print(string_format("%s%s whispers: %s\r\n",ChatTagIdentifiers[mes.tag],mes.who.c_str(),mes.what.c_str()));
            break;
        }
    case 9:     //CHAT_MSG_REPLY
        {
            print(string_format("To %s : %s\r\n",mes.who.c_str(),mes.what.c_str()));
            break;
        }
    case 0:     //CHAT_MSG_SYSTEM
        {
            print("> " + mes.what + "\r\n");
            break;
        }
    default:
        {
            print(string_format("message type %u : %s\r\n",mes.type,mes.what.c_str()));
            break;
        }
    }
    return;
}

char* ChatModule::ChatLanguages(uint32 lang)
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

void ChatModule::send_cmsg_messagechat(std::string data)
{
    uint32 mtype;

    if (lang == 0)
        lang = sDB->ishordeplayer ? 1:7;
    
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
    send_out_pack();
}

void ChatModule::handle_smsg_chat_player_not_found(inc_pack *InPack)
{
    std::string playername;
    *InPack >> playername;
    print("Player " + playername + " not found\r\n");
}

void ChatModule::handle_smsg_notification(inc_pack *InPack)
{
    std::string notification;
    *InPack >> notification;
    print("NOTIFICATION: " + notification + "\r\n");
}
