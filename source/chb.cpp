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

    while(1)
    {
        if (!sAProcessor.IsAuthed)
        {
            if (!sAProcessor.Update())
                return 1;
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
            
            if(!sMainSocket.Update(&InPacket))
                return 1;
            
            if(sMainSocket.IsAuthed)
            {
                OuPacket.reset();
                if (!sSession.Update(&InPacket,&OuPacket))
                    return 1;
                
                if(OuPacket.gc() != 0 && !sMainSocket.send_out_pack(&OuPacket))
                    return 1;

                if (ClPacket.type != 0)
                {
                    OuPacket.reset();
                    if(!sSession.ClUpdate(&ClPacket,&OuPacket))
                        return 1;
                    if(OuPacket.gc() != 0 && !sMainSocket.send_out_pack(&OuPacket))
                        return 1;
                    ClPacket.type = 0;
                    _beginthread(ClRun,0,NULL);
                }
            }
        }
    }
    printf("exiting\n");

    return 0;
}
