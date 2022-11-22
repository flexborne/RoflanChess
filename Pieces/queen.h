#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

class Queen : public Piece
{
public:
    explicit Queen(Side::e side, uint8_t position) noexcept;

    // Piece interface
public:
    PieceType getType() const noexcept final;
};

#endif // QUEEN_H
