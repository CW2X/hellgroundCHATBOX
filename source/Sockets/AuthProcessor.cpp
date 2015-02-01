/* This source file is part of koostosh's chb project
see README for copyright notice */

#include "AuthProcessor.h"
#include "..\Util.h"
#include "MySha.h"
#include "MontgomeryExp.h"

AuthProcessor::AuthProcessor()
{
    PortString = AUTH_PORT;
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
            throw string_format("unknown command: %u \r\n",(uint8)buffer[0]);
        }

    }
}

void AuthProcessor::MagicVoid()
{
    MySha sha;
    MontgomeryExp mexp;
    uint8       I[20], x[20], a[20], v[32], t[32], u[20];;
    sha.UpdateData(m_username);
    sha.UpdateData(":");
    sha.UpdateData(m_password);
    sha.Finalize();
    memcpy(I, sha.GetDigest(), 20);

    sha.Initialize();
    sha.UpdateData(s,32);
    sha.UpdateData(I,20);
    sha.Finalize();
    memcpy(x, sha.GetDigest(), 20);

    uint32 randomSeed = getMsTime();
    sha.Initialize();
    sha.UpdateData((uint8*)&randomSeed, 4);
    sha.Finalize();
    memcpy(a, sha.GetDigest(), 20);

    mexp.init(N);
    mexp.ModExpSimple(v, g, x);
    mexp.ModExpSimple(A, g, a);

    sha.Initialize();
    sha.UpdateData(A,32);
    sha.UpdateData(B,32);
    sha.Finalize(); 
    memcpy(u, sha.GetDigest(), 20);

    mexp.ModExpFull(t, B, v, a, u, x);

    uint8 t1[16];
    for (int i = 0; i < 16; ++i)
    {
        t1[i] = t[i * 2];
    }
    sha.Initialize();
    sha.UpdateData(t1, 16);
    sha.Finalize();
    for (int i = 0; i < 20; ++i)
    {
        K[i * 2] = sha.GetDigest()[i];
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
        K[i * 2 + 1] = sha.GetDigest()[i];
    }

    sha.Initialize();
    sha.UpdateData(N,32);
    sha.Finalize();
    memcpy(u, sha.GetDigest(), 20);
    sha.Initialize();
    sha.UpdateData(&g,1);
    sha.Finalize();
    for (int i = 0; i < 20; ++i)
    {
        u[i] ^= sha.GetDigest()[i];
    }
    sha.Initialize();
    sha.UpdateData(m_username);
    sha.Finalize();
    memcpy(M, sha.GetDigest(), 20);

    sha.Initialize();
    sha.UpdateData(u,20);
    sha.UpdateData(M, 20);
    sha.UpdateData(s,32);
    sha.UpdateData(A,32);
    sha.UpdateData(B,32);
    sha.UpdateData(K,40);
    sha.Finalize();
    memcpy(M, sha.GetDigest(), 20);

    sha.Initialize();
    sha.UpdateData(A,32);
    sha.UpdateData(M,20);
    sha.UpdateData(K,40);
    sha.Finalize();

    memcpy(M2, sha.GetDigest(), 20);
}

void AuthProcessor::send_logon_challenge()
{
    AUTH_LOGON_CHALLENGE_U sLC;

    sLC.data.cmd            = 0;
    sLC.data.error          = 0;
    sLC.data.size           = 30 + m_username.size();
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
    sLC.data.I_len          = m_username.size();
    for(uint8 i=0; i<m_username.size();i++)
        sLC.data.I[i]=(uint8)(m_username.c_str()[i]);

    send_packet(sLC.c,m_username.length()+34);
}

void AuthProcessor::recv_logon_challenge(char buffer[BUFFER_SIZE_IN],uint8 datalength)
{
    switch (buffer[2])
    {
    case 0x00:
        {
            if (buffer[35] != 1 || buffer[37] != 32 || datalength < 119)
                throw string_format("logon challenge: invalid response from server\r\n");
            
            memcpy(B, buffer + 3, 32);
            g = buffer[36]; // always 7
            memcpy(N, buffer + 38, 32); // always 894B645E89E1535BBDAD5B8B290650530801B18EBFBF5E8FAB3C82872A3E9BB7
            memcpy(s,buffer+70,32);
            return;
        }
    case 0x03:
        throw "account banned\r\n";
    case 0x04:
        throw "unknown account\r\n";
    default:
        throw string_format("other logon challenge error: %u\r\n",(uint8)buffer[2]);
    }
}

void AuthProcessor::send_logon_proof()
{
    AUTH_LOGON_PROOF_U sLP;
    
    // beggining of magic
    MagicVoid();
    // end of magic.

    sLP.data.cmd            = 1;
    memcpy(sLP.data.A, A, 32);
    memcpy(sLP.data.M1, M, 20);
    //sLP.data.crc_hash       = ; dunno what's that
    sLP.data.number_of_keys = 0;
    sLP.data.securityFlags  = 0;
    
    send_packet(sLP.c,sizeof(sLP));
}

void AuthProcessor::recv_logon_proof(char buffer[BUFFER_SIZE_IN],uint8 datalength)
{
    if(buffer[1] == 0x04)
        throw("invalid password\r\n");
    else if(buffer[1] == 0x00)
    {
        uint16 sum = 0 ;
        for (uint8 i=22;i<32;i++)
            sum += (uint8)buffer[i];
        if((uint8)buffer[24] != 128 || sum != 128)
            throw string_format("logon proof: invalid response from server %u %u\r\n",sum,(uint8)buffer[24]);
        
        if (memcmp(buffer + 2, M2, 20))
            throw "logon proof: invalid M2 key\r\n";
        
        return;
    }
    else
        throw string_format("logon proof error: %u\r\n",buffer[1]);

}

void AuthProcessor::send_realm_list()
{
    char c[5] = {0x10,0x00,0x00,0x00,0x00};
    send_packet(c,5);
}

void AuthProcessor::recv_realm_list(char buffer[BUFFER_SIZE_IN],uint8 datalength)
{
    if((uint8)buffer[1] != datalength - 3)
        throw "realm list invalid packet size\r\n";
    if((uint8)buffer[2] || (uint8)buffer[3] || (uint8)buffer[4] || (uint8)buffer[5] || (uint8)buffer[6] || (uint8)buffer[datalength-1] || (uint8)buffer[datalength-2] != 0x10)
        throw "realm list: invalid response from server\r\n";

    realms = (uint8)buffer[7];
    m_ret += string_format("received %i realm info\r\n",realms);
    if (realms>4)
        throw "too many realms\r\n";
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
            realmdata[i].build    = uint16(((uint8)buffer[pos+3]) | ((uint8)buffer[pos+4] << 8));
            pos += 5;
        }

        m_ret += string_format("%u | %u | %u | %s(%u.%u.%u %u) | %s:%s | %u\r\n",realmdata[i].icon,realmdata[i].lock,realmdata[i].flag,
            realmdata[i].name.c_str(),realmdata[i].version1,realmdata[i].version2,realmdata[i].version3,
            realmdata[i].build,realmdata[i].address.c_str(),realmdata[i].port.c_str(),realmdata[i].AmountOfCharacters);
    }
    if (realms > 0)
        IsAuthed = true;
}

void AuthProcessor::Update(std::string* retstr)
{
    m_ret = "";
    if(!IsConnected)
    {
        open_socket();
        send_logon_challenge();
        *retstr += m_ret;
        return;
    }

    char recvbuff[BUFFER_SIZE_IN];
    uint16 datalength;
    datalength = 0;
    
    recv_packet(recvbuff,&datalength);
    if (datalength>0)
        handle_incoming(recvbuff,(uint8)datalength);
    *retstr += m_ret;
}

void AuthProcessor::Initialize(std::string username,std::string password)
{
    m_username = username;
    m_password = password;
    AddressString = SERVER_ADDRESS;
}
