/*
 * This implementations works iff data is given in whole bytes and total data size is < 2^32 bits
 */

#ifndef _AUTH_MYSHA_H
#define _AUTH_MYSHA_H

#include "..\base_defs.h"

class MySha
{
public:
    MySha() { Initialize(); };
    void UpdateData(uint8 *input, uint32 length);
    void UpdateData(const std::string &str)
    {
        UpdateData((uint8*)str.c_str(), str.length());
    }
    void Initialize();
    void Finalize();
    uint8 *GetDigest() { return digest; };

    static void ModExpSimple(uint8* r,uint8 g,uint8* p,uint8* N);
    static void ModExpFull(uint8* S,uint8* B,uint8* v,uint8* a,uint8* u,uint8* x,uint8* N);
    static void SetRand(uint8* a);
private:
    void    Process();

    uint32  h[5];
    uint8   data[64];
    uint32  counter;
    uint8   digest[20];
};

#endif
