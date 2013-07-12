#include "Session.h"

bool Session::Update(inc_pack* InPack,out_pack* OuPack)
{
    
    switch(InPack->cmd)
    {
    case 0: break;
    case 0x003B: return handle_char_enum(InPack);
    case 0x01EE: return handle_auth_response(InPack,OuPack);
    default: break;
    }

    return true;
}

bool Session::handle_auth_response(inc_pack* InPack,out_pack* OuPack)
{
    OuPack->cmd  = 0x0037;
    OuPack->size = 0;
    send = true;
    return true;
}