#include "base_defs.h"
#include "iopackets.h"
#include <map>
#include <list>

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
    char        who[15];
};

struct PlayerInfo
{
    uint32      guid;
    std::string name;
    uint32      clas;
    uint32      race;
    uint32      gender;
};

static char* ChatTagIdentifiers[]   = {"    ","AFK|","DND|","DND|","[GM]","[GM]","[GM]","[GM]"};

class Session
{
public:
    Session();
    bool Update(inc_pack* InPack,out_pack* OuPack);
    bool ClUpdate(cli_pack* InPack,out_pack* OuPack);
   
private:
    bool handle_smsg_char_enum(inc_pack* InPack,out_pack* OuPack);
    bool handle_smsg_name_query_response(inc_pack*);
    bool handle_smsg_messagechat(inc_pack* InPack,out_pack* OuPack);
    bool handle_smsg_channel_notify(inc_pack* InPack,out_pack* OuPack);
    bool handle_smsg_channel_list(inc_pack* InPack);
    bool handle_smsg_auth_response(inc_pack* InPack,out_pack* OuPack);
    bool handle_smsg_login_verify_world(inc_pack* InPack,out_pack* OuPack); 
    bool handle_Cl(cli_pack* InPack,out_pack* OuPack);

    bool send_cmsg_login(out_pack* OuPack,uint8 i);
    bool send_cmsg_join_channel(out_pack* OuPack,std::string name);
    bool send_cmsg_leave_channel(out_pack* OuPack,uint8 no);
    bool send_cmsg_messagechat(std::string data, out_pack* OuPack);
    bool send_cmsg_char_enum(out_pack* OuPack);
    bool send_cmsg_name_query(out_pack* OuPack,uint32 guid);
    bool send_cmsg_channel_list(out_pack* OuPack,std::string channelname);

    char*           ChatLanguages(uint32 lang);
    CharacterData   characters[7];
    std::string     channels[9];
    bool            channelson[9];
    uint8           activechannel;
    uint8           cinredirect;
    std::map<uint32,PlayerInfo> PlayersInfoMap;
    std::list<uint32> UnkPlayers,RequestedPlayers;
    // first one stores all guids that are on our channels, second one only those who write sth and we want their nicks ASAP
    std::string     whisptarget;
};