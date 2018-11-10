/* This source file is part of koostosh's chb project
see README for copyright notice */

#ifndef SESSION_H
#define SESSION_H

#include "Modules/LoginModule.h"
#include "Modules/ChatModule.h"
#include "Modules/SocialModule.h"
#include "Modules/Database.h"

class MainSocket;
class CHBMain;

class Session
{
public:
    Session( Database * database );
    void Update(inc_pack* InPack);
    void ClUpdate(std::string clData);
private:
    void handle_Cl(std::string clData);

    out_pack        OuPack;
    std::string     username;
    
    Database*       m_database;
    CHBMain *       m_chbMain;
    LoginModule     sLoginModule;
    ChatModule      sChatModule;
    SocialModule    sSocialModule;
};

#endif