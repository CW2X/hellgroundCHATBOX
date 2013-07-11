/*
 * file based on:
 * MaNGOS <http://www.mangosproject.org/>
 * Trinity <http://www.trinitycore.org/>
 * Hellground <http://www.hellground.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#ifndef _AUTH_HMAC_H
#define _AUTH_HMAC_H

#include "../base_defs.h"
#include "hmac(OpenSSL).h"
#include "sha.h"

class BigNumber;

#define SEED_KEY_SIZE 16

class HmacHash
{
    public:
        HmacHash();
        HmacHash(uint32 len, uint8 *seed);
        ~HmacHash();
        void UpdateBigNumber(BigNumber *bn);
        void UpdateData(const uint8 *data, int length);
        void UpdateData(const std::string &str);
        void Initialize();
        void Finalize();
        uint8 *GetDigest() { return m_digest; };
        int GetLength() { return SHA_DIGEST_LENGTH; };
    private:
        HMAC_CTX m_ctx;
        uint8 m_key[SEED_KEY_SIZE];
        uint8 m_digest[SHA_DIGEST_LENGTH];
};
#endif

