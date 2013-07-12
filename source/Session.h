#include "base_defs.h"

struct CharacterData
{
    uint32      guid;
    std::string name;
    uint8       race;
    uint8       clas;
    uint8       gender;
    uint8       level;
};

static char* CharacterClasses[]   = {"","Warrior","Paladin","Hunter","Rogue","Priest","DeKacz","Shaman","Mage","Warlock","Monkacz?","Druid"};
static char* CharacterRaces[]     = {"","Human","Orc","Dwarf","Night Elf","Undead","Tauren","Gnome","Troll","Goblin","Blood Elf","Draenei"};

class Session
{
public:
    bool Update(inc_pack* InPack,out_pack* OuPack);
    
    CharacterData characters[8];

private:
    bool handle_char_enum(inc_pack* InPack,out_pack* OuPack);
    bool handle_auth_response(inc_pack* InPack,out_pack* OuPack);
    bool send_cmsg_login(out_pack* OuPack);
};