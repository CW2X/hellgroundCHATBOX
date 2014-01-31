#ifndef SESSION_H
#define SESSION_H

#include "LoginModule.h"
#include "ChatModule.h"
#include "SocialModule.h"
#include "Database.h"

class MainSocket;

class Session
{
public:
    Session();
    void Update(inc_pack* InPack);
    void ClUpdate(std::string clData);
    void InitializeSocket(std::string username, std::string password);
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