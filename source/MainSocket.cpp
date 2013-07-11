#include "MainSocket.h"

bool MainSocket::Update()
{
    if(!IsConnected)
    {

        open_socket();
    }
    //else
    return true;
}