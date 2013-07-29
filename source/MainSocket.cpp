#include "MainSocket.h"

MainSocket::MainSocket()
{
    loaded  = 0;
}

bool MainSocket::Update(inc_pack* packet)
{
    char recvbuff[BUFFER_SIZE_IN];
    uint16 datalength;
    packet->reset();

    if(!IsConnected)
    {
        m_crypt.SetKey(&K);
        m_crypt.Init();
        return open_socket();
    }
    if(!IsAuthed)
    {
        datalength = 0;
        if (!recv_packet(recvbuff,&datalength))
            return false;
        if (datalength>0 && recv_auth_challenge(recvbuff,datalength))
            return send_auth_session();
        return true;
    }

    if (loaded == 0) // did we received whole data last time?
    {
        datalength = 4;
        if (!recv_packet(recvbuff,&datalength))
            return false;
        if (datalength == 0)
            return true;
        decrypt_header((uint8*)recvbuff);

        curc = (uint16)(((uint8)recvbuff[3] << 8) | (uint8)recvbuff[2]);
        curs = (uint16)(((uint8)recvbuff[0] << 8) | (uint8)recvbuff[1]) - 2;
        
        if(curs > BUFFER_SIZE_IN)
        {
            printf("possible Buffer Overflow! Interrupting (%u 0x%04X)",curs,curc);
            return false;
        }

        datalength = curs;
        if (datalength)
        {
            if (!recv_packet(recvbuff,&datalength))
                return false;
        }

        if (datalength < curs)
        { // not whole packet received
            loaded = datalength;
            packet->set(recvbuff);
            return true;
        }
    
        if(IsIgnoredOpcode(curc))
            return true;

        packet->reset(curs,curc);
        packet->set(recvbuff);
        return true;
    }
    else
    {
        datalength = curs - loaded;
        if (!recv_packet(recvbuff,&datalength))
            return false;

        packet->set(recvbuff,loaded);

        if(datalength < curs- loaded)
        {// still not whole packet
            loaded += datalength;
            return true;
        }

        if(IsIgnoredOpcode(curc))
        {
            loaded = 0;
            return true;
        }
        packet->reset(curs,curc);
        return true;

    }
}

bool MainSocket::recv_auth_challenge(char buffer[BUFFER_SIZE_IN],uint16 datalength)
{
    if( (uint8)buffer[2] != 0xEC || (uint8)buffer[3] != 0x01 || datalength != 8)
        return false;
    serverSeed = MAKE_UINT32(buffer[7],buffer[6],buffer[5],buffer[4]);
    return true;
}

bool MainSocket::send_auth_session()
{
    uint8 lbuf[BUFFER_SIZE_OUT];
    uint8 i;
    
    // |Size (2)|Cmd (4)|Build (4)|unk (4)|username string(any)|0x00|seed (4)|digest (20)|
    lbuf[1] = (uint8)(username.length()+37); lbuf[0]=0x00 ; //size send in header is counted without uint16 size
    lbuf[2] = 0xED; lbuf[3] = 0x01; lbuf[4] = 0x00; lbuf[5] = 0x00;
    lbuf[6] = 0x9E; lbuf[7] = 0x21; lbuf[8] = 0x00; lbuf[9] = 0x00;

    for(i = 0; i <username.length();i++)
        lbuf[i+14] = (uint8)(username.c_str()[i]);
    i += 19;
    lbuf[i-5] = 0x00;
    lbuf[i-4] = 0x4D;lbuf[i-3] = 0xE6;lbuf[i-2] = 0x40;lbuf[i-1] = 0xBB;
    clientSeed = 0xBB40E64D;

    // begining of little magic
    Sha1Hash sha;
    uint32 t = 0;
    uint32 seed = serverSeed;
    sha.UpdateData(username);
    sha.UpdateData((uint8 *) & t, 4);
    sha.UpdateData((uint8 *) & clientSeed, 4);
    sha.UpdateData((uint8 *) & seed, 4);
    sha.UpdateBigNumbers(&K, NULL);
    sha.Finalize();
    // end of little magic
    for(i = 0;i<20;i++)
    lbuf[i+19+username.length()] = sha.GetDigest()[i];
    send_packet((char*)lbuf,username.length()+39);
    IsAuthed = true;
    return true;
}

bool MainSocket::send_out_pack(out_pack* packet)
{
    uint8 buffer[BUFFER_SIZE_OUT];

    buffer[0] = 0x00; buffer[4] = 0x00; buffer[5] = 0x00;
    buffer[1] = (uint8)(packet->gs() + 4);//size send in header is counted without uint16 size
    buffer[2] = (uint8)(packet->gc() % 256);
    buffer[3] = (uint8)((packet->gc() - buffer[2])/256);

    for (uint8 i=0;i<packet->gs();i++)
        buffer[i+6] = packet->gd(i);

    encrypt_header(buffer);
    return send_packet((char*) buffer,(uint8)(packet->gs() + 6));
}

bool MainSocket::IsIgnoredOpcode(uint16 opcode)
{
    switch(opcode)
    {
    case 0x0042: //SMSG_LOGIN_SETTIMESPEED
    case 0x0083: //SMSG_GUILD_INVITE
    case 0x00A9: //SMSG_UPDATE_OBJECT
    case 0x00AA: //SMSG_DESTROY_OBJECT
    case 0x00B3: //SMSG_GAMEOBJECT_CUSTOM_ANIM
    case 0x00B5: //MSG_MOVE_START_FORWARD
    case 0x00B6: //MSG_MOVE_START_BACKWARD
    case 0x00B7: //MSG_MOVE_STOP
    case 0x00B8: //MSG_MOVE_START_STRAFE_LEFT
    case 0x00B9: //MSG_MOVE_START_STRAFE_RIGHT
    case 0x00BA: //MSG_MOVE_STOP_STRAFE
    case 0x00BB: //MSG_MOVE_JUMP
    case 0x00BC: //MSG_MOVE_START_TURN_LEFT
    case 0x00BD: //MSG_MOVE_START_TURN_RIGHT
    case 0x00BE: //MSG_MOVE_STOP_TURN
    case 0x00BF: //MSG_MOVE_START_PITCH_UP
    case 0x00C0: //MSG_MOVE_START_PITCH_DOWN
    case 0x00C1: //MSG_MOVE_STOP_PITCH
    case 0x00C2: //MSG_MOVE_SET_RUN_MODE
    case 0x00C3: //MSG_MOVE_SET_WALK_MODE
    case 0x00C9: //MSG_MOVE_FALL_LAND
    case 0x00CA: //MSG_MOVE_START_SWIM
    case 0x00DA: //MSG_MOVE_SET_FACING
    case 0x00DB: //MSG_MOVE_SET_PITCH
    case 0x00DD: //SMSG_MONSTER_MOVE
    case 0x00E2: //SMSG_FORCE_RUN_SPEED_CHANGE
    case 0x00E6: //SMSG_FORCE_SWIM_SPEED_CHANGE
    case 0x00E8: //SMSG_FORCE_MOVE_ROOT
    case 0x00EE: //MSG_MOVE_HEARTBEAT
    case 0x00EA: //SMSG_FORCE_MOVE_UNROOT
    case 0x00FD: //SMSG_TUTORIAL_FLAGS
    case 0x0122: //SMSG_INITIALIZE_FACTIONS
    case 0x0127: //SMSG_SET_PROFICIENCY
    case 0x0129: //SMSG_ACTION_BUTTONS
    case 0x012A: //SMSG_INITIAL_SPELLS
    case 0x0131: //SMSG_SPELL_START
    case 0x0132: //SMSG_SPELL_GO
    case 0x0133: //SMSG_SPELL_FAILURE
    case 0x0137: //SMSG_UPDATE_AURA_DURATION
    case 0x013C: //SMSG_AI_REACTION
    case 0x0143: //SMSG_ATTACKSTART
    case 0x0144: //SMSG_ATTACKSTOP
    case 0x014A: //SMSG_ATTACKERSTATEUPDATE
    case 0x014E: //SMSG_CANCEL_COMBAT
    case 0x0150: //SMSG_SPELLHEALLOG
    case 0x0151: //SMSG_SPELLENERGIZELOG
    case 0x0155: //SMSG_BINDPOINTUPDATE
    case 0x0182: //SMSG_MOUNTSPECIAL_ANIM
    case 0x0179: //SMSG_PET_SPELLS
    case 0x01EA: //SMSG_ITEM_TIME_UPDATE
    case 0x01F6: //SMSG_COMPRESSED_UPDATE_OBJECT
    case 0x0209: //SMSG_ACCOUNT_DATA_TIMES
    case 0x0214: //SMSG_GAMEOBJECT_SPAWN_ANIM_OBSOLETE
    case 0x0215: //SMSG_GAMEOBJECT_DESPAWN_ANIM
    case 0x021E: //SMSG_SET_REST_START
    case 0x024C: //SMSG_SPELLLOGEXECUTE
    case 0x024E: //SMSG_PERIODICAURALOG
    case 0x0250: //SMSG_SPELLNONMELEEDAMAGELOG
    case 0x0266: //SMSG_SET_FLAT_SPELL_MODIFIER
    case 0x0267: //SMSG_SET_PCT_SPELL_MODIFIER
    case 0x02A6: //SMSG_SPELL_FAILED_OTHER
    case 0x02C2: //SMSG_INIT_WORLD_STATES
    case 0x02DA: //SMSG_FORCE_WALK_SPEED_CHANGE
    case 0x02F4: //SMSG_WEATHER
    case 0x0329: //MSG_SET_DUNGEON_DIFFICULTY
    case 0x030D: //SMSG_SPLINE_MOVE_SET_RUN_MODE
    case 0x030E: //SMSG_SPLINE_MOVE_SET_WALK_MODE
    case 0x0359: //MSG_MOVE_START_ASCEND
    case 0x0381: //SMSG_FORCE_FLIGHT_SPEED_CHANGE
    case 0x0390: //SMSG_TIME_SYNC_REQ
    case 0x03A4: //SMSG_SET_EXTRA_AURA_INFO
    case 0x03A5: //SMSG_SET_EXTRA_AURA_INFO_NEED_UPDATE
    case 0x03C8: //SMSG_FEATURE_SYSTEM_STATUS
    case 0x041D: //SMSG_SEND_UNLEARN_SPELLS
        return true;
    default:
        return false;
    }
}
