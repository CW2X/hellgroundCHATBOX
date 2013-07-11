#include "BaseSocket.h"
#include "Auth\AuthCrypt.h"
#include "Auth\Sha1.h"

class MainSocket : public BaseSocket
{
public:
    bool Update();
    void SetKey(uint8 * key) {K.SetBinary(key,40);};

    bool send_auth();
private:
    void prepare_packet(uint8* packet) {m_crypt.EncryptSend(packet,6);};

    AuthCrypt m_crypt;
    BigNumber K;
    bool isAuthed;
    uint32 serverSeed,clientSeed;
};