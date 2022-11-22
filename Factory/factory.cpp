#include "factory.h"

// pieces
#include "pawn.h"
#include "king.h"
#include "queen.h"
#include "knight.h"
#include "bishop.h"
#include "rook.h"
#include "piece.h"

#include "smallPieceInfo.h"

#include <exception>

const std::map<PieceType, std::function<std::unique_ptr<Piece>(Side::e, uint8_t)>> Factory::mapping
{
    { PieceType::Pawn,   [](Side::e side, uint8_t position) { return std::make_unique<Pawn>(side, position); } },
    { PieceType::King,   [](Side::e side, uint8_t position) { return std::make_unique<King>(side, position); } },
    { PieceType::Bishop, [](Side::e side, uint8_t position) { return std::make_unique<Bishop>(side, position); } },
    { PieceType::Rook,   [](Side::e side, uint8_t position) { return std::make_unique<Rook>(side, position); } },
    { PieceType::Queen,  [](Side::e side, uint8_t position) { return std::make_unique<Queen>(side, position); } },
    { PieceType::Knight, [](Side::e side, uint8_t position) { return std::make_unique<Knight>(side, position); } }
};

Factory::Factory() { }

std::unique_ptr<Piece> Factory::create(SmallPieceInfo pieceInfo) {
    if (auto creatorFunc = mapping.find(pieceInfo.type); creatorFunc != mapping.end()) {
        return creatorFunc->second(pieceInfo.side, pieceInfo.position);
    }

    throw std::invalid_argument("[Factor::create] invalid argument");
}
