/* This source file is part of koostosh's chb project
see README for copyright notice */

#ifndef CHBMAIN_H
#define CHBMAIN_H

#include "Session.h"
#include "Sockets\MainSocket.h"
#include "Sockets\AuthProcessor.h"

class CHBMain
{
public:
    CHBMain();

    bool IsInWorld() const;
    bool Update(std::string* retstr,std::string* commstr);
    void Input(std::string in);
    void Initialize(std::string username,std::string password);
    void send_out_pack(out_pack* packet);
    void print(std::string s) {m_ret += s;};
    void i_comm(std::string s) {m_comm += s;};
private:
    bool            firstTick;
    bool            initialized;
    bool            stopWorking;
    Database        m_database;
    MainSocket      m_socket;
    AuthProcessor   m_authProcessor;
    Session         m_session;
    std::string     m_ret;
    std::string     m_comm;
};

#endif
