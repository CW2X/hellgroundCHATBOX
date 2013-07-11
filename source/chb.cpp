#include "base_defs.h"
#include "AuthProcessor.h"
#include "MainSocket.h"

AuthProcessor   sAProcessor;
MainSocket      sMainSocket;

int main( void )
{
    if(!sAProcessor.open_socket())
        return 1;
    
    if (!sAProcessor.send_logon_challenge())
        return 1;

    while(1)
    {
        if (!sAProcessor.Update())
            break;
        
        if (sAProcessor.RealmListReady)
        {
            if(!sMainSocket.IsConnected)
            {
                sMainSocket.ServerAdress = sAProcessor.GetRealmAdress(0);
                sMainSocket.ServerPort = sAProcessor.GetRealmPort(0);
            }
            if(!sMainSocket.Update())
                break;
        }
    }
    printf("exiting");

    return 0;
}