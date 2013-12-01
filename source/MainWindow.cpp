#include "MainWindow.h"
using namespace chb;

void MainWindow::BackgroundThread()
{
    inc_pack    InPacket;
    std::string retstr;
    
    try
    {
        while(1)
        {
            if (!sSession->initialized)
            {
                Thread::Sleep(1000);
                continue;
            }

            uint8 css = CSN::Update(&InPacket,&retstr);
            if (css & CSN::CSS_ERROR)
                throw retstr;
            if (css & CSN::CSS_INFO)
            {
                readData = gcnew String(retstr.c_str());
                print_socket_msg();
            }
            
            if(css & CSN::CSS_READY)
            {
                sSession->Update(&InPacket,&retstr);
                if (retstr != "")
                {
                    readData = gcnew String(retstr.c_str());
                    print_session_msg();
                }
            }
        }
    }
    catch (std::string error)
    {
        readData = gcnew String(error.c_str());
        print_socket_msg();
    }
    catch (char* error)
    {
        readData = gcnew String(error);
        print_socket_msg();
    }
    catch (...)
    {
        readData = gcnew String("Unhandled exception!");
        print_socket_msg();
    };
}

void MainWindow::print_socket_msg()
{
    if (ExitingProgram)
        return;
    if (this->InvokeRequired)
    {
        this->Invoke(gcnew MethodInvoker(this,&chb::MainWindow::print_socket_msg));
    }
    else
    {
        viewtext->Text = viewtext->Text + readData;
        viewtext->Select(viewtext->TextLength-1,0);
        viewtext->ScrollToCaret();
    }
} 

void MainWindow::print_session_msg()
{
    if (this->InvokeRequired)
    {
        this->Invoke(gcnew MethodInvoker(this,&chb::MainWindow::print_socket_msg));
    }
    else
    {
        viewtext->Text = viewtext->Text + readData;
        viewtext->Select(viewtext->TextLength-1,0);
        viewtext->ScrollToCaret();
    }
}

void Session::InitializeSocket(std::string username,std::string password)
{
    CSN::Initialize(username,password,"logon.hellground.net");
    initialized = true;
}
