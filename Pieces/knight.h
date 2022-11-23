#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

class Knight : public Piece
{
public:
    explicit Knight(Side::e side, uint8_t position) noexcept;

    // Piece interface
public:
    PieceType getType() const noexcept final;
};

#endif // KNIGHT_H
