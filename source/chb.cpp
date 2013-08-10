#include "AuthProcessor.h"
#include "Session.h"
#include <iostream>
#include <process.h>

cli_pack    ClPacket;

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
    Session         sSession;
    ClPacket.type = 0x03;
    uint8           RealmId;
    
    inc_pack InPacket;
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
                    sSession.Update(&InPacket);

                    if (ClPacket.type != 0)
                    {
                        sSession.ClUpdate(&ClPacket);
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
    catch (char* error)
    {
        std::cout << error;
        return 0;
    }
    catch (...)
    {
        std::cout << "Catched unhandled exception!";
        return 0;
    }

    return 0;
}
