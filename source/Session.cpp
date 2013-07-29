#include "Session.h"

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
    case 0x0099: return handle_smsg_channel_notify(InPack,OuPack);      //SMSG_CHANNEL_NOTIFY
    case 0x009B: return handle_smsg_channel_list(InPack);               //SMSG_CHANNEL_LIST
    case 0x01EE: return handle_smsg_auth_response(InPack,OuPack);       //SMSG_AUTH_RESPONSE
    case 0x0236: return handle_smsg_login_verify_world(InPack,OuPack);  //SMSG_LOGIN_VERIFY_WORLD
        //opcodes to be taken care of ... possibly
    case 0x0067: //SMSG_CONTACT_LIST
    case 0x0092: //SMSG_GUILD_EVENT
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
