#include <ws2tcpip.h>
#include <WinSock2.h>
#include "base_defs.h"

#ifndef BASE_SOCKET
#define BASE_SOCKET

class BaseSocket
{
public:
    BaseSocket();
    bool IsConnected;
    bool IsAuthed;
    PCSTR ServerAdress;
    PCSTR ServerPort;
protected:
    bool open_socket();
    void close_socket(); // for now never used
    bool send_packet(char buffer[BUFFER_SIZE],uint16 datalength);
    bool recv_packet(char * buffer,uint16* datalength);
private:
    WSADATA wsa_data;
    int MySocket;
};
#endif