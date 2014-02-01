#ifndef MAIN_SOCKET_H
#define MAIN_SOCKET_H

#include "BaseSocket.h"
#include "iopackets.h"
#include "Auth\AuthCrypt.h"
#include "Auth\Sha1.h"

class MainSocket : public BaseSocket
{
public:
    void Update(inc_pack* packet,std::string* retstr);
    void SetKey(uint8 * key) {K.SetBinary(key,40);};
    void send_out_pack(out_pack* packet);
    MainSocket();
    
    std::string m_username;
private:
    void encrypt_header(uint8* header) {m_crypt.EncryptSend(header,6);};
    void decrypt_header(uint8* header) {m_crypt.DecryptRecv(header,4);};
    void recv_auth_challenge(char buffer[BUFFER_SIZE_IN],uint16 datalength);
    void send_auth_session();
    bool IsIgnoredOpcode(uint16 opcode);

    AuthCrypt   m_crypt;
    BigNumber   K;
    uint32      serverSeed,clientSeed;
    uint16      curs,curc,loaded;
    char        header[4];
    uint8       header_loaded;
    
    char recvbuff[BUFFER_SIZE_IN];
};
#endif