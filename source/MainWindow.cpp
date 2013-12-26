#include "MainWindow.h"
using namespace chb;

void MainWindow::BackgroundThread()
{
    std::string retstr;
    unsigned char returnvalue;

    while(1)
    {

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

        returnvalue = mainDllUpdateFunction(&retstr);

        if (retstr != "")
        {
            readData = gcnew String(retstr.c_str());
            print_msg();
        }
        
        if (returnvalue == 1)
        {
            Thread::Sleep(500);
            CreateLoginForm();
            
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

void MainWindow::LoginFormReturn(std::string username,std::string password)
{
    if (mainDllInputFunction != NULL)
        mainDllInputFunction(std::string("/login ") + username + std::string(" ") + password);
}
