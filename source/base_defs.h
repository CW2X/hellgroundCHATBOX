#ifndef BASE_DEFINES
#define BASE_DEFINES

#include <stdio.h>
#include <string>

#define REALMLIST_ADDRESS "25.105.135.246"//"logon.hellground.net"
#define AUTH_PORT "3724"
#define FAKE_OS 'CHA'
#define BUFFER_SIZE_IN  2048
#define BUFFER_SIZE_OUT 256

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
    uint8   data[BUFFER_SIZE_IN];
};

struct out_pack
{
    uint16  size;               //this is just size of data
    uint32  cmd;
    uint8   data[BUFFER_SIZE_OUT];

};

struct cli_pack
{
    uint16      size;
    uint8       type;
    std::string data;
};

#endif