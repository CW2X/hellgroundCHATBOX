#define TARGET_ADDRESS "25.105.135.246"
#define MAIN_LOGIN "user2"
#define MAIN_PASSWORD "f318ad75bf6d4f5b219984dc565bd072b43bba8f"
#define AUTH_PORT "3724"
#define BUFFER_SIZE 256

typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned __int64 uint64;
typedef signed char      int8;
typedef signed short     int16;
typedef signed int       int32;
typedef signed __int64   int64;
int send_packet(char buffer[BUFFER_SIZE],uint8 datalength);