#include "rook.h"

Rook::Rook(Side::e side, uint8_t position) noexcept :
    Piece {side, position}
{
    setImagePath(getType());
}

PieceType Rook::getType() const noexcept
{
    return PieceType::Rook;
}
