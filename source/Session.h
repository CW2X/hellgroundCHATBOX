#include "base_defs.h"

class Session
{
public:
    bool Update(inc_pack* InPack,out_pack* OuPack);
    bool send;
    bool handle_char_enum(inc_pack* InPack) {return false;};
    bool handle_auth_response(inc_pack* InPack,out_pack* OuPack);
};