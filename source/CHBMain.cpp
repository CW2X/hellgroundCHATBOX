#include "CHBMain.h"

CHBMain::CHBMain()
{
    firstTick = true;
    initialized = false;
    m_ret = "";
    m_comm = "";
}

void CHBMain::Update(std::string* retstr,std::string* commstr)
{
    inc_pack InPacket;

    try
    {
        *retstr = "";
        *commstr = "";
        if(firstTick)
        {
            firstTick = false;
            *retstr = std::string("Chatbox v.")+ VERSION +"\r\n";
            *commstr = std::string("Ln\nCh:Say\n");
        }

        if(!sMainSocket.IsAuthed)
        {
            if (!initialized)
                return;

            if (!sAProcessor.IsAuthed)
            {
                sAProcessor.Update(retstr);
                return;
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
            sSession.Update(&InPacket);
        }

        *retstr += m_ret;
        *commstr += m_comm;
        m_ret = "";
        m_comm = "";

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

void Module::send_out_pack()
{
    sCHBMain->send_out_pack(&OuPack);
}

void Module::print(std::string s)
{
    sCHBMain->print(s);
}

void Module::i_comm(std::string s)
{
    sCHBMain->i_comm(s);
}

extern "C"
{
    _declspec(dllexport) void MainDllUpdate(std::string* retstr,std::string* commstr)
    {
        sCHBMain->Update(retstr,commstr);
    };
    _declspec(dllexport) void MainDllInput(std::string in)
    {
        sCHBMain->Input(in);
    };
}
