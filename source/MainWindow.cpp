#include "MainWindow.h"
#include "Session.h"
using namespace chb;

void chb::BackgroundThread()
{
    Session         sSession;
    inc_pack InPacket;
    std::string retstr;

    try
    {
        while(1)
        {
            uint8 css = CSN::Update(&InPacket,&retstr);
            if (css & CSN::CSS_ERROR)
                throw retstr;
            if (css & CSN::CSS_INFO)
                //print
                ;

            
            if(css & CSN::CSS_READY)
                sSession.Update(&InPacket);
        }
    }
    catch (std::string)
    {
        //std::cout << error;
    }
    catch (char*)
    {
        //std::cout << error;
    }
    catch (...)
    {
        //std::cout << "Catched unhandled exception!";
    };
}