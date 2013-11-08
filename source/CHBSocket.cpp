#include "CHBSocket.h"
using namespace CSN;

CHBSocket::CHBSocket()
{
}

void CHBSocket::Initialize(std::string login,std::string password,std::string address)
{

}

uint8 CHBSocket::Update(inc_pack* InPacket,std::string* retstr)
{
    *retstr = "";
    uint8 css = CSS_NONE;

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
                sMainSocket.ServerAdress = sAProcessor.GetRealmAdress(0);
                sMainSocket.ServerPort = sAProcessor.GetRealmPort(0);
                sMainSocket.SetKey(sAProcessor.GetKey());
                sMainSocket.username = sAProcessor.GetUsername();
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
    return css;
}

void CHBSocket::send_out_pack(out_pack* packet)
{
    sMainSocket.send_out_pack(packet);
}
