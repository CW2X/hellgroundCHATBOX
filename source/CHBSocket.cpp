#include "CHBSocket.h"
using namespace CSN;

CHBSocket::CHBSocket()
{
    initialized = false;
}

void CHBSocket::Initialize(std::string username,std::string password,std::string address)
{
    sMainSocket.m_username = username;
    sAProcessor.Initialize(username,password,address);
    initialized = true;
}

uint8 CHBSocket::Update(inc_pack* InPacket,std::string* retstr)
{
    *retstr = "";
    uint8 css = CSS_NONE;
    
    if(!initialized)
        return css;
    
    try
    {
        if (!sAProcessor.IsAuthed)
        {
            sAProcessor.Update(retstr);
        }
        else
        {
            if(!sMainSocket.IsConnected)
            {
                sMainSocket.AddressString = sAProcessor.GetRealmAdress(0);
                sMainSocket.PortString = sAProcessor.GetRealmPort(0);
                sMainSocket.SetKey(sAProcessor.GetKey());
            }
            
            sMainSocket.Update(InPacket,retstr);
        }
    }
    catch (std::string error)
    {
        *retstr = "CHBSocket: " + error;
        return CSS_ERROR;
    }
    catch (char* error)
    {
        *retstr = "CHBSocket: " + std::string(error);
        return CSS_ERROR;
    }
    catch (...)
    {
        *retstr = "CHBSocket: Catched unhandled exception!";
        return CSS_ERROR;
    }

    if (sMainSocket.IsAuthed)
        css |= CSS_READY;
    if (*retstr != "")
        css |= CSS_INFO;
    return css;
}

void CHBSocket::send_out_pack(out_pack* packet)
{
    sMainSocket.send_out_pack(packet);
}
