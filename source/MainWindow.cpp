#include "MainWindow.h"
using namespace chb;

void MainWindow::BackgroundThread()
{
    std::string retstr;
    std::string commstr;

    while(1)
    {
        //Load functions if nesescary
        if (mainDllUpdateFunction == NULL)
        {
            HMODULE MainDll = LoadLibrary(TEXT("CHBMain.dll"));
            if (!MainDll)
            {
                continue;// some error handling
            }
            mainDllUpdateFunction = (mainDllUpdateType)GetProcAddress(MainDll,"MainDllUpdate");
            mainDllInputFunction = (mainDllInputType)GetProcAddress(MainDll,"MainDllInput");
            continue;
        }
        //call function
        mainDllUpdateFunction(&retstr,&commstr);

        //print output
        if (retstr != "")
        {
            readData = gcnew String(retstr.c_str());
            print_msg();
        }
        
        //parse internal commands
        while(!commstr.empty())
        {
            size_t siz = commstr.find("\n");
            std::string sub= commstr.substr(0,siz);
            if (siz != std::string::npos)
                commstr.erase(0,siz+1);
            else
                commstr.clear();

            if (sub == "Ln")
            {
                Thread::Sleep(500);
                CreateLoginForm();
            }
            else if (sub.substr(0,3) == "Ch:")
            {
                channelLabel = gcnew String((sub.substr(3,sub.length()-3) + ":").c_str());
                set_channel_label();
            }
            else if (sub.substr(0,4) == "FrAN")
            {
                FriendsListbox->Items->Remove(gcnew String(sub.substr(4,sub.length()-4).c_str()));
                FriendsListbox->Items->Insert(0,gcnew String(sub.substr(4,sub.length()-4).c_str()));
            }
            else if (sub.substr(0,4) == "FrAF")
            {
                FriendsListbox->Items->Remove(gcnew String(sub.substr(4,sub.length()-4).c_str()));
                FriendsListbox->Items->Insert(FriendsListbox->Items->IndexOf(L"===Offline===") + 1
                    ,gcnew String(sub.substr(4,sub.length()-4).c_str()));
            }
            else if (sub.substr(0,3) == "FrR")
            {
                FriendsListbox->Items->Remove(gcnew String(sub.substr(3,sub.length()-3).c_str()));
            }
            else if (sub.substr(0,3) == "GuA")
            {
                GuildListbox->Items->Add(gcnew String(sub.substr(3,sub.length()-3).c_str()));
            }
            else if (sub.substr(0,3) == "GuR")
            {
                GuildListbox->Items->Remove(gcnew String(sub.substr(3,sub.length()-3).c_str()));
            }
            else
            {
                readData = gcnew String("UIC\r\n");
                print_msg();
            }
        }
    }
}

void MainWindow::print_msg()
{
    if (this->InvokeRequired)
    {
        this->Invoke(gcnew MethodInvoker(this,&chb::MainWindow::print_msg));
    }
    else
    {
        viewtext->Text = viewtext->Text + readData;
        viewtext->Select(viewtext->TextLength-1,0);
        viewtext->ScrollToCaret();
    }
}

void MainWindow::set_channel_label()
{
    if (this->InvokeRequired)
        this->Invoke(gcnew MethodInvoker(this,&chb::MainWindow::set_channel_label));
    else
        channel_label->Text = channelLabel;
}

void MainWindow::LoginFormReturn(std::string username,std::string password)
{
    if (mainDllInputFunction != NULL)
        mainDllInputFunction(std::string("/login ") + username + std::string(" ") + password);
}
