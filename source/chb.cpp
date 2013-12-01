#include "MainWindow.h"
#include "LoginForm.h"

using namespace chb;

int main( void )
{
    Form^ form = gcnew LoginForm;
    form->StartPosition = FormStartPosition::CenterScreen;
    Application::Run(form);

    return 0;
}
