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
                readData = gcnew String((sub.substr(3,sub.length()-3) + ":").c_str());
                set_channel_label();
            }
            else if (sub.substr(0,4) == "FrAN")
            {
                readData = gcnew String(sub.substr(4,sub.length()-4).c_str());
                friend_add_online();
            }
            else if (sub.substr(0,4) == "FrAF")
            {
                readData = gcnew String(sub.substr(4,sub.length()-4).c_str());
                friend_add_offline();
            }
            else if (sub.substr(0,3) == "FrR")
            {
                readData = gcnew String(sub.substr(3,sub.length()-3).c_str());
                friend_remove();
            }
            else if (sub.substr(0,3) == "GuA")
            {
                readData = gcnew String(sub.substr(3,sub.length()-3).c_str());
                guild_add();
            }
            else if (sub.substr(0,3) == "GuR")
            {
                readData = gcnew String(sub.substr(3,sub.length()-3).c_str());
                guild_remove();
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
        
        if(this->scrollingCheckbox->Checked)
            viewtext->AppendText(readData);
        else
            viewtext->Text = viewtext->Text + readData;
    }
}

void MainWindow::set_channel_label()
{
    if (this->InvokeRequired)
        this->Invoke(gcnew MethodInvoker(this,&chb::MainWindow::set_channel_label));
    else
        channel_label->Text = readData;
}

void MainWindow::friend_add_online()
{
    if (this->InvokeRequired)
        this->Invoke(gcnew MethodInvoker(this,&chb::MainWindow::friend_add_online));
    else
    {
        FriendsListbox->Items->Remove(readData);
        FriendsListbox->Items->Insert(0,readData);
    }
}

void MainWindow::friend_add_offline()
{
    if (this->InvokeRequired)
        this->Invoke(gcnew MethodInvoker(this,&chb::MainWindow::friend_add_offline));
    else
    {
        FriendsListbox->Items->Remove(readData);
        FriendsListbox->Items->Insert(FriendsListbox->Items->IndexOf(L"===Offline===") + 1,readData);
    }
}

void MainWindow::friend_remove()
{
    if (this->InvokeRequired)
        this->Invoke(gcnew MethodInvoker(this,&chb::MainWindow::friend_remove));
    else
        FriendsListbox->Items->Remove(readData);
}

void MainWindow::guild_add()
{
    if (this->InvokeRequired)
        this->Invoke(gcnew MethodInvoker(this,&chb::MainWindow::guild_add));
    else
        GuildListbox->Items->Add(readData);
}

void MainWindow::guild_remove()
{
    if (this->InvokeRequired)
        this->Invoke(gcnew MethodInvoker(this,&chb::MainWindow::guild_remove));
    else
        GuildListbox->Items->Remove(readData);
}
void MainWindow::LoginFormReturn(std::string username,std::string password)
{
    if (mainDllInputFunction != NULL)
        mainDllInputFunction(std::string("/login ") + username + std::string(" ") + password);
    this->inputtext->Focus();
}
