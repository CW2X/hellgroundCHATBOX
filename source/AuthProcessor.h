#include "base_defs.h"
#include "Auth\BigNumber.h"
#include "Auth\bn.h"
#include "Auth\Sha1.h"

enum eAuthCmd
{
    CMD_AUTH_LOGON_CHALLENGE        = 0x00,
    CMD_AUTH_LOGON_PROOF            = 0x01,
    CMD_AUTH_RECONNECT_CHALLENGE    = 0x02,
    CMD_AUTH_RECONNECT_PROOF        = 0x03,
    CMD_REALM_LIST                  = 0x10,
    CMD_XFER_INITIATE               = 0x30,
    CMD_XFER_DATA                   = 0x31,
    CMD_XFER_ACCEPT                 = 0x32,
    CMD_XFER_RESUME                 = 0x33,
    CMD_XFER_CANCEL                 = 0x34
};

#pragma pack(push)
#pragma pack(1)
struct AUTH_LOGON_CHALLENGE_D
{
    uint8   cmd;
    uint8   error;
    uint16  size;
    uint32  gamename;
    uint8   version1;
    uint8   version2;
    uint8   version3;
    uint16  build;
    uint32  platform;
    uint32  os;
    uint32  country;
    uint32  timezone_bias;
    uint32  ip;
    uint8   I_len;
    uint8   I[32];
};

struct AUTH_LOGON_PROOF_D
{
    uint8   cmd;
    uint8   A[32];
    uint8   M1[20];
    uint8   crc_hash[20];
    uint8   number_of_keys;
    uint8   securityFlags;
};
#pragma pack(pop)

union AUTH_LOGON_CHALLENGE_U { AUTH_LOGON_CHALLENGE_D data;char c [66];};
union AUTH_LOGON_PROOF_U { AUTH_LOGON_PROOF_D data;char c [75];};

class AuthProcessor
{
public:
    bool handle_incoming(char buffer[BUFFER_SIZE],uint8 datalength);
    
    void MagicVoid(const std::string& rI);

    bool send_logon_challenge();
    bool send_logon_proof();
    bool send_realm_list();
    //more SEND
    bool recv_logon_challenge(char buffer[BUFFER_SIZE],uint8 datalength);
    bool recv_logon_proof(char buffer[BUFFER_SIZE],uint8 datalength);
    //more RECV
private:
    BigNumber A,B,a,g,N,K,s,unk3,v,x,M,M2; // magic variables
};

