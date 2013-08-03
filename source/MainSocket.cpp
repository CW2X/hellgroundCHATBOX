#include "MainSocket.h"
#include "Util.h"

MainSocket::MainSocket()
{
    loaded  = 0;
    header_loaded = 0;
}

void MainSocket::Update(inc_pack* packet)
{
    char recvbuff[BUFFER_SIZE_IN];
    uint16 datalength;
    packet->reset();

    if(!IsConnected)
    {
        m_crypt.SetKey(&K);
        m_crypt.Init();
        open_socket();
    }
    if(!IsAuthed)
    {
        datalength = 0;
        recv_packet(recvbuff,&datalength);
        if (datalength>0)
        {
            recv_auth_challenge(recvbuff,datalength);
            send_auth_session();
        }
        return;
    }

    if (loaded == 0) // starting new packet
    {
        if (header_loaded == 0) //starting new header
        {
            datalength = 4;
            recv_packet(header,&datalength);
            if (datalength == 0)
                return;
            if (datalength != 4)
            {//not whole header!
                header_loaded = (uint8)datalength;
                return;
            }
        }
        else //last header was not fully received
        {
            char headertemp[4];
            datalength = 4 - header_loaded;
            recv_packet(headertemp,&datalength);

            for(uint8 i=0;i<datalength;i++)
                header[header_loaded+i] = headertemp[i];

            if (datalength != 4- header_loaded)
            {//still not whole
                header_loaded += datalength;
                return;
            }

            header_loaded = 0;
        }

        //we have good new header, lets process!
        decrypt_header((uint8*)header);

        curc = (uint16)(((uint8)header[3] << 8) | (uint8)header[2]);
        curs = (uint16)(((uint8)header[0] << 8) | (uint8)header[1]) - 2;
        
        if(curs > BUFFER_SIZE_IN)
            throw string_format("possible Buffer Overflow! Interrupting (%u 0x%04X)",curs,curc);

        datalength = curs;
        if (datalength)
            recv_packet(recvbuff,&datalength);

        if (datalength < curs)
        { // not whole packet received
            loaded = datalength;
            packet->set(recvbuff);
            return;
        }
    
        if(IsIgnoredOpcode(curc))
            return;

        packet->reset(curs,curc);
        packet->set(recvbuff);
    }
    else //not whole data was received last time
    {
        datalength = curs - loaded;
        recv_packet(recvbuff,&datalength);

        packet->set(recvbuff,loaded);

        if(datalength < curs- loaded)
        {// still not whole packet
            loaded += datalength;
            return;
        }

        loaded = 0;
        if(IsIgnoredOpcode(curc))
            return;
        packet->reset(curs,curc);
    }
}

void MainSocket::recv_auth_challenge(char buffer[BUFFER_SIZE_IN],uint16 datalength)
{
    if( (uint8)buffer[2] != 0xEC || (uint8)buffer[3] != 0x01 || datalength != 8)
        throw "wrong auth challenge received";
    serverSeed = uint32(((uint8)buffer[4]) | ((uint8)buffer[5] << 8) | ((uint8)buffer[6] << 16) | ((uint8)buffer[7] << 24));
}

void MainSocket::send_auth_session()
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
}

void MainSocket::send_out_pack(out_pack* packet)
{
    uint8 buffer[BUFFER_SIZE_OUT];

    buffer[0] = 0x00; buffer[4] = 0x00; buffer[5] = 0x00;
    buffer[1] = (uint8)(packet->gs() + 4);//size send in header is counted without uint16 size
    buffer[2] = (uint8)(packet->gc() % 256);
    buffer[3] = (uint8)((packet->gc() - buffer[2])/256);

    for (uint8 i=0;i<packet->gs();i++)
        buffer[i+6] = packet->gd(i);

    encrypt_header(buffer);
    send_packet((char*) buffer,(uint8)(packet->gs() + 6));
}

bool MainSocket::IsIgnoredOpcode(uint16 opcode)
{
    switch(opcode)
    {
    case 0x003B: //SMSG_CHAR_ENUM
    case 0x0051: //SMSG_NAME_QUERY_RESPONSE
    case 0x0096: //SMSG_MESSAGECHAT
    case 0x0099: //SMSG_CHANNEL_NOTIFY
    case 0x009B: //SMSG_CHANNEL_LIST
    case 0x01EE: //SMSG_AUTH_RESPONSE
    case 0x0236: //SMSG_LOGIN_VERIFY_WORLD
    case 0x03EF: //SMSG_USERLIST_ADD
    case 0x03F1: //SMSG_USERLIST_UPDATE
        //opcodes to be taken care of ... possibly
    case 0x0067: //SMSG_CONTACT_LIST
    case 0x0092: //SMSG_GUILD_EVENT
    case 0x0103: //SMSG_EMOTE
    case 0x0105: //SMSG_TEXT_EMOTE
    case 0x01CB: //SMSG_NOTIFICATION
    case 0x02A9: //SMSG_CHAT_PLAYER_NOT_FOUND
    case 0x033D: //SMSG_MOTD
    case 0x03F0: //SMSG_USERLIST_REMOVE
        return false;
    default:
        return true;
    }
}
