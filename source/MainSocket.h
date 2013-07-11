#include "BaseSocket.h"
#include "Auth\AuthCrypt.h"
#include "Auth\Sha1.h"

class MainSocket : public BaseSocket
{
public:
    bool Update();
    void SetKey(uint8 * key) {K.SetBinary(key,40);};

private:
    void prepare_packet(uint8* packet) {m_crypt.EncryptSend(packet,6);};
    bool recv_auth_challenge(char buffer[BUFFER_SIZE],uint8 datalength);
    bool send_auth_session();

    AuthCrypt m_crypt;
    BigNumber K;
    uint32 serverSeed,clientSeed;
};