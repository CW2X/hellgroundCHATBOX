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

bool Session::handle_char_enum(inc_pack* InPack)
{
    // data: |number of characters|char 1|char2|...
    // character struct: |guid(8)|name|0|race(1)|class(1)|gender(1)|look(5)|level(1)|zone(4)|map(4)|xyz floats(12)|
    // |guild(4)|flags(4)|1|petdisplay(4)|petlvl(4)|petfamily(4)|items (20*9)| = 239 + name
    uint16 point = 1;
    
    printf("received characters list, %u characters\n",InPack->data[0]);
    for (uint8 i=0;i<InPack->data[0];i++)
    {
        characters[i].guid = MAKE_UINT32(InPack->data[point+3],InPack->data[point+2],InPack->data[point+1],InPack->data[point]);
        point += 8;
        while (InPack->data[point])
            characters[i].name.append(1,(char)InPack->data[point++]);
        characters[i].race = InPack->data[point+1];
        characters[i].clas = InPack->data[point+2];
        characters[i].gender = InPack->data[point+3];
        characters[i].level = InPack->data[point+9];
        point += 231;
        printf("%s (guid %u): lvl %u %s %s %s\n",characters[i].name.c_str(),characters[i].guid,characters[i].level,
            (characters[i].gender ? "female": "male"),CharacterRaces[characters[i].race],CharacterClasses[characters[i].clas]);
    }
    return false;
}