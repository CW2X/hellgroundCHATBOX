/* This source file is part of koostosh's chb project
see README for copyright notice */

#ifndef AUTH_PROCESSOR_H
#define AUTH_PROCESSOR_H

#include "BaseSocket.h"

enum eAuthCmd
{
    CMD_AUTH_LOGON_CHALLENGE        = 0x00,
    CMD_AUTH_LOGON_PROOF            = 0x01,
    CMD_AUTH_RECONNECT_CHALLENGE    = 0x02,
    CMD_AUTH_RECONNECT_PROOF        = 0x03,
    CMD_REALM_LIST                  = 0x10,
    CMD_XFER_INITIATE               = 0x30,
    CMD_XFER_DATA                   = 0x31,
    CMD_XFER_ACCEPT                 = 0x32,
    CMD_XFER_RESUME                 = 0x33,
    CMD_XFER_CANCEL                 = 0x34
};

#pragma pack(push)
#pragma pack(1)
struct AUTH_LOGON_CHALLENGE_D
{
    uint8   cmd;
    uint8   error;
    uint16  size;
    uint32  gamename;
    uint8   version1;
    uint8   version2;
    uint8   version3;
    uint16  build;
    uint32  platform;
    uint32  os;
    uint32  country;
    uint32  timezone_bias;
    uint32  ip;
    uint8   I_len;
    uint8   I[32];
};

struct AUTH_LOGON_PROOF_D
{
    uint8   cmd;
    uint8   A[32];
    uint8   M1[20];
    uint8   crc_hash[20];
    uint8   number_of_keys;
    uint8   securityFlags;
};
#pragma pack(pop)

union AUTH_LOGON_CHALLENGE_U { AUTH_LOGON_CHALLENGE_D data;char c [66];};
union AUTH_LOGON_PROOF_U { AUTH_LOGON_PROOF_D data;char c [75];};

struct RealmInfo
{
    uint8 icon;
    uint8 lock;
    uint8 flag;
    std::string name;
    std::string address;
    std::string port;
    float population;
    uint8 AmountOfCharacters;
    uint8 timezone;
    uint8 version1;
    uint8 version2;
    uint8 version3;
    uint16 build;
};

class AuthProcessor : public BaseSocket
{
public:
    AuthProcessor();
    void Update(std::string* retstr);
    void Initialize(std::string username,std::string password);
    
    const char* GetRealmAdress(uint8 i) {return realmdata[i].address.c_str();};
    const char* GetRealmPort(uint8 i) {return realmdata[i].port.c_str();};
    uint8* GetKey() { return K; };
private:
    void MagicVoid();

    void handle_incoming(char buffer[BUFFER_SIZE_IN],uint8 datalength);
    void send_logon_challenge(); // initialize 
    void send_logon_proof();
    void send_realm_list();
    void recv_logon_challenge(char buffer[BUFFER_SIZE_IN],uint8 datalength);
    void recv_logon_proof(char buffer[BUFFER_SIZE_IN],uint8 datalength);
    void recv_realm_list(char buffer[BUFFER_SIZE_IN],uint8 datalength);
    
    uint8 A[32],B[32],K[40],M[20],M2[20],N[32],s[32],g;
    uint8 realms;
    RealmInfo realmdata[4];
    std::string m_username,m_password;
};

#endif