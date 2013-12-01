#include "iopackets.h"

namespace CSN
{
    enum //flags returned by Update
    {
        CSS_NONE = 0x00,            // Empty flag
        CSS_INFO = 0x01,            // retstr has information to be printed
        CSS_READY = 0x02,           // client is authed and connected to realm

        CSS_ERROR = 0x80            // error, CHBSocket is no longer operational, retstr set to error name
    };

    using namespace System::Runtime::InteropServices;
    [DllImport("CHBSocket.dll", CharSet = CharSet::Auto, SetLastError = true)]
    void Initialize(std::string login, std::string password, std::string address);

    [DllImport("CHBSocket.dll", CharSet = CharSet::Auto, SetLastError = true)]
    uint8 Update(inc_pack* InPacket, std::string* retstr);

    [DllImport("CHBSocket.dll", CharSet = CharSet::Auto, SetLastError = true)]
    void send_out_pack(out_pack* packet);
};
