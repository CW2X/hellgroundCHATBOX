#include "MainSocket.h"

bool MainSocket::Update(inc_pack* packet)
{
    char recvbuff[BUFFER_SIZE_IN];
    uint16 datalength;
    packet->cmd = (uint16)0;

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
    datalength = 4;
    if (!recv_packet(recvbuff,&datalength))
        return false;
    if (datalength == 0)
        return true;
    decrypt_header((uint8*)recvbuff);

    packet->size = MAKE_UINT16(recvbuff[0],recvbuff[1]) - 2;
    packet->cmd  = MAKE_UINT16(recvbuff[3],recvbuff[2]);
    datalength = packet->size;
    if (!recv_packet(recvbuff,&datalength))
        return false;

    if (datalength != packet->size)
    {
        printf("wrong packet size, recived %u, size in header %u, opcode 0x%04X\n",datalength,packet->size,packet->cmd);
        return false;
    }
    
    if(packet->size > BUFFER_SIZE_IN)
    {
        printf("possible Buffer Overflow! Interrupting (%u 0x%04X)",packet->size,packet->cmd);
        return false;
    }

    if(IsIgnoredOpcode(packet->cmd))
    {
        packet->cmd = 0x0000;
        packet->size = 0x0000;
        return true;
    }
    
    for (uint16 i=0;i<packet->size;i++)
        packet->data[i]=recvbuff[i];
    return true;
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
    uint8 lbuf[BUFFER_SIZE_IN];
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
    uint8 buffer[BUFFER_SIZE_IN];

    buffer[0] = 0x00; buffer[4] = 0x00; buffer[5] = 0x00;
    buffer[1] = (uint8)(packet->size + 4);//size send in header is counted without uint16 size
    buffer[2] = (uint8)(packet->cmd % 256);
    buffer[3] = (uint8)((packet->cmd - buffer[2])/256);

    for (uint8 i=0;i<packet->size;i++)
        buffer[i+6] = packet->data[i];

    encrypt_header(buffer);
    return send_packet((char*) buffer,(uint8)(packet->size + 6));
}

bool MainSocket::IsIgnoredOpcode(uint16 opcode)
{
    switch(opcode)
    {
    case 0x0042: //SMSG_LOGIN_SETTIMESPEED
    case 0x00A9: //SMSG_UPDATE_OBJECT
    case 0x00AA: //SMSG_DESTROY_OBJECT
    case 0x00DD: //SMSG_MONSTER_MOVE
    case 0x00E2: //SMSG_FORCE_RUN_SPEED_CHANGE
    case 0x00E6: //SMSG_FORCE_SWIM_SPEED_CHANGE
    case 0x00E8: //SMSG_FORCE_MOVE_ROOT
    case 0x00EA: //SMSG_FORCE_MOVE_UNROOT
    case 0x00FD: //SMSG_TUTORIAL_FLAGS
    case 0x0122: //SMSG_INITIALIZE_FACTIONS
    case 0x0127: //SMSG_SET_PROFICIENCY
    case 0x0129: //SMSG_ACTION_BUTTONS
    case 0x012A: //SMSG_INITIAL_SPELLS
    case 0x0131: //SMSG_SPELL_START
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
    case 0x0250: //SMSG_SPELLNONMELEEDAMAGELOG
    case 0x02C2: //SMSG_INIT_WORLD_STATES
    case 0x02F4: //SMSG_WEATHER
    case 0x0329: //MSG_SET_DUNGEON_DIFFICULTY
    case 0x030D: //SMSG_SPLINE_MOVE_SET_RUN_MODE
    case 0x0381: //SMSG_FORCE_FLIGHT_SPEED_CHANGE
    case 0x0390: //SMSG_TIME_SYNC_REQ
    case 0x03A4: //SMSG_SET_EXTRA_AURA_INFO
    case 0x03C8: //SMSG_FEATURE_SYSTEM_STATUS
    case 0x041D: //SMSG_SEND_UNLEARN_SPELLS
        return true;
    default:
        return false;
    }
}
