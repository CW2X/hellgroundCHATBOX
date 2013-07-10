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

#include "..\base_defs.h"
#include "sha.h"
#include "BigNumber.h"

class Sha1Hash
{
    public:
        Sha1Hash();
        ~Sha1Hash();

        void UpdateFinalizeBigNumbers(BigNumber *bn0, ...);
        void UpdateBigNumbers(BigNumber *bn0, ...);

        void UpdateData(const uint8 *dta, int len);
        void UpdateData(const std::string &str);

        void Initialize();
        void Finalize();

        uint8 *GetDigest(void) { return mDigest; };
        int GetLength(void) { return SHA_DIGEST_LENGTH; };

        BigNumber GetBigNumber();

    private:
        SHA_CTX mC;
        uint8 mDigest[SHA_DIGEST_LENGTH];
};

