/* This source file is part of koostosh's chb project
see README for copyright notice */

#ifndef AUTH_MONTGOMERYEXP_H
#define AUTH_MONTGMOERYEXP_H

#include "../base_defs.h"

class MontgomeryExp
{
public:
    void init(uint8* mod);
    void ModExpSimple(uint8* S, uint8 G, uint8* P);
    void ModExpFull(uint8* S, uint8* B, uint8* V, uint8* A, uint8* U, uint8* X);
private:
    void internal_ModExp(uint8* S, uint8* A, uint8* P,uint8 size);
    void step(uint8*S, uint8* A,uint8* B);
    void modN(uint8*S, uint8* A);
    uint8 N[32], Ni[32], Ri[32];
};

#endif