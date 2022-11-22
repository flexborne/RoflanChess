#ifndef PSEUDORANDOMGENERATOR_H
#define PSEUDORANDOMGENERATOR_H

#include "defines.h"

// std
#include <array>

namespace PseudoRandomGenerator {
    //pseudo random number state
    extern uint32_t state;

    uint32_t getRandomU32Number();
    uint64_t getRandomU64Number();
    uint64_t generateMagicNumber();

    uint64_t findMagicNumber(auto square, PieceType pieceType);

    void initMagicNumbers(PieceType pieceType);
}

#endif // PSEUDORANDOMGENERATOR_H
