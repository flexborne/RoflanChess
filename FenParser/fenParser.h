#ifndef FENPARSER_H
#define FENPARSER_H

// std
#include <set>
#include <string_view>
#include <optional>
#include <bitset>

#include "defines.h"
#include "smallPieceInfo.h"

extern const std::string_view DEFAULT_START_POSITION;

class FenParser
{
public:
    FenParser() noexcept = default;

    struct ParseResult
    {
        std::set<SmallPieceInfo> piecesToCreate;
        Side::e  toMove;
        std::bitset<4> castleRights {0ULL};
        std::optional<uint8_t> enPassantSquare = std::nullopt;
    };

    static auto parseFen(std::string_view fen = DEFAULT_START_POSITION)
        -> ParseResult;
private:

};

#endif // FENPARSER_H
