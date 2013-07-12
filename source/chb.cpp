#include "AuthProcessor.h"
#include "MainSocket.h"

int main( void )
{
    AuthProcessor   sAProcessor;
    MainSocket      sMainSocket;

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
                sMainSocket.ServerAdress = sAProcessor.GetRealmAdress(0);
                sMainSocket.ServerPort = sAProcessor.GetRealmPort(0);
                sMainSocket.SetKey(sAProcessor.GetKey());
            }
            
            if(!sMainSocket.Update(&InPacket))
                return 1;

            // here we have incoming data (if any) in [InPacket] struct
            OuPacket.size = 0;
            // mainhandler(&InPacket,&OutPacket)
            // here we have outgoing data (if any) in [OuPacket] struct

            if(OuPacket.size != 0 && !sMainSocket.send_out_pack(&OuPacket))
                return 1;
        }
    }
    printf("exiting\n");

    return 0;
}