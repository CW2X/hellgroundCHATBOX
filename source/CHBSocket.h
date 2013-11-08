#include "MainSocket.h"
#include "AuthProcessor.h"
#include "CSN.h"

class CHBSocket
{
public:
    void Initialize(std::string login,std::string password,std::string address);
    uint8 Update(inc_pack* InPacket,std::string* retstr);
    void send_out_pack(out_pack* packet);
    
    static CHBSocket& getInstance()
    {
        static CHBSocket instance;
        return instance;
    }
private:
    CHBSocket();
        
    MainSocket    sMainSocket;
    AuthProcessor sAProcessor;
};

void CSN::Initialize(std::string login,std::string password,std::string address) {return CHBSocket::getInstance().Initialize(login,password,address);};
uint8 CSN::Update(inc_pack* InPacket,std::string* retstr) {return CHBSocket::getInstance().Update(InPacket,retstr);};
void CSN::send_out_pack(out_pack* OuPack) {return CHBSocket::getInstance().send_out_pack(OuPack);};
