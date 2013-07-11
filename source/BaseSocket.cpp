#include "BaseSocket.h"

BaseSocket::BaseSocket()
{
    IsConnected = false;
    IsAuthed    = false;
}

bool BaseSocket::open_socket()
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
        printf("getaddrinfo failed with error: %d\n", IsError);
        WSACleanup();
        return false;
    }
    
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {
        MySocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        
        if (MySocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return false;
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
        printf("Unable to connect to server!\n");
        WSACleanup();
        return false;
    }
    
    IsConnected = true;
    printf("connection successful\n");
    return true;
}

bool BaseSocket::send_packet(char buffer[BUFFER_SIZE],uint8 datalength)
{
    int IsError = send( MySocket,buffer, datalength, 0 );
    if (IsError == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(MySocket);
        WSACleanup();
    }
    return (IsError != -1);
}

bool BaseSocket::recv_packet(char* buffer, uint8* datalength)
{
    fd_set recvset;
    timeval tv;

    FD_ZERO(&recvset);
    FD_SET(MySocket,&recvset);
    tv.tv_sec  = 0;
    tv.tv_usec = 0;
    if (select(MySocket+1,&recvset,NULL,NULL,& tv) == 1)
    {
        int IsError = recv(MySocket, buffer, BUFFER_SIZE, 0);
        if (IsError <0)
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
            return false;
        }
        *datalength = (uint8)IsError;
        return true;
    }
    *datalength = 0;
    return true;
}

void BaseSocket::close_socket()
{
    closesocket(MySocket);
    WSACleanup();
    return;
}