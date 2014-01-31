#include "LoginModule.h"
#include "Util.h"
#include "Database.h"

void LoginModule::Handle(inc_pack* InPack)
{
    switch(InPack->gc())
    {
    case 0x003B: handle_smsg_char_enum(InPack);break;                   //SMSG_CHAR_ENUM
    case 0x01EE: handle_smsg_auth_response(InPack); break;              //SMSG_AUTH_RESPONSE
    case 0x0236: handle_smsg_login_verify_world(InPack); break;         //SMSG_LOGIN_VERIFY_WORLD
    default:
        break;
    }
}

void LoginModule::handle_smsg_auth_response(inc_pack* InPack)
{
    uint8 error;
    *InPack >> error;
    if (error != 0x0C)
        throw "auth response received error\r\n";
    print("authorization succesful\r\n");
    send_cmsg_char_enum();
}

void LoginModule::handle_smsg_char_enum(inc_pack* InPack)
{
    // data: |number of characters|char 1|char2|...
    // character struct: |guid(8)|name|0|race(1)|class(1)|gender(1)|look(5)|level(1)|zone(4)|map(4)|xyz floats(12)|
    // |guild(4)|flags(4)|1|petdisplay(4)|petlvl(4)|petfamily(4)|items (20*9)| = 239 + name
    uint16 point = 1;
    uint8  nofchars;

    *InPack >> nofchars;
    print(string_format("received characters list, %u characters\r\n",nofchars));
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
        print(string_format("%s (guid %u): %u %s %s %s \r\n",characters[i].name.c_str(), characters[i].guid,
            characters[i].level,(characters[i].gender ? " female ": " male "),CharacterRaces[characters[i].race],
            CharacterClasses[characters[i].clas]));
        //PlayersInfoMap[characters[i].guid].name = characters[i].name;
    }
    print("select character: ");
}

void LoginModule::handle_smsg_login_verify_world(inc_pack* InPack)
{
    print("player logged in\r\n\r\n");
}

void LoginModule::send_cmsg_char_enum()
{
    print("requesting character list\r\n");
    OuPack.reset( 0x0037);
    send_out_pack();
}

void LoginModule::send_cmsg_login(uint8 i)
{
    i -= 48;
    if (i>7 || characters[--i].guid == 0)
        i = 0;
    OuPack.reset(0x003D);
    OuPack << characters[i].guid;
    OuPack << (uint32)0;
    send_out_pack();
    sDB->ishordeplayer = (1 << characters[i].race) & 1380? true : false;
}
