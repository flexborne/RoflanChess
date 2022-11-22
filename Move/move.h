#ifndef MOVE_H
#define MOVE_H


#include <QObject>

// std
#include <cstdint>

// utils
#include "enumArray.h"
#include "defines.h"

ENUM_DEFINE(MoveType, EnPassant, DoublePush, Capture, Castling, Quiet, QuietPromotion, CapturePromotion);

extern const EnumArray<MoveType, std::string_view> moveType_longNames;

class Move
{
    Q_GADGET
    Q_PROPERTY(int source_square READ getSourceSquare)
    Q_PROPERTY(int target_square READ getTargetSquare)
public:
    Move() = default;
    ~Move() = default;

    Move(PieceType pieceType, Side::e pieceSide, uint8_t sourceSquare, uint8_t targetSquare, MoveType moveType) noexcept;

    MoveType getType() const;

    uint8_t getTargetSquare() const;

    uint8_t getSourceSquare() const;

    PieceType getPieceType() const;

    Side::e getPieceSide() const;

    /// @brief custom ordering operator for storing class in set
    bool operator<(const Move& move) const;

    /*
              binary move bits                          hexidecimal constants

        00 0000 0000 0000 0011 1111    source square       0x3f
        00 0000 0000 1111 1100 0000    target square       0xfc0
        00 0000 1111 0000 0000 0000    piece type          0xf000
        00 1111 0000 0000 0000 0000    move type           0xf0000
        11 0000 0000 0000 0000 0000    piece side          300000
    */

    uint64_t encode() const noexcept;
    static Move decode(uint64_t encoded) noexcept;

private:
    PieceType pieceType;
    Side::e pieceSide;

    uint8_t sourceSquare, targetSquare;

    MoveType type;
};

Q_DECLARE_METATYPE(Move);

#endif // MOVE_H
