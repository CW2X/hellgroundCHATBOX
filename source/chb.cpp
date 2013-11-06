#include "Session.h"
#include <iostream>
#include <process.h>

cli_pack    ClPacket;

/*void __cdecl ClRun(void * args)
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
}*/

int main( void )
{
    Session         sSession;
    ClPacket.type = 0x03;
    
    inc_pack InPacket;
    std::string retstr;
    try
    {
        while(1)
        {
            if (!sSocket.Update(&InPacket,&retstr))
                throw retstr;
            
            if(sSocket.AllReady())
            {
                sSession.Update(&InPacket);

                if (ClPacket.type != 0)
                {
                    sSession.ClUpdate(&ClPacket);
                    ClPacket.type = 0;
                    //_beginthread(ClRun,0,NULL);
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
