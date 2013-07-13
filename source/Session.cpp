#include "Session.h"

bool Session::Update(inc_pack* InPack,out_pack* OuPack)
{
    switch(InPack->cmd)
    {
    case 0: break;
    case 0x003B: return handle_char_enum(InPack,OuPack);
    case 0x0096: return handle_chat_message(InPack,OuPack);//SMSG_MESSAGECHAT
    case 0x01EE: return handle_auth_response(InPack,OuPack);
    case 0x0236: return handle_login_verify(InPack,OuPack);
        //opcodes to be taken care of ... possibly
    case 0x0067: //SMSG_CONTACT_LIST
    case 0x0099: //SMSG_CHANNEL_NOTIFY
    case 0x0103: //SMSG_EMOTE
    case 0x033D: //SMSG_MOTD
    case 0x03F1: //SMSG_USERLIST_UPDATE
        break;
        //ignored opcodes:
    case 0x0042: //SMSG_LOGIN_SETTIMESPEED
    case 0x00A9: //SMSG_UPDATE_OBJECT
    case 0x00AA: //SMSG_DESTROY_OBJECT
    case 0x00DD: //SMSG_MONSTER_MOVE
    case 0x00E2: //SMSG_FORCE_RUN_SPEED_CHANGE
    case 0x00E6: //SMSG_FORCE_SWIM_SPEED_CHANGE
    case 0x00FD: //SMSG_TUTORIAL_FLAGS
    case 0x0122: //SMSG_INITIALIZE_FACTIONS
    case 0x0127: //SMSG_SET_PROFICIENCY
    case 0x0129: //SMSG_ACTION_BUTTONS
    case 0x012A: //SMSG_INITIAL_SPELLS
    case 0x0132: //SMSG_SPELL_GO
    case 0x0137: //SMSG_UPDATE_AURA_DURATION
    case 0x013C: //SMSG_AI_REACTION
    case 0x0143: //SMSG_ATTACKSTART
    case 0x0144: //SMSG_ATTACKSTOP
    case 0x014A: //SMSG_ATTACKERSTATEUPDATE
    case 0x0155: //SMSG_BINDPOINTUPDATE
    case 0x01F6: //SMSG_COMPRESSED_UPDATE_OBJECT
    case 0x0209: //SMSG_ACCOUNT_DATA_TIMES
    case 0x021E: //SMSG_SET_REST_START
    case 0x02C2: //SMSG_INIT_WORLD_STATES
    case 0x02F4: //SMSG_WEATHER
    case 0x0329: //MSG_SET_DUNGEON_DIFFICULTY
    case 0x030D: //SMSG_SPLINE_MOVE_SET_RUN_MODE
    case 0x0381: //SMSG_FORCE_FLIGHT_SPEED_CHANGE
    case 0x0390: //SMSG_TIME_SYNC_REQ
    case 0x03A4: //SMSG_SET_EXTRA_AURA_INFO
    case 0x03C8: //SMSG_FEATURE_SYSTEM_STATUS
    case 0x041D: //SMSG_SEND_UNLEARN_SPELLS
    
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
    OuPack->cmd  = 0x0037;
    OuPack->size = 0;
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
    return send_cmsg_login(OuPack);
}

bool Session::send_cmsg_login(out_pack* OuPack)
{
    OuPack->cmd = 0x003D;
    OuPack->size = 8;
    
    OuPack->data[3] = uint8(characters[0].guid & 0xFF000000);
    OuPack->data[2] = uint8(characters[0].guid & 0x00FF0000);
    OuPack->data[1] = uint8(characters[0].guid & 0x0000FF00);
    OuPack->data[0] = uint8(characters[0].guid & 0x000000FF);
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