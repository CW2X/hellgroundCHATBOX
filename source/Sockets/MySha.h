/* This source file is part of koostosh's chb project
see README for copyright notice */

/*
 * This implementations works iff data is given in whole bytes and total data size is < 2^32 bits
 */

#ifndef AUTH_MYSHA_H
#define AUTH_MYSHA_H

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

private:
    void    Process();

    uint32  h[5];
    uint8   data[64];
    uint32  counter;
    uint8   digest[20];
};

#endif
