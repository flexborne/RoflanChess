#ifndef BITBOARD_H
#define BITBOARD_H

#include "defines.h"

// std
#include <optional>

// basic functions that can be applied on bitboard
namespace Bitboard::Functions {
    constexpr auto getBit(uint64_t bitboard, auto square) {
        return bitboard & (1ULL << square);
    }

    constexpr void setBit(uint64_t& bitboard, auto square) {
        bitboard |= (1ULL << square);
    }

    /// @return is bit popped
    constexpr bool popBit(uint64_t& bitboard, auto square) {
        if (getBit(bitboard, square)) {
            bitboard ^= (1ULL << square);
            return true;
        }

        return false;
    }

    // counts number of bits of bitboard
    constexpr uint8_t countBits(uint64_t bitboard) {
        uint8_t count = 0;

        while(bitboard) {
            ++count;
            bitboard &= bitboard - 1;
        }

        return count;
    }

    //get least significant first bit index
    constexpr std::optional<uint8_t> getLSBitIndex(uint64_t bitboard) {
        if (bitboard) {
            return countBits((bitboard & -bitboard) - 1);
        }
        return std::nullopt;
    }

    constexpr uint64_t setOccupancy(auto index, uint64_t attackMask) {
        //occupancy map
        uint64_t occupancy = 0ULL;

        const auto bitsInMask = countBits(attackMask);
        //loop over the range of bits within attack mask
        for (auto count = 0; count < bitsInMask; ++count) {
            //get LS1B index
            auto square = getLSBitIndex(attackMask);

            if (square) {
                popBit(attackMask, *square);

                if (index & (1 << count))
                    occupancy |= (1ULL << *square);
            }
        }

        return occupancy;
    }
}

namespace Bitboard::Generation {
    constexpr uint64_t generateBishopAttacksOnTheFly(auto square, const uint64_t& block) {
        uint64_t attacks = 0ULL;

        //init target ranks and files
        const int targetRank = square / BOARD_SIZE;
        const int targetFile = square % BOARD_SIZE;

        const auto isBlock = [&block] (const auto& position) -> bool {
            if (position & block)
                return true;
            return false;
        };

        //mask relevant bishop occupancy bits
        for (auto rank = targetRank + 1, file = targetFile + 1; rank < BOARD_SIZE && file < BOARD_SIZE; ++rank, ++file) {
            attacks |= (1ULL << (BOARD_SIZE*rank + file));
            if (isBlock(1ULL << (BOARD_SIZE*rank + file)))
                break;
        }
        for (auto rank = targetRank - 1, file = targetFile + 1; rank > -1 && file < BOARD_SIZE; --rank, ++file) {
            attacks |= (1ULL << (BOARD_SIZE*rank + file));
            if (isBlock(1ULL << (BOARD_SIZE*rank + file)))
                break;
        }
        for (auto rank = targetRank + 1, file = targetFile - 1; rank < BOARD_SIZE && file > -1; ++rank, --file) {
            attacks |= (1ULL << (BOARD_SIZE*rank + file));
            if (isBlock(1ULL << (BOARD_SIZE*rank + file)))
                break;
        }
        for (auto rank = targetRank - 1, file = targetFile - 1; rank > -1 && file > -1; --rank, --file) {
            attacks |= (1ULL << (BOARD_SIZE*rank + file));
            if (isBlock(1ULL << (BOARD_SIZE*rank + file)))
                break;
        }

        return attacks;
    }

    constexpr uint64_t generateRookAttacksOnTheFly(auto square, const uint64_t& block) {
        uint64_t attacks = 0ULL;

        //init target ranks and files
        const int targetRank = square / BOARD_SIZE;
        const int targetFile = square % BOARD_SIZE;

        const auto isBlock = [&block] (const auto& position) -> bool {
            if (position & block)
                return true;
            return false;
        };

        //mask relevant rook occupancy bits
        for (auto rank = targetRank + 1; rank < BOARD_SIZE; ++rank) {
            attacks |= (1ULL << (BOARD_SIZE*rank + targetFile));
            if (isBlock(1ULL << (BOARD_SIZE*rank + targetFile)))
                break;
        }
        for (auto rank = targetRank - 1; rank > -1; --rank) {
            attacks |= (1ULL << (BOARD_SIZE*rank + targetFile));
            if (isBlock(1ULL << (BOARD_SIZE*rank + targetFile)))
                break;
        }
        for (auto file = targetFile + 1; file < BOARD_SIZE; ++file) {
            attacks |= (1ULL << (BOARD_SIZE*targetRank + file));
            if (isBlock(1ULL << (BOARD_SIZE*targetRank + file)))
                break;
        }
        for (auto file = targetFile - 1; file > -1; --file) {
            attacks |= (1ULL << (BOARD_SIZE*targetRank + file));
            if (isBlock(1ULL << (BOARD_SIZE*targetRank + file)))
                break;
        }

        return attacks;
    }

}   // end of namespace


#endif // BITBOARD_H
