#ifndef ENGINEDEFINES_H
#define ENGINEDEFINES_H

// std
#include <array>

#include "defines.h"


constexpr uint8_t N_BITBOARDS = 12;
constexpr uint8_t N_OCCUPANCY_BITBOARDS = 3;


//precalculated 8x8 bitmap boards for easier moves calculation

extern const uint64_t bitboardWithoutAFile;
/*     8x8 board without 'a' file
 8     0 1 1 1 1 1 1 1
 7     0 1 1 1 1 1 1 1
 6     0 1 1 1 1 1 1 1
 5     0 1 1 1 1 1 1 1
 4     0 1 1 1 1 1 1 1
 3     0 1 1 1 1 1 1 1
 2     0 1 1 1 1 1 1 1
 1     0 1 1 1 1 1 1 1
       a b c d e f g h */

extern const uint64_t bitboardWithoutABFile;
/*     8x8 board without 'a' and 'b' files
 8     0 1 1 1 1 1 1 1
 7     0 1 1 1 1 1 1 1
 6     0 1 1 1 1 1 1 1
 5     0 1 1 1 1 1 1 1
 4     0 1 1 1 1 1 1 1
 3     0 1 1 1 1 1 1 1
 2     0 1 1 1 1 1 1 1
 1     0 1 1 1 1 1 1 1
       a b c d e f g h */

extern const uint64_t bitboardWithoutHFile;
/*     8x8 board without 'h' file
 8     1 1 1 1 1 1 1 0
 7     1 1 1 1 1 1 1 0
 6     1 1 1 1 1 1 1 0
 5     1 1 1 1 1 1 1 0
 4     1 1 1 1 1 1 1 0
 3     1 1 1 1 1 1 1 0
 2     1 1 1 1 1 1 1 0
 1     1 1 1 1 1 1 1 0
       a b c d e f g h */

extern const uint64_t bitboardWithoutHGFile;
/*     8x8 board without 'h' and 'g' files
 8     1 1 1 1 1 1 0 0
 7     1 1 1 1 1 1 0 0
 6     1 1 1 1 1 1 0 0
 5     1 1 1 1 1 1 0 0
 4     1 1 1 1 1 1 0 0
 3     1 1 1 1 1 1 0 0
 2     1 1 1 1 1 1 0 0
 1     1 1 1 1 1 1 0 0
       a b c d e f g h */

extern const std::array<uint64_t, N_SQUARES> bishopMagicNumbers;
extern const std::array<uint64_t, N_SQUARES> rookMagicNumbers;

extern const std::array<std::array<uint64_t, N_SQUARES>, 2> pawnAttacks;
extern const std::array<uint64_t, N_SQUARES> knightAttacks;
extern const std::array<uint64_t, N_SQUARES> kingAttacks;
extern const std::array<uint64_t, N_SQUARES> maskBishopAttacks;
extern const std::array<uint64_t, N_SQUARES> maskRookAttacks;

// relevany occupancy bit count for every square on the board
extern const std::array<uint64_t, N_SQUARES> bishopRelevantBits;
extern const std::array<uint64_t, N_SQUARES> rookRelevantBits;

extern const std::array<std::array<uint64_t, 512>, N_SQUARES> bishopAttacks;
extern const std::array<std::array<uint64_t, 4096>, N_SQUARES> rookAttacks;

inline constexpr std::array<uint64_t, N_SQUARES> bishopRelevantBits {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6,
};

inline constexpr std::array<uint64_t, N_SQUARES> rookRelevantBits {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12,
};

inline constexpr std::array<uint64_t, N_SQUARES> castlingRights {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

// get bishop attacks assuming current border occupancy
static inline uint64_t getBishopAttacks(auto square, uint64_t occupancy) {
    occupancy &= maskBishopAttacks[square];
    occupancy *= bishopMagicNumbers[square];

    occupancy >>= N_SQUARES - bishopRelevantBits[square];
    return bishopAttacks[square][occupancy];
}

// get rook attacks assuming current border occupancy
static inline uint64_t getRookAttacks(auto square, uint64_t occupancy) {
    occupancy &= maskRookAttacks[square];
    occupancy *= rookMagicNumbers[square];

    occupancy >>= N_SQUARES - rookRelevantBits[square];

    return rookAttacks[square][occupancy];
}

// get queen attacks assuming current border occupancy
static inline uint64_t getQueenAttacks(auto square, uint64_t occupancy) {
    return getRookAttacks(square, occupancy) | getBishopAttacks(square, occupancy);
}


#endif // ENGINEDEFINES_H
