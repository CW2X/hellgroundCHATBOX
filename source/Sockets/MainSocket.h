/* This source file is part of koostosh's chb project
see README for copyright notice */

#ifndef MAIN_SOCKET_H
#define MAIN_SOCKET_H

#include "BaseSocket.h"
#include "..\iopackets.h"

class MainSocket : public BaseSocket
{
public:
    void Update(inc_pack* packet,std::string* retstr);
    void SetKey(uint8 * key) {
        BigKey = new uint8[40];
        memcpy(BigKey, key, 40); };
    void send_out_pack(out_pack* packet);
    MainSocket();
    
    std::string m_username;
private:
    void generate_key();
    void encrypt_header(uint8* header);
    void decrypt_header(uint8* header);
    void recv_auth_challenge(char buffer[BUFFER_SIZE_IN],uint16 datalength);
    void send_auth_session();
    bool IsIgnoredOpcode(uint16 opcode);

    uint32      serverSeed,clientSeed;
    uint16      curs,curc,loaded;
    char        header[4];
    uint8       header_loaded;
    uint8       SmallKey[20];
    uint8*      BigKey;
    uint8       index_s, index_r, last_s, last_r;
    
    char recvbuff[BUFFER_SIZE_IN];
};
#endif