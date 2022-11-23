#ifndef DEFINES_H
#define DEFINES_H

// std
#include <string>

// utils
#include "enumArray.h"

#include <QObject>

namespace Side
{
    Q_NAMESPACE         // required for meta object creation
    enum class e {
        White,
        Black,
        Both
    };
    Q_ENUM_NS(e)  // register the enum in meta object data
}

constexpr const uint8_t BOARD_SIZE = 8;
constexpr const uint8_t N_SQUARES = BOARD_SIZE*BOARD_SIZE;

ENUM_DEFINE(PieceType, King, Knight, Rook, Queen, Bishop, Pawn);

extern const EnumArray<PieceType, std::string> pieceType_longNames;

std::string getPieceTypeName(PieceType type);


#endif // DEFINES_H
