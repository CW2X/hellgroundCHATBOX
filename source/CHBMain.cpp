#include "CHBMain.h"

CHBMain::CHBMain()
{
    firstTick = true;
    initialized = false;
}

uint8 CHBMain::Update(std::string* retstr)
{
    inc_pack InPacket;

    try
    {
        *retstr = "";

        if(firstTick)
        {
            firstTick = false;
            *retstr = std::string("Chatbox v.")+ VERSION +"\r\n";
            return 1;
        }

        if(!sMainSocket.IsAuthed)
        {
            if (!initialized)
                return 0;

            if (!sAProcessor.IsAuthed)
            {
                sAProcessor.Update(retstr);
                return 0;
            }
        
            if(!sMainSocket.IsConnected)
            {
                sMainSocket.AddressString = sAProcessor.GetRealmAdress(0);
                sMainSocket.PortString = sAProcessor.GetRealmPort(0);
                sMainSocket.SetKey(sAProcessor.GetKey());
            }    
            sMainSocket.Update(&InPacket,retstr);
        }
        else
        {
            sMainSocket.Update(&InPacket,retstr);
            sSession.Update(&InPacket,retstr);
        }
    }
    catch (std::string error)
    {
        *retstr += error;
    }
    catch (char* error)
    {
        *retstr += error;
    }
    catch(...)
    {
        *retstr += "Unhandled error!";
    }
    return 0;
}

void CHBMain::Input(std::string in)
{
    sSession.ClUpdate(in);
}

void CHBMain::Initialize(std::string username,std::string password)
{
    sMainSocket.m_username = username;
    sAProcessor.Initialize(username,password);
    initialized = true;
}

void CHBMain::send_out_pack(out_pack* packet) 
{
    sMainSocket.send_out_pack(packet);
}

void Session::send_out_pack()
{
    sCHBMain->send_out_pack(&OuPack);
}

extern "C"
{
    _declspec(dllexport) uint8 MainDllUpdate(std::string* retstr)
    {
        return sCHBMain->Update(retstr);
    };
    _declspec(dllexport) void MainDllInput(std::string in)
    {
        sCHBMain->Input(in);
    };
}
