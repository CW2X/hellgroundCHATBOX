/* This source file is part of koostosh's chb project
see README for copyright notice */

#ifndef LOGINMODULE_H
#define LOGINMODULE_H

#include "Module.h"

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

class LoginModule : public Module
{
public:
    void Handle(inc_pack* InPack);
    void Command(std::string cmd,std::string args) {};

    void send_cmsg_login(uint8 i);
private:
    void handle_smsg_auth_response(inc_pack* InPack);
    void handle_smsg_char_enum(inc_pack* InPack);
    void handle_smsg_login_verify_world(inc_pack* InPack);
    void send_cmsg_char_enum();

    CharacterData   characters[7];
};

#endif