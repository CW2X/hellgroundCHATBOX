#include "MainWindow.h"
using namespace chb;

void MainWindow::BackgroundThread()
{
    std::string retstr;
    std::string commstr;

    Thread::Sleep(500);// wait for everything to get ready
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
        if (!retstr.empty())
        {
            readData = %String(retstr.c_str());
            this->Invoke(gcnew MethodInvoker(this, &chb::MainWindow::print_msg));
        }

        //parse internal commands
        if (!commstr.empty())
        {
            commandData = %String(commstr.c_str());
            parse_commands();
        }
    }
}

void MainWindow::print_msg()
{
    if (this->scrollingCheckbox->Checked)
    {
        viewtext->AppendText(readData);
        viewtext->SelectionStart = viewtext->Text->Length;
        viewtext->ScrollToCaret();
    }
    else
        viewtext->AppendText(readData);
}

void MainWindow::parse_commands()
{
    while (!String::IsNullOrEmpty(commandData))
    {
        int siz = commandData->IndexOf("\n");
        String^ sub = commandData->Substring(0, siz);
        if (siz != commandData->Length)
            commandData = commandData->Remove(0,siz+1);
        else
            commandData->Remove(commandData->Length);

        if (sub == "Ln")
        {
            CreateLoginForm();
        }
        else if (sub->Substring(0, 3) == "Ch:")
        {
            readData = String::Concat(sub->Substring(3, sub->Length - 3), ":");
            set_channel_label();
        }
        else if (sub->Substring(0, 4) == "FrAN")
        {
            readData = sub->Substring(4, sub->Length - 4);
            friend_add_online();
        }
        else if (sub->Substring(0, 4) == "FrAF")
        {
            readData = sub->Substring(4, sub->Length - 4);
            friend_add_offline();
        }
        else if (sub->Substring(0, 3) == "FrR")
        {
            readData = sub->Substring(3, sub->Length - 3);
            friend_remove();
        }
        else if (sub->Substring(0, 3) == "GuA")
        {
            readData = sub->Substring(3, sub->Length - 3);
            guild_add();
        }
        else if (sub->Substring(0, 3) == "GuR")
        {
            readData = sub->Substring(3, sub->Length - 3);
            guild_remove();
        }
        else
        {
            readData = gcnew String("UIC\r\n");
            print_msg();
        }
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
