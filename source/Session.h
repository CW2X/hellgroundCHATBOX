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

static char* CharacterClasses[]     = {"","Warrior","Paladin","Hunter","Rogue","Priest","DeKacz","Shaman","Mage","Warlock","Monkacz?","Druid"};
static char* CharacterRaces[]       = {"","Human","Orc","Dwarf","Night Elf","Undead","Tauren","Gnome","Troll","Goblin","Blood Elf","Draenei"};

struct ChatMessage
{
    uint8       type;
    uint32      lang;
    uint32      guid;
    uint8       tag;
    uint32      length;
    std::string channel;
    std::string what;
};

static char* ChatTagIdentifiers[]   = {"    ","AFK|","DND|","DND|","[GM]","[GM]","[GM]","[GM]"};

class Session
{
public:
    bool Update(inc_pack* InPack,out_pack* OuPack);
    
    CharacterData characters[8];

private:
    bool handle_char_enum(inc_pack* InPack,out_pack* OuPack);
    bool handle_chat_message(inc_pack* InPack,out_pack* OuPack);
    bool handle_auth_response(inc_pack* InPack,out_pack* OuPack);
    bool handle_login_verify(inc_pack* InPack,out_pack* OuPack);

    bool send_cmsg_login(out_pack* OuPack);
    bool send_cmsg_join_channel(out_pack* OuPack,std::string name);

    char* ChatLanguages(uint32 lang);
};