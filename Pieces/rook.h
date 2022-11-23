#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

class Rook : public Piece
{
public:
    explicit Rook(Side::e side, uint8_t position) noexcept;

    // Piece interface
public:
    PieceType getType() const noexcept final;
};

#endif // ROOK_H
