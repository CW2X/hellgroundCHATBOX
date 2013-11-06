#include "MainSocket.h"
#include "AuthProcessor.h"

#ifdef THIS_IS_DLL
#define DLL_API _declspec(dllexport)
#else
#define DLL_API _declspec(dllimport)
#endif

namespace CSN
{
    class CHBSocket
    {
    public:
        DLL_API bool Update(inc_pack* InPacket,std::string* retstr);
        DLL_API bool AllReady();
        DLL_API void send_out_pack(out_pack* packet);
    
        DLL_API static CHBSocket& getInstance()
        {
            static CHBSocket instance;
            return instance;
        }
    private:
        CHBSocket();

        MainSocket    sMainSocket;
        AuthProcessor sAProcessor;
    };
}
#define sSocket CSN::CHBSocket::getInstance()