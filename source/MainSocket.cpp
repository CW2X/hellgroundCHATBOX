#include "MainSocket.h"

bool MainSocket::Update(inc_pack* packet)
{
    char recvbuff[BUFFER_SIZE];
    uint8 datalength;
    packet->size = (uint16)0;

    if(!IsConnected)
    {
        m_crypt.SetKey(&K);
        m_crypt.Init();
        return open_socket();
    }
    if(!IsAuthed)
    {
        if (!recv_packet(recvbuff,&datalength))
            return false;
        if (datalength>0 && recv_auth_challenge(recvbuff,datalength))
                return send_auth_session();
        return true;
    }
    if (!recv_packet(recvbuff,&datalength))
        return false;
    if (datalength == 0)
        return true;
    decrypt_header((uint8*)recvbuff);
    printf("decrypted data: \n");
    for (uint8 i=0;i<datalength;i++)
        printf("%u |",(uint8)recvbuff[i]);
    printf("EOT|\n");
    if (datalength != (uint8)recvbuff[1] +2)
    {
        printf("wrong packet size, recived %u, size in header %u",datalength,(uint8)recvbuff[1] +2);
        return false;
    }
    packet->size = (uint8)recvbuff[1] - 2;
    packet->cmd  = (uint8)recvbuff[2] + 256*(uint8)recvbuff[3];
    printf("size %u cmd 0x%04X\n",packet->size,packet->cmd);
    return true;
}

bool MainSocket::recv_auth_challenge(char buffer[BUFFER_SIZE],uint8 datalength)
{
    if( (uint8)buffer[2] != 0xEC || (uint8)buffer[3] != 0x01 || datalength != 8)
        return false;
    serverSeed = (uint8)buffer[4] + 256*((uint8)buffer[5] + 256*((uint8)buffer[6] + 256*(uint8)buffer[7]));
    return true;
}

bool MainSocket::send_auth_session()
{
    uint8 lbuf[BUFFER_SIZE];
    uint8 i;
    std::string username = MAIN_LOGIN;
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
    uint8 buffer[BUFFER_SIZE];

    buffer[0] = 0x00; buffer[4] = 0x00; buffer[5] = 0x00;
    buffer[1] = (uint8)(packet->size + 6);
    buffer[2] = (uint8)(packet->cmd % 256);
    buffer[3] = (uint8)((packet->cmd - buffer[2])/256);

    for (uint8 i=0;i<packet->size;i++)
        buffer[i+6] = packet->data[i];

    encrypt_header(buffer);
    return send_packet((char*) buffer,(uint8)(packet->size + 6));
}