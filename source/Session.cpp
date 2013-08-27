#include "Session.h"

Session::Session()
{
    for(uint8 i=0;i<9;i++)
        channelson[i] = false;
    activechannel = 1;
    cinredirect = 0;
}

void Session::Update(inc_pack* InPack)
{
    switch(InPack->gc())
    {
    case 0: break;
    case 0x003B: handle_smsg_char_enum(InPack); return;                  //SMSG_CHAR_ENUM
    case 0x0051: handle_smsg_name_query_response(InPack); return;        //SMSG_NAME_QUERY_RESPONSE
    case 0x0096: handle_smsg_messagechat(InPack); return;                //SMSG_MESSAGECHAT
    case 0x0099: handle_smsg_channel_notify(InPack); return;             //SMSG_CHANNEL_NOTIFY
    case 0x009B: handle_smsg_channel_list(InPack); return;               //SMSG_CHANNEL_LIST
    case 0x01CB: handle_smsg_notification(InPack);return;                //SMSG_NOTIFICATION
    case 0x01EE: handle_smsg_auth_response(InPack); return;              //SMSG_AUTH_RESPONSE
    case 0x0236: handle_smsg_login_verify_world(InPack); return;         //SMSG_LOGIN_VERIFY_WORLD
    case 0x02A9: handle_smsg_chat_player_not_found(InPack);return;       //SMSG_CHAT_PLAYER_NOT_FOUND
    case 0x03EF:                                                         //SMSG_USERLIST_ADD
    case 0x03F1: handle_smsg_userlist_add(InPack); return;               //SMSG_USERLIST_UPDATE
        return;
    default:
        return;
    }
}
