#ifndef CHBMAIN_H
#define CHBMAIN_H

#include "Session.h"
#include "MainSocket.h"
#include "AuthProcessor.h"

class CHBMain
{
public:
    static CHBMain* getInstance()
    {
        static CHBMain instance;
        return &instance;
    }

    void Update(std::string* retstr,std::string* commstr);
    void Input(std::string in);
    void Initialize(std::string username,std::string password);
    void send_out_pack(out_pack* packet);
private:
    CHBMain();
    
    bool firstTick;
    bool initialized;
    MainSocket    sMainSocket;
    AuthProcessor sAProcessor;
    Session       sSession;
};
#define sCHBMain CHBMain::getInstance()

#endif
