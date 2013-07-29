#include "Session.h"

bool Session::handle_smsg_auth_response(inc_pack* InPack,out_pack* OuPack)
{
    uint8 error;
    *InPack >> error;
    if (error != 0x0C)
    {
        printf("auth response received error: %u\n",error);
        return false;
    }
    printf("authorization succesful\n");
    return true;
}

bool Session::handle_smsg_char_enum(inc_pack* InPack,out_pack* OuPack)
{
    // data: |number of characters|char 1|char2|...
    // character struct: |guid(8)|name|0|race(1)|class(1)|gender(1)|look(5)|level(1)|zone(4)|map(4)|xyz floats(12)|
    // |guild(4)|flags(4)|1|petdisplay(4)|petlvl(4)|petfamily(4)|items (20*9)| = 239 + name
    uint16 point = 1;
    uint8  nofchars;

    *InPack >> nofchars;
    printf("received characters list, %u characters\n",nofchars);
    for (uint8 i=0;i<nofchars;i++)
    {
        *InPack >> characters[i].guid;
        InPack->skip(4);
        *InPack >> characters[i].name;
        *InPack >> characters[i].race >> characters[i].clas >> characters[i].gender;
        InPack->skip(5);
        *InPack >> characters[i].level;
        InPack->skip(29); InPack->skip(12);//pet data
        InPack->skip(20*9);//items
        printf("%s (guid %u): lvl %u %s %s %s\n",characters[i].name.c_str(),characters[i].guid,characters[i].level,
            (characters[i].gender ? "female": "male"),CharacterRaces[characters[i].race],CharacterClasses[characters[i].clas]);
        PlayersInfoMap[characters[i].guid].name = characters[i].name;
    }
    printf("select character: ");
    cinredirect = 1;
    return true;
}

bool Session::send_cmsg_login(out_pack* OuPack,uint8 i)
{
    i -= 48;
    cinredirect = 0;
    if (i>7 || characters[--i].guid == 0)
        i = 0;
    OuPack->reset(0x003D);
    *OuPack << characters[i].guid;
    *OuPack << (uint32)0;
    return true;
}

bool Session::handle_smsg_login_verify_world(inc_pack* InPack,out_pack* OuPack)
{
    printf("player logged in\n\n");
    return send_cmsg_join_channel(OuPack,"world");
    return true;
}

bool Session::send_cmsg_char_enum(out_pack* OuPack)
{
    printf("requesting character list\n");
    OuPack->reset( 0x0037);
    return true;
}