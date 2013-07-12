#ifndef BASE_DEFINES
#define BASE_DEFINES

#include <stdio.h>
#include <string>

#define REALMLIST_ADDRESS "25.105.135.246"//"logon.hellground.net"
#define MAIN_LOGIN "user2"//"emerytura125"
#define MAIN_PASSWORD "f318ad75bf6d4f5b219984dc565bd072b43bba8f"//"4f3c709b69dd490932bdd5bde14a0dbcc0027529"
#define AUTH_PORT "3724"
#define BUFFER_SIZE 1024

typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned __int64 uint64;
typedef signed char      int8;
typedef signed short     int16;
typedef signed int       int32;
typedef signed __int64   int64;

#define MAKE_UINT32(a,b,c,d)    uint32(((uint8)d) | ((uint8)c << 8) | ((uint8)b << 16) | ((uint8)a << 24))
#define MAKE_UINT16(a,b)        uint16(((uint8)b) | ((uint8)a << 8))

struct inc_pack
{
    uint16  size;               //this is just size of data
    uint16  cmd;
    uint8   data[BUFFER_SIZE];
};

struct out_pack
{
    uint16  size;               //this is just size of data
    uint32  cmd;
    uint8   data[BUFFER_SIZE];

};
#endif