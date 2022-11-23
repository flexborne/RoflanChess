#ifndef SMALLPIECEINFO_H
#define SMALLPIECEINFO_H

#include "defines.h"

/// @brief piece creation info

struct SmallPieceInfo
{
    PieceType type;
    Side::e side;
    uint8_t position;

    bool operator<(const SmallPieceInfo& other) const
    {
        return std::tie(type, side, position) < std::tie(other.type, other.side, other.position);
    }

};

struct SmallPieceInfoEx : public SmallPieceInfo
{
    std::optional<uint8_t> movedTo;
    std::optional<PieceType> promotedTo = std::nullopt;

    SmallPieceInfoEx(PieceType type, Side::e side, uint8_t position,
                     std::optional<uint8_t> movedTo = std::nullopt) :
        SmallPieceInfo{.type = type, .side = side, .position = position},
        movedTo{movedTo} {}
};

#endif // SMALLPIECEINFO_H
