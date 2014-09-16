#ifndef _AUTH_MYSHA_H
#define _AUTH_MYSHA_H

#include "..\base_defs.h"

#define DIGEST_SIZE 20
class MySha
{
public:
    MySha() { Init(); };
    void AppendData(const uint8 *input, int length);
    void Init();
    void End();
    uint8 *GetDigest() { return (uint8*)h; };
private:
    void    Process();
    void    FillUp();

    uint32  h[5];
    uint32  data[16];
    uint16  counter;
};

#endif
