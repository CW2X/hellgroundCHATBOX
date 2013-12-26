#include "LoginForm.h"

void chb::MainWindow::CreateLoginForm()
{
    if (this->InvokeRequired)
    {
        this->Invoke(gcnew MethodInvoker(this,&chb::MainWindow::CreateLoginForm));
    }
    else
    {
        Form^ form = gcnew LoginForm;
        form->Owner = this;
        form->Show();
    }
}