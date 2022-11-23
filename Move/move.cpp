#include "move.h"

const EnumArray<MoveType, std::string_view> moveType_longNames { {
    { MoveType::EnPassant, "EnPassant" },
    { MoveType::DoublePush, "DoublePush" },
    { MoveType::Capture, "Capture" },
    { MoveType::Castling, "Castling" },
    { MoveType::Quiet, "Quiet" },
    { MoveType::QuietPromotion, "QuietPromotion" },
    { MoveType::CapturePromotion, "CapturePromotion" }
    } };

Move::Move(PieceType pieceType, Side::e pieceSide, uint8_t sourceSquare, uint8_t targetSquare, MoveType moveType) noexcept
    : pieceType{pieceType}, pieceSide{pieceSide}, sourceSquare{sourceSquare}, targetSquare{targetSquare}, type{moveType} {}

MoveType Move::getType() const
{
    return type;
}

uint8_t Move::getTargetSquare() const
{
    return targetSquare;
}

uint8_t Move::getSourceSquare() const
{
    return sourceSquare;
}

PieceType Move::getPieceType() const
{
    return pieceType;
}

Side::e Move::getPieceSide() const
{
    return pieceSide;
}

bool Move::operator<(const Move& piece) const
{
    return std::tie(sourceSquare, targetSquare) < std::tie(piece.sourceSquare, piece.targetSquare);
}

uint64_t Move::encode() const noexcept
{
    return sourceSquare
            | (targetSquare << 6)
            | (static_cast<size_t>(pieceType) << 12)
            | (static_cast<size_t>(type) << 16)
            | (static_cast<size_t>(pieceSide) << 20);
}

Move Move::decode(uint64_t encoded) noexcept
{
    return Move {
        static_cast<PieceType>((encoded & 0xf000) >> 12),
        static_cast<Side::e>((encoded & 0x300000) >> 20),
        static_cast<uint8_t>(encoded & 0x3f),
        static_cast<uint8_t>((encoded & 0xfc0) >> 6),
        static_cast<MoveType>((encoded & 0xf0000) >> 16)
    };
}
