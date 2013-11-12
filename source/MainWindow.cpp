#include "MainWindow.h"
#include "Session.h"
using namespace chb;

void MainWindow::BackgroundThread()
{
    Session     sSession;
    inc_pack    InPacket;
    std::string retstr;
    
    CSN::Initialize("DERLIN358","PASSDERLIN","logon.hellground.net");
    try
    {
        while(1)
        {
            uint8 css = CSN::Update(&InPacket,&retstr);
            if (css & CSN::CSS_ERROR)
                throw retstr;
            if (css & CSN::CSS_INFO)
            {
                readData = gcnew String(retstr.c_str());
                print_msg();
            }
            
            if(css & CSN::CSS_READY)
                sSession.Update(&InPacket);
        }
    }
    catch (std::string error)
    {
        readData = gcnew String(error.c_str());
        print_msg();
    }
    catch (char* error)
    {
        readData = gcnew String(error);
        print_msg();
    }
    catch (...)
    {
        readData = gcnew String("Unhandled exception!");
        print_msg();
    };
}

void MainWindow::print_msg()
{
    if (this->InvokeRequired)
    {
        this->Invoke(gcnew MethodInvoker(this,&chb::MainWindow::print_msg));
    }
    else
        viewtext->Text = viewtext->Text + readData;
} 
