#include <WinSock2.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include "base_defs.h"
#include "AuthProcessor.h"

bool startup_connection();
int IO_SOCKET;
AuthProcessor sProcessor;

int main( void )

{
    char recvbuf[ BUFFER_SIZE ];
    int IsError;
    
    if(!startup_connection())
        return 1;
    
    if (!sProcessor.send_logon_challenge())
        return 1;

    while(1)
    {
        IsError = recv(IO_SOCKET, recvbuf, BUFFER_SIZE, 0);
        if ( IsError > 0 )
        {
            printf("Bytes received: %d\n", IsError);
            if (!sProcessor.handle_incoming(recvbuf,IsError))
                return 1;
        }
        else if (IsError <0)
            printf("recv failed with error: %d\n", WSAGetLastError());
    }

    closesocket(IO_SOCKET);
    WSACleanup();
    printf("exiting");

    return 0;
}

bool startup_connection()
{
    printf("initializing connection \n");
    WSADATA wsa_data;
    struct addrinfo *result = NULL,*ptr = NULL,hints;
    int IsError;

    WSAStartup(MAKEWORD(2,2),& wsa_data);

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    IsError = getaddrinfo(TARGET_ADDRESS, AUTH_PORT, &hints, &result);
    if ( IsError != 0 ) {
        printf("getaddrinfo failed with error: %d\n", IsError);
        WSACleanup();
        return false;
    }

    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        IO_SOCKET = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (IO_SOCKET == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return false;
        }

        // Connect to server.
        IsError = connect( IO_SOCKET, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (IsError == SOCKET_ERROR) {
            closesocket(IO_SOCKET);
            IO_SOCKET = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (IO_SOCKET == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return false;
    }
    
    return true;
}

int send_packet(char buffer[BUFFER_SIZE],uint8 datalength)
{
    int IsError = send( IO_SOCKET,buffer, datalength, 0 );
    if (IsError == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(IO_SOCKET);
        WSACleanup();
    }
    else
        printf("Bytes sent: %ld\n", IsError);
    return IsError;
}