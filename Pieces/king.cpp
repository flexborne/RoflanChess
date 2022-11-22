#include "king.h"

King::King(Side::e side, uint8_t position) noexcept :
    Piece {side, position}
{
    setImagePath(getType());
}

PieceType King::getType() const noexcept {
    return PieceType::King;
}
