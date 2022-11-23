#include "pseudoRandomGenerator.h"

#include "bitboard.h"
#include "engineDefines.h"

// std
#include <array>

// debug
#include "spdlog/spdlog.h"

//just a random number
uint32_t PseudoRandomGenerator::state = 1804289383;

uint32_t PseudoRandomGenerator::getRandomU32Number() {
    auto number = state;

    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    //update random number state
    state = number;

    return state;
}

uint64_t PseudoRandomGenerator::getRandomU64Number() {
    const auto slicedRandom = [&](auto shift = 0) -> uint64_t {
        return ((static_cast<uint64_t>(getRandomU32Number()) & 0xFFFF) << shift);
    };

    return slicedRandom(0) | slicedRandom(16) | slicedRandom(32) | slicedRandom(48);

}

uint64_t PseudoRandomGenerator::generateMagicNumber() {
    return getRandomU64Number() & getRandomU64Number() & getRandomU64Number();
}


uint64_t PseudoRandomGenerator::findMagicNumber(auto square, PieceType pieceType) {
    std::array<uint64_t, 4096> occupancies, attacks, usedAttacks;

    uint64_t relevantBits, attacksMask;

    using enum PieceType;

    switch (pieceType) {
    case Bishop: {
        relevantBits = bishopRelevantBits[square];
        attacksMask = maskBishopAttacks[square];
        break;
    }
    case Rook: {
        relevantBits = rookRelevantBits[square];
        attacksMask = maskRookAttacks[square];
        break;
    }
    default:
        return 0ULL;
    }

    //init occupancy indices
    int occupancyIndices = (1 << relevantBits);

    //loop over occupancy indices
    // loop over occupancy indicies
    for (int index = 0; index < occupancyIndices; ++index)
    {
        // init occupancies
        occupancies[index] = Bitboard::Functions::setOccupancy(index, attacksMask);

        // init attacks
        switch (pieceType) {
        case Bishop: {
            attacks[index] = Bitboard::Generation::generateBishopAttacksOnTheFly(square, occupancies[index]);
            break;
        }
        case Rook: {
            attacks[index] = Bitboard::Generation::generateRookAttacksOnTheFly(square, occupancies[index]);
            break;
        }
        default:
            return 0ULL;
        }
    }

    static constexpr uint32_t MAX_RANDOM_COUNT = 1e8;

    //test magick numbers loop
    for (uint32_t randomCount = 0; randomCount < MAX_RANDOM_COUNT; ++randomCount) {
        // generate magic number candidate
        uint64_t magicNumber = generateMagicNumber();

        // skip inappropriate magic numbers
        if (Bitboard::Functions::countBits((attacksMask * magicNumber) & 0xFF00000000000000) < 6) continue;

        // init used attacks
        usedAttacks.fill(0ULL);
        // init index & fail flag
        int index, fail;

        // test magic index loop
        for (index = 0, fail = 0; !fail && index < occupancyIndices; ++index) {
            // init magic index
            int magicIndex = static_cast<int>((occupancies[index] * magicNumber) >> (N_SQUARES - relevantBits));

            // if magic index works
            if (usedAttacks[magicIndex] == 0ULL)
                // init used attacks
                usedAttacks[magicIndex] = attacks[index];

            // otherwise
            else if (usedAttacks[magicIndex] != attacks[index])
                // magic index doesn't work
                fail = 1;
        }
        //if magic number works
        if (!fail)
            return magicNumber;

    }

    //otherwise
    spdlog::error("Magic number fails! ");

    return 0ULL;
}


///* @brief
/* This method was used for creation bishopMagicNumbers & rookMagicNumbers
    it could be constexpr, but i don't see the reason of making whole call stack
    constexpr, so i just copied result into variables */
void PseudoRandomGenerator::initMagicNumbers(PieceType pieceType) {
    for (auto square = 0; square < N_SQUARES; ++square) {
        printf(" 0x%llxULL,\n", findMagicNumber(square, pieceType));
    }
    printf(" \n\n\n");
}
