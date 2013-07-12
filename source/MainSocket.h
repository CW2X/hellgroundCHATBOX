#include "BaseSocket.h"
#include "Auth\AuthCrypt.h"
#include "Auth\Sha1.h"

class MainSocket : public BaseSocket
{
public:
    bool Update(inc_pack* packet);
    void SetKey(uint8 * key) {K.SetBinary(key,40);};
    bool send_out_pack(out_pack* packet);
private:
    void encrypt_header(uint8* header) {m_crypt.EncryptSend(header,6);};
    void decrypt_header(uint8* header) {m_crypt.DecryptRecv(header,4);};
    bool recv_auth_challenge(char buffer[BUFFER_SIZE],uint8 datalength);
    bool send_auth_session();

    AuthCrypt m_crypt;
    BigNumber K;
    uint32 serverSeed,clientSeed;
};