#include "AuthProcessor.h"
#include <iostream>

AuthProcessor::AuthProcessor()
{
    ServerAdress = REALMLIST_ADDRESS;
    ServerPort = AUTH_PORT;
}

void AuthProcessor::handle_incoming(char buffer[BUFFER_SIZE_IN],uint8 datalength)
{
    switch(buffer[0])
    {
    case CMD_AUTH_LOGON_CHALLENGE:
        {
            recv_logon_challenge(buffer,datalength);
            return send_logon_proof();
        }
    case CMD_AUTH_LOGON_PROOF:
        {
            recv_logon_proof(buffer,datalength);
            return send_realm_list();
        }
    case CMD_REALM_LIST:
        {
            return recv_realm_list(buffer,datalength);
        }
    default:
        {
            for (uint8 i=0;i<datalength;i++)
            {
                printf("%u ",(uint8)buffer[i]);
            }
            printf("|EOT|\n");
            throw string_format("unknown command: %u \n",(uint8)buffer[0]);
        }

    }
}

void AuthProcessor::MagicVoid()
{
    Sha1Hash sha;
    sha.UpdateData(username);
    sha.UpdateData(":");
    sha.UpdateData(password);
    sha.Finalize();

    BigNumber I;
    I.SetBinary(sha.GetDigest(),sha.GetLength());
    
    sha.Initialize();
    sha.UpdateData(s.AsByteArray(), s.GetNumBytes());
    sha.UpdateData(I.AsByteArray(), I.GetNumBytes());
    sha.Finalize();
    x.SetBinary(sha.GetDigest(), sha.GetLength());
    v = g.ModExp(x, N);

    a.SetRand(8 * 19);
    A = g.ModExp(a,N);
    
    sha.Initialize();
    sha.UpdateBigNumbers(&A,&B,NULL);
    sha.Finalize();
    BigNumber u;
    u.SetBinary(sha.GetDigest(), 20);
    BigNumber S = (B - (v*3)).ModExp((a + (u*x)),N);
    uint8 t[32];
    uint8 t1[16];
    uint8 vK[40];
    memcpy(t, S.AsByteArray(32), 32);
    for (int i = 0; i < 16; ++i)
    {
        t1[i] = t[i * 2];
    }
    sha.Initialize();
    sha.UpdateData(t1, 16);
    sha.Finalize();
    for (int i = 0; i < 20; ++i)
    {
        vK[i * 2] = sha.GetDigest()[i];
    }
    for (int i = 0; i < 16; ++i)
    {
        t1[i] = t[i * 2 + 1];
    }
    sha.Initialize();
    sha.UpdateData(t1, 16);
    sha.Finalize();
    for (int i = 0; i < 20; ++i)
    {
        vK[i * 2 + 1] = sha.GetDigest()[i];
    }
    K.SetBinary(vK, 40);

    uint8 hash[20];

    sha.Initialize();
    sha.UpdateBigNumbers(&N, NULL);
    sha.Finalize();
    memcpy(hash, sha.GetDigest(), 20);
    sha.Initialize();
    sha.UpdateBigNumbers(&g, NULL);
    sha.Finalize();
    for (int i = 0; i < 20; ++i)
    {
        hash[i] ^= sha.GetDigest()[i];
    }
    BigNumber t3;
    t3.SetBinary(hash, 20);

    sha.Initialize();
    sha.UpdateData(username);
    sha.Finalize();
    uint8 t4[SHA_DIGEST_LENGTH];
    memcpy(t4, sha.GetDigest(), SHA_DIGEST_LENGTH);

    sha.Initialize();
    sha.UpdateBigNumbers(&t3, NULL);
    sha.UpdateData(t4, SHA_DIGEST_LENGTH);
    sha.UpdateBigNumbers(&s, &A, &B, &K, NULL);
    sha.Finalize();
    M.SetBinary(sha.GetDigest(), 20);

    sha.Initialize();
    sha.UpdateBigNumbers(&A,&M,&K,NULL);
    sha.Finalize();

    M2.SetBinary(sha.GetDigest(),20);
}

void AuthProcessor::send_logon_challenge()
{
    AUTH_LOGON_CHALLENGE_U sLC;
    printf("login(uppercase): ");
    std::cin >> username;

    sLC.data.cmd            = 0;
    sLC.data.error          = 0;
    sLC.data.size           = 30 + username.size();
    sLC.data.gamename       = (uint32)'WoW';
    sLC.data.version1       = 2;
    sLC.data.version2       = 4;
    sLC.data.version3       = 3;
    sLC.data.build          = 8606;
    sLC.data.platform       = (uint32)'x86';
    sLC.data.os             = (uint32)FAKE_OS;
    sLC.data.country        = (uint32)'enGB';
    sLC.data.timezone_bias  = 60; // why? why not.
    sLC.data.ip             = 0xF6876919;
    sLC.data.I_len          = username.size();
    for(uint8 i=0; i<username.size();i++)
        sLC.data.I[i]=(uint8)(username.c_str()[i]);

    send_packet(sLC.c,username.length()+34);
}

void AuthProcessor::recv_logon_challenge(char buffer[BUFFER_SIZE_IN],uint8 datalength)
{
    switch (buffer[2])
    {
    case 0x00:
        {
            if (buffer[35] != 1 || buffer[37] != 32 || datalength < 119)
                throw string_format("logon challenge: invalid response from server\n");
            
            uint8 local[32];
            for(int i = 0; i<32;i++)
                local[i] = (uint8)buffer[i+3];
            B.SetBinary(local,32);
            local[0] = buffer[36];
            g.SetBinary(local,1); // always 7
            for(int i = 0; i<32;i++)
                local[i] = (uint8)buffer[i+38];
            N.SetBinary(local,32); // always 894B645E89E1535BBDAD5B8B290650530801B18EBFBF5E8FAB3C82872A3E9BB7
            for(int i = 0; i<32;i++)
                local[i] = (uint8)buffer[i+70];
            s.SetBinary(local,32);
            // unk3.setbinary ?
            return;
        }
    case 0x03:
        throw "account banned\n";
    case 0x04:
        throw "unknown account\n";
    default:
        throw string_format("other logon challenge error: %u\n",(uint8)buffer[2]);
    }
}

void AuthProcessor::send_logon_proof()
{
    AUTH_LOGON_PROOF_U sLP;
    printf("password(uppercase): ");
    std::cin >> password;
    // beggining of magic
    MagicVoid();
    // end of magic.

    sLP.data.cmd            = 1;
    for(uint8 i=0; i<32;i++)
        sLP.data.A[i] = A.AsByteArray()[i];
    for(uint8 i=0; i<20;i++)
        sLP.data.M1[i] =M.AsByteArray()[i];
    //sLP.data.crc_hash       = ; dunno what's that
    sLP.data.number_of_keys = 0;
    sLP.data.securityFlags  = 0;
    
    send_packet(sLP.c,sizeof(sLP));
}

void AuthProcessor::recv_logon_proof(char buffer[BUFFER_SIZE_IN],uint8 datalength)
{
    if(buffer[1] == 0x04)
        printf("invalid password\n");
    else if(buffer[1] == 0x00)
    {
        uint16 sum = 0 ;
        for (uint8 i=22;i<32;i++)
            sum += (uint8)buffer[i];
        if((uint8)buffer[24] != 128 || sum != 128)
            throw string_format("logon proof: invalid response from server %u %u\n",sum,(uint8)buffer[24]);
        
        for(uint8 i=0;i<20;i++)
            if ((uint8)buffer[i+2] != (uint8)M2.AsByteArray()[i])
                throw "logon proof: invalid M2 key\n";
        
        return;
    }
    else
        throw string_format("logon proof error: %u\n",buffer[1]);

}

void AuthProcessor::send_realm_list()
{
    char c[5] = {0x10,0x00,0x00,0x00,0x00};
    send_packet(c,5);
}

void AuthProcessor::recv_realm_list(char buffer[BUFFER_SIZE_IN],uint8 datalength)
{
    if((uint8)buffer[1] != datalength - 3)
        throw "realm list invalid packet size\n";
    if((uint8)buffer[2] || (uint8)buffer[3] || (uint8)buffer[4] || (uint8)buffer[5] || (uint8)buffer[6] || (uint8)buffer[datalength-1] || (uint8)buffer[datalength-2] != 0x10)
        throw "realm list: invalid response from server\n";

    realms = (uint8)buffer[7];
    printf("received %i realm info\n",realms);
    if (realms>4)
        throw "too many realms\n";
    uint8 pos = 9;
    for(uint8 i=0;i<realms;i++)
    {
        realmdata[i].icon = (uint8)buffer[pos];
        realmdata[i].lock = (uint8)buffer[pos+1];
        realmdata[i].flag = (uint8)buffer[pos+2];
        pos +=3;
        while ((uint8)buffer[pos])
        {
            realmdata[i].name.append(1,buffer[pos]);
            pos++;
        }
        pos++;
        while ((uint8)buffer[pos]!= 58)
        {
            realmdata[i].address.append(1,buffer[pos]);
            pos++;
        }
        pos++;
        while ((uint8)buffer[pos])
        {
            realmdata[i].port.append(1,buffer[pos]);
            pos++;
        }
        // realmdata[i].population = ? float on pos+1 ... pos+4
        realmdata[i].AmountOfCharacters = (uint8)buffer[pos+5];
        realmdata[i].timezone = (uint8)buffer[pos+6];
        pos += 8;
        if (realmdata[i].flag & 0x04)
        {
            realmdata[i].version1 = (uint8)buffer[pos];
            realmdata[i].version2 = (uint8)buffer[pos+1];
            realmdata[i].version3 = (uint8)buffer[pos+2];
            realmdata[i].build    = MAKE_UINT16(buffer[pos+4],buffer[pos+3]);
            pos += 5;
        }

        printf("%u | %u | %u | %s(%u.%u.%u %u) | %s:%s | %u\n",realmdata[i].icon,realmdata[i].lock,realmdata[i].flag,
            realmdata[i].name.c_str(),realmdata[i].version1,realmdata[i].version2,realmdata[i].version3,
            realmdata[i].build,realmdata[i].address.c_str(),realmdata[i].port.c_str(),realmdata[i].AmountOfCharacters);
    }
    if (realms > 0)
        IsAuthed = true;
}

void AuthProcessor::Update()
{
    if(!IsConnected)
    {
        open_socket();
        send_logon_challenge();
        return;
    }

    char recvbuff[BUFFER_SIZE_IN];
    uint16 datalength;
    datalength = 0;
    
    recv_packet(recvbuff,&datalength);
    if (datalength>0)
        handle_incoming(recvbuff,(uint8)datalength);
}