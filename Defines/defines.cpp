#include "defines.h"

// std
#include <stdexcept>

const EnumArray<PieceType, std::string> pieceType_longNames { {
    { PieceType::King, "King" },
    { PieceType::Knight, "Knight" },
    { PieceType::Rook, "Rook" },
    { PieceType::Queen, "Queen" },
    { PieceType::Bishop, "Bishop" },
    { PieceType::Pawn, "Pawn" }
    } };


std::string getPieceTypeName(PieceType type)
{
    return pieceType_longNames[type];
}
