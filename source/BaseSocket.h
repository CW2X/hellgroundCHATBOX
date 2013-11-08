#ifndef BASE_SOCKET
#define BASE_SOCKET

#include <ws2tcpip.h>
#include <WinSock2.h>
#include "base_defs.h"

class BaseSocket
{
public:
    BaseSocket();
    bool IsConnected;
    bool IsAuthed;
    PCSTR ServerAdress;
    PCSTR ServerPort;
protected:
    void open_socket();
    void close_socket(); // for now never used
    void send_packet(char buffer[BUFFER_SIZE_OUT],uint16 datalength);
    void recv_packet(char * buffer,uint16* datalength);
private:
    WSADATA wsa_data;
    int MySocket;
    std::string m_ret;
};

#endif