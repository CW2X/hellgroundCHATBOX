#include <ws2tcpip.h>
#include <WinSock2.h>
#include "base_defs.h"

#ifndef BASE_SOCKET
#define BASE_SOCKET

class BaseSocket
{
public:
    BaseSocket();
    bool open_socket();
    void close_socket();
    bool send_packet(char buffer[BUFFER_SIZE],uint8 datalength);
    bool recv_packet(char * buffer,uint8* datalength);

    PCSTR ServerAdress;
    PCSTR ServerPort;
    bool IsConnected;
private:
    WSADATA wsa_data;
    int MySocket;
};
#endif