#ifndef GAMEBOARDDATA_H
#define GAMEBOARDDATA_H

// std
#include <vector>
#include <optional>
#include <bitset>

#include "piece.h"
#include "defines.h"

struct GameBoardData
{
    std::vector<const Piece*> pieces;
    Side::e  sideToMove;
    std::bitset<4> castleRights;
    std::optional<uint8_t> enPassantSquare;
};

#endif // GAMEBOARDDATA_H
