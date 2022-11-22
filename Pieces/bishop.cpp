#include "bishop.h"

Bishop::Bishop(Side::e side, uint8_t position) noexcept :
    Piece {side, position}
{
    setImagePath(getType());
}

PieceType Bishop::getType() const noexcept {
    return PieceType::Bishop;
}
