#include "Session.h"
#include "Zlib\zlib.h"

void Session::handle_smsg_compressed_update_object(inc_pack* InPack)
{
    int retu = uncompress_smsg_CUO(InPack);
    if (retu != Z_OK)
        {printf("SMSG_CUO decompress error: %i\n",retu);return;}
    handle_smsg_update_object(InPack);
}

int Session::uncompress_smsg_CUO(inc_pack* InPack)
{
    uint32 uncompressedSize;
    uint8 in[BUFFER_SIZE_IN];
    uint8 ou[BUFFER_SIZE_IN];
    *InPack >> uncompressedSize;
    for(uint16 i=0;i<InPack->gs() -4;i++)
        *InPack >> in[i];
    unsigned long oun = BUFFER_SIZE_IN;
    int retu=uncompress(ou,&oun,in,InPack->gs()-4);
    InPack->set((char*)ou);
    return retu;
}

void Session::handle_smsg_update_object(inc_pack* InPack)
{
}