#include "Session.h"

Session::Session()
{
    cinredirect = 2;
}

void Session::Update(inc_pack* InPack)
{
    switch(InPack->gc())
    {
    case 0: break;
    case 0x003B: sLoginModule.Handle(InPack); cinredirect = 1; break; //SMSG_CHAR_ENUM
    case 0x01EE: sLoginModule.Handle(InPack); break;                  //SMSG_AUTH_RESPONSE
    case 0x0236: sLoginModule.Handle(InPack);                         //SMSG_LOGIN_VERIFY_WORLD
        ClUpdate("/join world"); break; 

    case 0x0096: //SMSG_MESSAGECHAT
    case 0x0099: //SMSG_CHANNEL_NOTIFY
    case 0x009B: //SMSG_CHANNEL_LIST
    case 0x01CB: //SMSG_NOTIFICATION
    case 0x02A9: //SMSG_CHAT_PLAYER_NOT_FOUND
    case 0x03EF: //SMSG_USERLIST_ADD
    case 0x03F1: //SMSG_USERLIST_UPDATE
        sChatModule.Handle(InPack); break;
    
    case 0x0067: //SMSG_CONTACT_LIST
    case 0x0068: //SMSG_FRIEND_STATUS
        sSocialModule.Handle(InPack); break;

    case 0x0051: //SMSG_NAME_QUERY_RESPONSE
        sDB->Handle(InPack); break;
    default:
        break;
    }
}
