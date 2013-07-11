#include "MainSocket.h"

bool MainSocket::Update()
{
    if(!IsConnected)
    {
        m_crypt.SetKey(&K);
        m_crypt.Init();
        return open_socket();
    }

    return send_auth();
}

bool MainSocket::send_auth()
{
    char lbuf[BUFFER_SIZE];
    uint8 i;
    std::string username = MAIN_LOGIN;

    lbuf[2] = 0xED; lbuf[3] = 0x01; lbuf[4] = 0x00; lbuf[5] = 0x00;
    lbuf[6] = 0x9E; lbuf[3] = 0x21; lbuf[4] = 0x00; lbuf[5] = 0x00;

    for(i = 0; i <username.length();i++)
        lbuf[i+16] = (uint8)(username.c_str()[i]);
    i += 17;
    lbuf[i-1] = 0x00;
    clientSeed = 0;
    for(;i<username.length()+21;i++)
    {
        lbuf[i] = (uint8)rand();
        clientSeed *= 256;
        clientSeed +=lbuf[i];
    }
    printf("%u",clientSeed);
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
    for(;i<username.length()+41;i++)
        lbuf[i] = sha.GetDigest()[i-41];
    send_packet(lbuf,10);
    return false;
}