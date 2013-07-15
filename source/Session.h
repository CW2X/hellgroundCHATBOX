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
    Session();
    bool Update(inc_pack* InPack,out_pack* OuPack);
    bool ClUpdate(cli_pack* InPack,out_pack* OuPack);
   
private:
    bool handle_char_enum(inc_pack* InPack,out_pack* OuPack);
    bool handle_chat_message(inc_pack* InPack,out_pack* OuPack);
    bool handle_auth_response(inc_pack* InPack,out_pack* OuPack);
    bool handle_login_verify(inc_pack* InPack,out_pack* OuPack);
    bool handle_Cl(cli_pack* InPack,out_pack* OuPack);

    bool send_cmsg_login(out_pack* OuPack);
    bool send_cmsg_join_channel(out_pack* OuPack,std::string name);
    bool send_chat_message(std::string data, out_pack* OuPack);
    bool send_char_enum(out_pack* OuPack);

    char* ChatLanguages(uint32 lang);
    CharacterData characters[8];
    std::string channels[9];
    uint8 nofchannels,activechannel;
};