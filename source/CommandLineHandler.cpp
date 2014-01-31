#include "CHBMain.h"
#include "Util.h"

void Session::ClUpdate(std::string clData)
{
    if (clData.c_str()[0] == '/')
        handle_Cl(clData);
    else
    {
        switch (cinredirect)
        {
        case 0:
            sChatModule.Command("msg",clData);
            break;
        case 1:
            sLoginModule.send_cmsg_login((uint8)clData.c_str()[0]);
            break;
        }
    }
}

void Session::handle_Cl(std::string clData)
{
    if(clData[0] == 0x00 || clData[1] == 0x00)
        return;

    std::string     cmd,args;
    uint8           space=0;

    while(clData.size() > space && clData.c_str()[space] != ' ')
        space++;
    
    cmd = clData.substr(1,space-1);
    if(clData.size() > space)
        args = clData.substr(space+1,clData.size() - space +1);

    if (cmd == "login")
    {
        space = 0;

        while(args.size() > space && args.c_str()[space] != ' ')
            space++;
        if(args.size() > space)
        {
            username = args.substr(0,space);
            std::string password = args.substr(space+1,args.size() - space +1);
            string_to_uppercase(username);
            string_to_uppercase(password);
            sCHBMain->Initialize(username,password);
            return;
        }
        return;
    }
    
    if(cmd[0] >'0' && cmd[0] <='9' && cmd[1] == 0x00)
    {
        sChatModule.Command(cmd,args);
    }

    if (cmd == "say" || cmd == "s" || cmd == "yell" || cmd == "y" || cmd == "guild" || cmd == "g" ||
        cmd == "officer" || cmd == "o" || cmd == "r" || cmd == "join" || cmd == "leave")
    {
        sChatModule.Command(cmd,args);
    }

    if(cmd == "w" && args != "")
    {
        sChatModule.Command(cmd,args);
    }

}
