#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

class Pawn : public Piece
{
public:
    explicit Pawn(Side::e side, uint8_t position) noexcept;

    // Piece interface
public:
    PieceType getType() const noexcept final;
};

#endif // PAWN_H
