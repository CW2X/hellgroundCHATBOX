#include "BaseSocket.h"
#include "iopackets.h"
#include "Auth\AuthCrypt.h"
#include "Auth\Sha1.h"

class MainSocket : public BaseSocket
{
public:
    MainSocket();
    bool Update(inc_pack* packet);
    void SetKey(uint8 * key) {K.SetBinary(key,40);};
    bool send_out_pack(out_pack* packet);

    std::string username;
private:
    void encrypt_header(uint8* header) {m_crypt.EncryptSend(header,6);};
    void decrypt_header(uint8* header) {m_crypt.DecryptRecv(header,4);};
    bool recv_auth_challenge(char buffer[BUFFER_SIZE_IN],uint16 datalength);
    bool send_auth_session();
    bool IsIgnoredOpcode(uint16 opcode);

    AuthCrypt   m_crypt;
    BigNumber   K;
    uint32      serverSeed,clientSeed;
    uint16      curs,curc,loaded;
};