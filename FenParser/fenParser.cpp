#include "fenParser.h"

// std
#include <stdexcept>
#include <tuple>

// utils
#include "enumArray.h"

// default start position in fen notation
const std::string_view DEFAULT_START_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

namespace {

    static const EnumArray<PieceType, char> pieceType_to_character { {
        { PieceType::Pawn,   'p' },
        { PieceType::King,   'k' },
        { PieceType::Bishop, 'b' },
        { PieceType::Rook,   'r' },
        { PieceType::Queen,  'q' },
        { PieceType::Knight, 'n' },
    } };

    PieceType getPieceType(char ch) noexcept(false)
    {
        if (auto typeOpt = pieceType_to_character.parseEnum(ch)) {
            return *typeOpt;
        }

        throw std::invalid_argument("[getPieceType] Invalid character");
    }

    Side::e getSideSide(char ch)
    {
        return std::isupper(ch) ? Side::e::White : Side::e::Black;
    }

    std::uint8_t charToDigit(char ch) noexcept(false)
    {
        if (!std::isdigit(ch)) {
            throw std::invalid_argument("[charToDigit] char is not a digit!");
        }

        return ch - 48;
    }

}
//"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
auto FenParser::parseFen(std::string_view fen) -> ParseResult {
    ParseResult res;

    try {
        uint8_t position = 0;
        uint8_t it = 0;

        // pieces placement info
        for (auto ch : fen) {
            ++it;
            if (ch == ' ') {
                break;
            }

            if (std::isdigit(ch)) {
                position += charToDigit(ch);
                continue;
            }

            if (ch != '/') {
                res.piecesToCreate.insert(SmallPieceInfo{
                                           .type = getPieceType(std::tolower(ch)),
                                           .side = getSideSide(ch),
                                           .position = position});
                ++position;
            }
        }

        // side info
        switch (fen[it]) {
            case 'w': {
                res.toMove = Side::e::White;
                break;
            }

            case 'b': {
                res.toMove = Side::e::Black;
                break;
            }
            default:
                throw std::invalid_argument("Invalid side");
        }

        // castling rights
        it += 2;
        while (fen[it] != ' ') {
            switch (fen[it]) {
                case 'K': res.castleRights.set(1, true); break;
                case 'Q': res.castleRights.set(0, true); break;
                case 'k': res.castleRights.set(3, true); break;
                case 'q': res.castleRights.set(2, true); break;
                case '-': break;
            }
            ++it;
        }

        ++it;

        // enpassant square
        if (fen[it] != '-') {
            res.enPassantSquare = (fen[it] - 'a') +
                    8*(8 - (fen[it + 1] - '0'));
        }
    } catch(...) {
        /// @todo qt message with an error
        throw std::invalid_argument("[parseFen] Invalid fen");
    }

    return res;
}
