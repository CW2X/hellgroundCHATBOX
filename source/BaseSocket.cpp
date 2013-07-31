#include "BaseSocket.h"
#include "Util.h"

BaseSocket::BaseSocket()
{
    IsConnected = false;
    IsAuthed    = false;
}

void BaseSocket::open_socket()
{
    printf("initializing connection: %s:%s\n",ServerAdress,ServerPort);
    
    WSAStartup(MAKEWORD(2,2),& wsa_data);
    struct addrinfo *result = NULL,*ptr = NULL,hints;
    int IsError;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    IsError = getaddrinfo(ServerAdress, ServerPort, &hints, &result);
    if ( IsError != 0 ) {
        WSACleanup();
        throw string_format("getaddrinfo failed with error: %d\n", IsError);
    }
    
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
        MySocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        
        if (MySocket == INVALID_SOCKET) {
            WSACleanup();
            throw string_format("socket failed with error: %ld\n", WSAGetLastError());
        }
        
        IsError = connect( MySocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (IsError == SOCKET_ERROR) {
            closesocket(MySocket);
            MySocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    

    freeaddrinfo(result);

    if (MySocket == INVALID_SOCKET) {
        WSACleanup();
        throw "Unable to connect to server!\n";
    }
    
    IsConnected = true;
    printf("connection successful\n");
}

void BaseSocket::send_packet(char buffer[BUFFER_SIZE_OUT],uint16 datalength)
{
    int IsError = send( MySocket,buffer, datalength, 0 );
    if (IsError == SOCKET_ERROR) {
        closesocket(MySocket);
        WSACleanup();
        throw string_format("send failed with error: %d\n", WSAGetLastError());
    }
}

void BaseSocket::recv_packet(char* buffer, uint16* datalength)
{
    fd_set recvset;
    timeval tv;
    int IsError = 0;

    FD_ZERO(&recvset);
    FD_SET(MySocket,&recvset);
    tv.tv_sec  = 0;
    tv.tv_usec = 0;
    if (select(MySocket+1,&recvset,NULL,NULL,& tv) == 1)
    {
        if (*datalength == 0)
            IsError = recv(MySocket, buffer, BUFFER_SIZE_IN, 0);
        else
            IsError = recv(MySocket, buffer, *datalength, 0);
        if (IsError <0)
            throw string_format("recv failed with error: %d\n", WSAGetLastError());
        *datalength = (uint16)IsError;
        return;
    }

    *datalength = 0;
}

void BaseSocket::close_socket()
{
    closesocket(MySocket);
    WSACleanup();
    return;
}