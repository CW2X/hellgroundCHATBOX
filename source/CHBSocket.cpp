#include "CHBSocket.h"
using namespace CSN;

CHBSocket::CHBSocket()
{
}

bool CHBSocket::AllReady()
{
    return sMainSocket.IsAuthed;
}

bool CHBSocket::Update(inc_pack* InPacket,std::string* retstr)
{
    try
    {
        if (!sAProcessor.IsAuthed)
        {
            sAProcessor.Update();
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
            
            sMainSocket.Update(InPacket);
        }
    }
    catch (std::string error)
    {
        *retstr = error;
        return false;
    }
    catch (char* error)
    {
        *retstr = error;
        return false;
    }
    catch (...)
    {
        *retstr = "Catched unhandled exception!";
        return false;
    }
    return true;
}

void CHBSocket::send_out_pack(out_pack* packet)
{
    sMainSocket.send_out_pack(packet);
}