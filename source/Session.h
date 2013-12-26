#ifndef SESSION_H
#define SESSION_H

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
    std::string who;
};

struct PlayerInfo
{
    std::string name;
    uint32      clas;
    uint32      race;
    uint32      gender;
};

static char* ChatTagIdentifiers[]   = {"    ","AFK|","DND|","DND|","[GM]","[GM]","[GM]","[GM]"};

class MainSocket;

class Session
{
public:
    Session();
    void Update(inc_pack* InPack,std::string* retstr);
    void ClUpdate(std::string clData);
    void InitializeSocket(std::string username, std::string password);
private:
    void handle_smsg_char_enum(inc_pack* InPack);
    void handle_smsg_name_query_response(inc_pack* InPack);
    void handle_smsg_messagechat(inc_pack* InPack);
    void handle_smsg_channel_notify(inc_pack* InPack);
    void handle_smsg_channel_list(inc_pack* InPack);
    void handle_smsg_notification(inc_pack* InPack);
    void handle_smsg_auth_response(inc_pack* InPack);
    void handle_smsg_login_verify_world(inc_pack* InPack); 
    void handle_smsg_chat_player_not_found(inc_pack* InPack);
    void handle_smsg_userlist_add(inc_pack*);
    void handle_Cl(std::string clData);

    void send_cmsg_login(uint8 i);
    void send_cmsg_join_channel(std::string name);
    void send_cmsg_leave_channel(uint8 no);
    void send_cmsg_messagechat(std::string data);
    void send_cmsg_char_enum();
    void send_cmsg_name_query(uint32 guid);
    void send_cmsg_channel_list(std::string channelname);

    std::string Guid_to_name(uint32 guid);
    char* ChatLanguages(uint32 lang);
    void print(std::string s) {m_ret += s;};
    void send_out_pack();

    CharacterData   characters[7];
    std::string     channels[9];
    bool            channelson[9];
    uint8           activechannel;
    uint8           cinredirect;
    std::map<uint32,PlayerInfo> PlayersInfoMap;
    std::list<uint32> RequestedPlayers;
    std::string     whisptarget;
    out_pack        OuPack;
    bool            ishordeplayer;
    std::string     username;
    std::string     m_ret;
};

#endif