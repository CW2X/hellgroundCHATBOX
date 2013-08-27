#include "Session.h"
#include "Util.h"

void Session::handle_smsg_auth_response(inc_pack* InPack)
{
    uint8 error;
    *InPack >> error;
    if (error != 0x0C)
        throw string_format("auth response received error: %u\n",error);
    printf("authorization succesful\n");
    send_cmsg_char_enum();
}

void Session::handle_smsg_char_enum(inc_pack* InPack)
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
}

void Session::send_cmsg_login(uint8 i)
{
    i -= 48;
    cinredirect = 0;
    if (i>7 || characters[--i].guid == 0)
        i = 0;
    OuPack.reset(0x003D);
    OuPack << characters[i].guid;
    OuPack << (uint32)0;
    sMainSocket.send_out_pack(&OuPack);
    switch (characters[i].race)
    {
    case 1: case 3: case 4: case 7: case 11:
        ishordeplayer = false;
        break;
    case 2: case 5: case 6: case 8: case 10:
        ishordeplayer = true;
        break;
    }
}

void Session::handle_smsg_login_verify_world(inc_pack* InPack)
{
    printf("player logged in\n\n");
    send_cmsg_join_channel("world");
}

void Session::send_cmsg_char_enum()
{
    printf("requesting character list\n");
    OuPack.reset( 0x0037);
    sMainSocket.send_out_pack(&OuPack);
}