#include "queen.h"

Queen::Queen(Side::e side, uint8_t position) noexcept :
    Piece {side, position}
{
    setImagePath(getType());
}

PieceType Queen::getType() const noexcept {
    return PieceType::Queen;
}
