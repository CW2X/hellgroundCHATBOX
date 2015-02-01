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

        if (retstr.empty() && commstr.empty())
            Thread::Sleep(5);

        array<Object^>^myStringArray = { gcnew String(retstr.c_str()), gcnew String(commstr.c_str()) };
        this->Invoke(this->mPD, myStringArray);
    }
}

void MainWindow::LoginFormReturn(std::string username,std::string password)
{
    if (mainDllInputFunction != NULL)
        mainDllInputFunction(std::string("/login ") + username + std::string(" ") + password);
    this->inputtext->Focus();
}

void MainWindow::ProcessMethod(String^ print, String^ command)
{
    if (!String::IsNullOrEmpty(print))
    {
        if (this->scrollingCheckbox->Checked)
        {
            viewtext->AppendText(print);
            viewtext->SelectionStart = viewtext->Text->Length;
            viewtext->ScrollToCaret();
        }
        else
            viewtext->AppendText(print);
    }
    while (!String::IsNullOrEmpty(command))
    {
        int siz = command->IndexOf("\n");
        String^ sub = command->Substring(0, siz);
        if (siz != command->Length)
            command = command->Remove(0, siz + 1);
        else
            command->Remove(command->Length);

        if (sub == "Ln")
        {
            CreateLoginForm();
        }
        else if (sub == "Cls")
        {
            viewtext->Clear();
        }
        else if (sub->Substring(0, 3) == "Ch:")
        {
            channel_label->Text = String::Concat(sub->Substring(3, sub->Length - 3), ":");
        }
        else if (sub->Substring(0, 4) == "FrAN")
        {
            FriendsListbox->Items->Remove(sub->Substring(4, sub->Length - 4));
            FriendsListbox->Items->Insert(0, sub->Substring(4, sub->Length - 4));
        }
        else if (sub->Substring(0, 4) == "FrAF")
        {
            FriendsListbox->Items->Remove(sub->Substring(4, sub->Length - 4));
            FriendsListbox->Items->Insert(FriendsListbox->Items->IndexOf(L"===Offline===") + 1, sub->Substring(4, sub->Length - 4));
        }
        else if (sub->Substring(0, 3) == "FrR")
        {
            FriendsListbox->Items->Remove(sub->Substring(3, sub->Length - 3));
        }
        else if (sub->Substring(0, 3) == "GuA")
        {
            GuildListbox->Items->Add(sub->Substring(3, sub->Length - 3));
        }
        else if (sub->Substring(0, 3) == "GuR")
        {
            GuildListbox->Items->Remove(sub->Substring(3, sub->Length - 3));
        }
        else
        {
            viewtext->AppendText("UIC!\r\n");
        }
    }

}
