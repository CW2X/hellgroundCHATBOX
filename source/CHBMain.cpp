/* This source file is part of koostosh's chb project
see README for copyright notice */

#include "CHBMain.h"

#define WIN32_LEAN_AND_MEAN
#include <ws2tcpip.h>
#include <WinSock2.h>

CHBMain::CHBMain()
    : m_database( this )
    , m_session( &m_database )
{
    firstTick = true;
    initialized = false;
    stopWorking = false;
    m_ret = "";
    m_comm = "";
}

bool CHBMain::Update(std::string* retstr,std::string* commstr)
{
    if (stopWorking)
        return false;

    inc_pack InPacket;

    try
    {
        *retstr = "";
        *commstr = "";
        if(firstTick)
        {
            firstTick = false;
            *retstr = std::string("Chatbox v.") + VERSION + "\r\n";
            *commstr = std::string("Ln\nCh:Say\n");

            WSADATA wsa_data;
            int err = WSAStartup(MAKEWORD(2, 2), &wsa_data);
            if (err != 0)
            {
                *retstr += std::string("Unable to init winsock\r\n");
                stopWorking = true;
                return false;
            }
        }

        if(!m_socket.IsAuthed)
        {
            if (!initialized)
                return true;

            if (!m_authProcessor.IsAuthed)
            {
                m_authProcessor.Update(retstr);
                return true;
            }
        
            if(!m_socket.IsConnected)
            {
                m_socket.AddressString = m_authProcessor.GetRealmAdress(0);
                m_socket.PortString = m_authProcessor.GetRealmPort(0);
                m_socket.SetKey(m_authProcessor.GetKey());
            }    
            m_socket.Update(&InPacket,retstr);
        }
        else
        {
            m_socket.Update(&InPacket,retstr);
            m_session.Update(&InPacket);
        }

        *retstr += m_ret;
        *commstr += m_comm;
        m_ret = "";
        m_comm = "";

    }
    catch (std::string error)
    {
        *retstr += error;
        stopWorking = true;
    }
    catch (char* error)
    {
        *retstr += error;
        stopWorking = true;
    }
    catch(...)
    {
        *retstr += "Unhandled error!";
        stopWorking = true;
    }

    return !stopWorking;
}

void CHBMain::Input(std::string in)
{
    m_session.ClUpdate(in);
}

void CHBMain::Initialize(std::string username,std::string password)
{
    m_socket.m_username = username;
    m_authProcessor.Initialize(username,password);
    initialized = true;
}

void CHBMain::send_out_pack(out_pack* packet) 
{
    m_socket.send_out_pack(packet);
}
