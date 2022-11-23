#ifndef KING_H
#define KING_H

#include "piece.h"

class King : public Piece
{
public:
    explicit King(Side::e side, uint8_t position) noexcept;

    // Piece interface
public:
    PieceType getType() const noexcept final;
};

#endif // KING_H
