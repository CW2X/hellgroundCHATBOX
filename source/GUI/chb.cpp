/* This source file is part of koostosh's chb project
   see README for copyright notice */

#define WIN32_LEAN_AND_MEAN
#include "MainWindow.h"

using namespace chb;

int main( void )
{
    Form^ form = gcnew MainWindow;
    form->StartPosition = FormStartPosition::CenterScreen;
    Application::Run(form);

    return 0;
}
