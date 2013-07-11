#include "MainSocket.h"

bool MainSocket::Update()
{
    if(!IsConnected)
        return open_socket();

    char buf[6] = {0x35,0x00,0x00,0x00,0x00,0x00};
    send_packet(buf,6);
    return false;
}