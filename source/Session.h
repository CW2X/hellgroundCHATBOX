#ifndef SESSION_H
#define SESSION_H

#include "Modules/LoginModule.h"
#include "Modules/ChatModule.h"
#include "Modules/SocialModule.h"
#include "Modules/Database.h"

class MainSocket;

class Session
{
public:
    Session();
    void Update(inc_pack* InPack);
    void ClUpdate(std::string clData);
private:
    void handle_Cl(std::string clData);


    uint8           cinredirect;
    out_pack        OuPack;
    std::string     username;
    
    LoginModule     sLoginModule;
    ChatModule      sChatModule;
    SocialModule    sSocialModule;
};

#endif