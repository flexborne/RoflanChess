#include "knight.h"

Knight::Knight(Side::e side, uint8_t position) noexcept :
    Piece {side, position}
{
    setImagePath(getType());
}

PieceType Knight::getType() const noexcept
{
    return PieceType::Knight;
}
