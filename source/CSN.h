#include "iopackets.h"

#ifdef THIS_IS_DLL
#define DLL_API _declspec(dllexport)
#else
#define DLL_API _declspec(dllimport)
#endif


namespace CSN
{
    enum //flags returned by Update
    {
        CSS_NONE = 0x00,            // Empty flag
        CSS_INFO = 0x01,            // retstr has information to be printed
        CSS_READY = 0x02,           // client is authed and connected to realm

        CSS_ERROR = 0x80            // error, CHBSocket is no longer operational, retstr set to error name
    };
    DLL_API void Initialize(std::string login,std::string password,std::string address);
    DLL_API uint8 Update(inc_pack* InPacket,std::string* retstr);
    DLL_API void send_out_pack(out_pack* packet);
};
