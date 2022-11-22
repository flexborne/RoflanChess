#include "engineDefines.h"
#include "bitboard.h"

constexpr const uint64_t bitboardWithoutAFile = 18374403900871474942ULL;
constexpr const uint64_t bitboardWithoutABFile = 18229723555195321596ULL;
constexpr const uint64_t bitboardWithoutHFile = 9187201950435737471ULL;
constexpr const uint64_t bitboardWithoutHGFile = 4557430888798830399ULL;

constexpr std::array<uint64_t, N_SQUARES> rookMagicNumbers {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL
};

constexpr std::array<uint64_t, N_SQUARES> bishopMagicNumbers {
    0x1412500602041020ULL,
     0x5460624206022000ULL,
     0x83000c608400000ULL,
     0x4004040083108422ULL,
     0xc1114020000006ULL,
     0x9024300410400048ULL,
     0x2020262410000ULL,
     0x4202110101085ULL,
     0x1060213890408288ULL,
     0xb8202809350820ULL,
     0x404100112002400ULL,
     0x4600404008a0002ULL,
     0x4c01040422221021ULL,
     0x8824120600509ULL,
     0x244c008210022002ULL,
     0x84002a0049180820ULL,
     0x920101142900900ULL,
     0x2404006028220841ULL,
     0x1000120382000dULL,
     0x8001041404011ULL,
     0x8803400a02000ULL,
     0x1810048044002ULL,
     0x6008808044100808ULL,
     0x202000109010104ULL,
     0x3182400022180240ULL,
     0x8201043660041401ULL,
     0x18004804010e0402ULL,
     0x480a040008010820ULL,
     0x2194082044002002ULL,
     0x2008a20001004200ULL,
     0x40908041041004ULL,
     0x881002200540404ULL,
     0x4001082002082101ULL,
     0x8110408880880ULL,
     0x8000404040080200ULL,
     0x200020082180080ULL,
     0x1184440400114100ULL,
     0xc220008020110412ULL,
     0x4088084040090100ULL,
     0x8822104100121080ULL,
     0x100111884008200aULL,
     0x2844040288820200ULL,
     0x90901088003010ULL,
     0x1000a218000400ULL,
     0x1102010420204ULL,
     0x8414a3483000200ULL,
     0x6410849901420400ULL,
     0x201080200901040ULL,
     0x204880808050002ULL,
     0x1001008201210000ULL,
     0x16a6300a890040aULL,
     0x8049000441108600ULL,
     0x2212002060410044ULL,
     0x100086308020020ULL,
     0x484241408020421ULL,
     0x105084028429c085ULL,
     0x4282480801080cULL,
     0x81c098488088240ULL,
     0x1400000090480820ULL,
     0x4444000030208810ULL,
     0x1020142010820200ULL,
     0x2234802004018200ULL,
     0xc2040450820a00ULL,
     0x2101021090020ULL,
};

//consteval possible moves generation
namespace Bitboard::Generation::Constexpr {
    consteval uint64_t generateMaskPawnAttacks(auto square, Side::e side) {
        uint64_t attacks = 0ULL;
        uint64_t board = 0ULL;

        //set piece on board
        Bitboard::Functions::setBit(board, square);

        switch (side) {
            case Side::e::White: {
                if ((board >> (BOARD_SIZE - 1)) & bitboardWithoutAFile)
                    attacks |= (board >> (BOARD_SIZE - 1));
                if ((board >> (BOARD_SIZE + 1)) & bitboardWithoutHFile)
                    attacks |= (board >> (BOARD_SIZE + 1));
                break;
            }
            case Side::e::Black: {
                if ((board << (BOARD_SIZE - 1)) & bitboardWithoutHFile)
                    attacks |= (board << (BOARD_SIZE - 1));
                if ((board << (BOARD_SIZE + 1)) & bitboardWithoutAFile)
                    attacks |= (board << (BOARD_SIZE + 1));
                break;
            }
            default:
                return attacks;
        }

        return attacks;
    }

    consteval uint64_t generateMaskKnightAttacks(auto square) {
        uint64_t attacks = 0ULL;
        uint64_t board = 0ULL;

        //set piece on board
        Bitboard::Functions::setBit(board, square);

        if (auto newPlace = board >> (2*BOARD_SIZE + 1); newPlace & bitboardWithoutHFile) {
            attacks |= newPlace;
        }
        if (auto newPlace = board >> (2*BOARD_SIZE - 1); newPlace & bitboardWithoutAFile) {
            attacks |= newPlace;
        }
        if (auto newPlace = board >> (BOARD_SIZE + 2); newPlace & bitboardWithoutHGFile) {
            attacks |= newPlace;
        }
        if (auto newPlace = board >> (BOARD_SIZE - 2); newPlace & bitboardWithoutABFile) {
            attacks |= newPlace;
        }

        if (auto newPlace = board << (2*BOARD_SIZE + 1); newPlace & bitboardWithoutAFile) {
            attacks |= newPlace;
        }
        if (auto newPlace = board << (2*BOARD_SIZE - 1); newPlace & bitboardWithoutHFile) {
            attacks |= newPlace;
        }
        if (auto newPlace = board << (BOARD_SIZE + 2); newPlace & bitboardWithoutABFile) {
            attacks |= newPlace;
        }
        if (auto newPlace = board << (BOARD_SIZE - 2); newPlace & bitboardWithoutHGFile) {
            attacks |= newPlace;
        }

        return attacks;
    }

    consteval uint64_t generateMaskKingAttacks(auto square) {
        uint64_t attacks = 0ULL;
        uint64_t board = 0ULL;

        //set piece on board
        Bitboard::Functions::setBit(board, square);

        if (board >> BOARD_SIZE) attacks |= (board >> BOARD_SIZE);
        if ((board >> (BOARD_SIZE + 1)) & bitboardWithoutHFile) attacks |= (board >> (BOARD_SIZE + 1));
        if ((board >> (BOARD_SIZE - 1)) & bitboardWithoutAFile) attacks |= (board >> (BOARD_SIZE - 1));
        if ((board >> 1) & bitboardWithoutHFile) attacks |= (board >> 1);

        if (board << BOARD_SIZE) attacks |= (board << BOARD_SIZE);
        if ((board << (BOARD_SIZE + 1)) & bitboardWithoutAFile) attacks |= (board << (BOARD_SIZE + 1));
        if ((board << (BOARD_SIZE - 1)) & bitboardWithoutHFile) attacks |= (board << (BOARD_SIZE - 1));
        if ((board << 1) & bitboardWithoutHFile) attacks |= (board << 1);

        return attacks;
    }

    consteval uint64_t generateMaskBishopAttacks(auto square) {
        uint64_t attacks = 0ULL;

        //init target ranks and files
        const int targetRank = square / BOARD_SIZE;
        const int targetFile = square % BOARD_SIZE;

        //mask relevant bishop occupancy bits
        for (auto rank = targetRank + 1, file = targetFile + 1; rank < BOARD_SIZE - 1 && file < BOARD_SIZE - 1; ++rank, ++file) {
            attacks |= (1ULL << (BOARD_SIZE*rank + file));
        }
        for (auto rank = targetRank - 1, file = targetFile + 1; rank > 0 && file < BOARD_SIZE - 1; --rank, ++file) {
            attacks |= (1ULL << (BOARD_SIZE*rank + file));
        }
        for (auto rank = targetRank + 1, file = targetFile - 1; rank < BOARD_SIZE - 1 && file > 0; ++rank, --file) {
            attacks |= (1ULL << (BOARD_SIZE*rank + file));
        }
        for (auto rank = targetRank - 1, file = targetFile - 1; rank > 0 && file > 0; --rank, --file) {
            attacks |= (1ULL << (BOARD_SIZE*rank + file));
        }

        return attacks;
    }

    consteval uint64_t generateMaskRookAttacks(auto square) {
        uint64_t attacks = 0ULL;

        //init target ranks and files
        const int targetRank = square / BOARD_SIZE;
        const int targetFile = square % BOARD_SIZE;

        //mask relevant rook occupancy bits
        for (auto rank = targetRank + 1; rank < BOARD_SIZE - 1; ++rank) {
            attacks |= (1ULL << (BOARD_SIZE*rank + targetFile));
        }
        for (auto rank = targetRank - 1; rank > 0; --rank) {
            attacks |= (1ULL << (BOARD_SIZE*rank + targetFile));
        }
        for (auto file = targetFile + 1; file < BOARD_SIZE - 1; ++file) {
            attacks |= (1ULL << (BOARD_SIZE*targetRank + file));
        }
        for (auto file = targetFile - 1; file > 0; --file) {
            attacks |= (1ULL << (BOARD_SIZE*targetRank + file));
        }

        return attacks;
    }

    //array of bitboards for pawns attack
    consteval auto generatePawnAttacksAll() {
        std::array<std::array<uint64_t, N_SQUARES>, 2> pawnAttacks {};

        for (auto square = 0; square < N_SQUARES; ++square) {
            //result attacks bitboard
            pawnAttacks[static_cast<int>(Side::e::White)][square] = generateMaskPawnAttacks(square, Side::e::White);
            pawnAttacks[static_cast<int>(Side::e::Black)][square] = generateMaskPawnAttacks(square, Side::e::Black);
        }

        return pawnAttacks;
    }

    consteval auto generatePieceAttacksAll(PieceType pieceType) {
        using enum PieceType;

        std::array<uint64_t, N_SQUARES> pieceAttacks {};

        for (auto square = 0; square < N_SQUARES; ++square) {
            //result attacks bitboard
            switch (pieceType) {
            case Knight:
                pieceAttacks[square] = generateMaskKnightAttacks(square); break;
            case King:
                pieceAttacks[square] = generateMaskKingAttacks(square); break;
            case Bishop:
                pieceAttacks[square] = generateMaskBishopAttacks(square); break;
            case Rook:
                pieceAttacks[square] = generateMaskRookAttacks(square); break;
            default:
                break;
            }
        }
        return pieceAttacks;
    }

    template <int N>
    constexpr auto generateSlidersAttacks(PieceType pieceType) {
        using enum PieceType;


        std::array<std::array<uint64_t, N>, N_SQUARES> result {};

        for (auto square = 0; square < N_SQUARES; ++square) {
            uint64_t attackMask;

            switch (pieceType) {
            case Bishop:
                 attackMask = maskBishopAttacks[square]; break;
            case Rook:
                 attackMask = maskRookAttacks[square]; break;
            default:
                return result; //must be an exception
            }

            auto relevantBitsCount = Functions::countBits(attackMask);

            auto occupancyIndices = (1 << relevantBitsCount);

            //loop over occupancy indices
            for (auto index = 0; index < occupancyIndices; ++index) {
                uint64_t occupancy = Functions::setOccupancy(index, attackMask);
                uint64_t magicIndex;

                switch (pieceType) {
                case Bishop:
                    magicIndex = (occupancy*bishopMagicNumbers[square]) >> (N_SQUARES - bishopRelevantBits[square]);
                    result[square][magicIndex] = generateBishopAttacksOnTheFly(square, occupancy);
                    break;
                case Rook:
                    magicIndex = (occupancy*rookMagicNumbers[square]) >> (N_SQUARES - rookRelevantBits[square]);
                    result[square][magicIndex] = generateRookAttacksOnTheFly(square, occupancy);
                    break;
                default:
                    return result;
                }
            }
        }
        return result;
    }
} //end of namespace

constexpr std::array<std::array<uint64_t, N_SQUARES>, 2> pawnAttacks = Bitboard::Generation::Constexpr::generatePawnAttacksAll();
constexpr std::array<uint64_t, N_SQUARES> knightAttacks = Bitboard::Generation::Constexpr::generatePieceAttacksAll(PieceType::Knight);
constexpr std::array<uint64_t, N_SQUARES> kingAttacks = Bitboard::Generation::Constexpr::generatePieceAttacksAll(PieceType::King);
constexpr std::array<uint64_t, N_SQUARES> maskBishopAttacks = Bitboard::Generation::Constexpr::generatePieceAttacksAll(PieceType::Bishop);
constexpr std::array<uint64_t, N_SQUARES> maskRookAttacks = Bitboard::Generation::Constexpr::generatePieceAttacksAll(PieceType::Rook);

const std::array<std::array<uint64_t, 512>, N_SQUARES> bishopAttacks = Bitboard::Generation::Constexpr::generateSlidersAttacks<512>(PieceType::Bishop);
const std::array<std::array<uint64_t, 4096>, N_SQUARES> rookAttacks = Bitboard::Generation::Constexpr::generateSlidersAttacks<4096>(PieceType::Rook);
