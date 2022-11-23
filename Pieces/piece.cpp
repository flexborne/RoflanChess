#include "piece.h"
#include "utils.h"




Piece::Piece(Side::e side, uint8_t position) noexcept : side {side},
    position {position}
{ }

auto Piece::getAvailableMoves() const -> const std::set<Move>&
{
    return availableMoves;
}

auto Piece::getAvailableMoves() -> std::set<Move>&
{
    return availableMoves;
}

void Piece::addAvailableMove(Move move)
{
    availableMoves.emplace(std::move(move));
}

void Piece::clearMoves()
{
    availableMoves.clear();
}

auto Piece::getPosition() const -> uint8_t
{
    return position;
}

QString Piece::getImagePath() const noexcept
{
    return QString::fromStdString(imagePath);
}

void Piece::setImagePath(std::string_view imagePath)
{
    this->imagePath = imagePath;
}

Side::e Piece::getSide() const noexcept
{
    return side;
}

void Piece::setImagePath(PieceType type) noexcept
{
    switch (side) {
        case Side::e::White:
            imagePath = utils::add("qrc:/images/white",
                        getPieceTypeName(type),
                        ".png");
            return;
        case Side::e::Black:
            imagePath = utils::add("qrc:/images/black",
                        getPieceTypeName(type),
                        ".png"); return;
        default:
            return;
    }
}

Piece::~Piece() = default;
