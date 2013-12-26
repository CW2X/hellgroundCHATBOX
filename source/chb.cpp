#include "MainWindow.h"

using namespace chb;

int main( void )
{
    Form^ form = gcnew MainWindow;
    form->StartPosition = FormStartPosition::CenterScreen;
    Application::Run(form);

    return 0;
}
