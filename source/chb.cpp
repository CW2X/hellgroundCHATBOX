#include "AuthProcessor.h"
#include "MainSocket.h"
#include "Session.h"
#include <iostream>
#include <process.h>

cli_pack ClPacket;

void __cdecl ClRun(void * args)
{
    char instr[256];
    uint16 i=0;
    ClPacket.data.clear();

    std::cin.getline(instr,256); 
    while(instr[i] && instr[i] != '\n')
        ClPacket.data.append(1,instr[i++]);
    ClPacket.size = i;
    ClPacket.type  = (instr[0] == '/' ) ? 0x01 : 0x02 ;
    _endthread();
}

int main( void )
{
    AuthProcessor   sAProcessor;
    MainSocket      sMainSocket;
    Session         sSession;
    ClPacket.type = 0x03;
    uint8           RealmId;

    inc_pack InPacket;
    out_pack OuPacket;
    try
    {
        while(1)
        {
            if (!sAProcessor.IsAuthed)
            {
                sAProcessor.Update();
            }
            else
            {
                if(!sMainSocket.IsConnected)
                {
                    printf("select realm ");
                    std::cin >> RealmId;
                    sMainSocket.ServerAdress = sAProcessor.GetRealmAdress(RealmId-49);
                    sMainSocket.ServerPort = sAProcessor.GetRealmPort(RealmId-49);
                    sMainSocket.SetKey(sAProcessor.GetKey());
                    sMainSocket.username = sAProcessor.GetUsername();
                    printf("\n");
                }
            
                sMainSocket.Update(&InPacket);
            
                if(sMainSocket.IsAuthed)
                {
                    OuPacket.reset();
                    sSession.Update(&InPacket,&OuPacket);

                    if(OuPacket.gc() != 0)
                        sMainSocket.send_out_pack(&OuPacket);

                    if (ClPacket.type != 0)
                    {
                        OuPacket.reset();
                        sSession.ClUpdate(&ClPacket,&OuPacket);

                        if(OuPacket.gc() != 0)
                            sMainSocket.send_out_pack(&OuPacket);
                        ClPacket.type = 0;
                        _beginthread(ClRun,0,NULL);
                    }
                }
            }
        }
    }
    catch (std::string error)
    {
        std::cout << error;
        return 0;
    }

    return 0;
}
