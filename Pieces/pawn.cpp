#include "pawn.h"

Pawn::Pawn(Side::e side, uint8_t position) noexcept :
    Piece {side, position}

{
    setImagePath(getType());
}

PieceType Pawn::getType() const noexcept {
    return PieceType::Pawn;
}

